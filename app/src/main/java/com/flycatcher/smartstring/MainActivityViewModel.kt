package com.flycatcher.smartstring

import android.graphics.Bitmap
import android.os.FileUtils.ProgressListener
import android.util.Log
import androidx.annotation.MainThread
import androidx.lifecycle.LiveData
import androidx.lifecycle.SavedStateHandle
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.CoroutineName
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.async
import kotlinx.coroutines.cancelAndJoin
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class MainActivityViewModel(
    private val savedStateHandle: SavedStateHandle
) : ViewModel() {

    private val jniBridge = JniBridge()

    private var _currentPicture: StateFlow<AssetPicture> =
        savedStateHandle.getStateFlow(PICTURE_KEY, AssetPicture.SMILE_GIRL_GREY)
    val currentPicture = _currentPicture

    private var _bitmapFlow = MutableStateFlow<Bitmap?>(null)
    val bitmapFlow = _bitmapFlow

    private var generatorJob: Job? = null


    fun setBitmap(bitmap: Bitmap) {
        bitmapFlow.value = bitmap
    }

    fun showPrevPicture() {
        generatorJob?.cancel()
        savedStateHandle[PICTURE_KEY] = currentPicture.value.prev()
    }

    fun showNextPicture() {
        generatorJob?.cancel()
        savedStateHandle[PICTURE_KEY] = currentPicture.value.next()
    }

    fun generatePicture(bitmap: Bitmap, pins: Int, minDistance: Int, maxLines: Int,
                        lineWeight: Int) {
        generatorJob?.cancel()
        generatorJob = viewModelScope.launch(CoroutineName("BitmapGeneration")) {
            val progress = StringArtProgressImpl()
            val bitmapDeferred = async {
                jniBridge.greyImage(bitmap, pins, minDistance, maxLines, lineWeight, progress)
            }
            bitmapFlow.value = bitmapDeferred.await()
        }
    }

    fun checkProgress() {
        jniBridge.callbackJNI(object : JniBridge.JNIListener {
            override fun sendProgress(progress: Int, msg: String) {
                Log.d("JniCallback", "Message $msg is $progress%")
            }

        })
    }

    companion object {
        val PICTURE_KEY = "picture_key"
    }

    class StringArtProgressImpl: JniBridge.StringArtProgress {
        override fun sendProgress(progress: Int) {
            Log.d("JNI", "Progress: $progress")
        }

    }

}