package com.flycatcher.smartstring

import android.graphics.Bitmap
import android.util.Log
import androidx.lifecycle.SavedStateHandle
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.CoroutineName
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.async
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.channels.consumeEach
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.receiveAsFlow
import kotlinx.coroutines.launch

class MainActivityViewModel(
    private val savedStateHandle: SavedStateHandle
) : ViewModel() {

    private val jniBridge = JniBridge()

    private var _currentPicture: StateFlow<AssetPicture> =
        savedStateHandle.getStateFlow(PICTURE_KEY, AssetPicture.SMILE_GIRL_GREY)
    val currentPicture = _currentPicture

    private var _bitmapFlow = MutableStateFlow<Bitmap?>(null)
    val bitmapFlow = _bitmapFlow

    private var _progress = MutableStateFlow(0)
    val progress = _progress

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

    fun generatePicture(
        bitmap: Bitmap, pins: Int, minDistance: Int, maxLines: Int,
        lineWeight: Int
    ) {

        generatorJob?.cancel()
        generatorJob = viewModelScope.launch(CoroutineName("BitmapGeneration")) {
            _bitmapFlow.value = jniBridge.stringArtImage(
                bitmap, 500, 500,
                pins, minDistance, maxLines, lineWeight,
                object : JniBridge.StringArtProgress {
                    override fun sendProgress(progress: Int) {
                        _progress.value = progress
                    }
                }
            )
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

}