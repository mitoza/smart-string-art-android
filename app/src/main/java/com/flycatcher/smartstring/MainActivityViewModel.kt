package com.flycatcher.smartstring

import android.graphics.Bitmap
import androidx.annotation.MainThread
import androidx.lifecycle.LiveData
import androidx.lifecycle.SavedStateHandle
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.CoroutineName
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.async
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class MainActivityViewModel(
    private val savedStateHandle: SavedStateHandle
) : ViewModel() {

    private val jniBridge = JniBridge()

    private var _currentPicture: StateFlow<AssetPicture> = savedStateHandle.getStateFlow(PICTURE_KEY, AssetPicture.SMILE_GIRL)
    val currentPicture = _currentPicture

    private var _bitmapFlow = MutableStateFlow<Bitmap?>(null)
    val bitmapFlow = _bitmapFlow

    @MainThread
    fun setBitmap(bitmap: Bitmap) {
        bitmapFlow.value = bitmap
    }

    @MainThread
    fun showPrevPicture() {
        savedStateHandle[PICTURE_KEY] = currentPicture.value.prev()
    }

    @MainThread
    fun showNextPicture() {
        savedStateHandle[PICTURE_KEY] = currentPicture.value.next()
    }

    fun generatePicture() {
        if (bitmapFlow.value == null) return
        val mainThread = false

        if (mainThread) {
            bitmapFlow.value = jniBridge.greyImageRegular(bitmapFlow.value!!)
        } else {
            viewModelScope.launch(CoroutineName("BitmapGeneration")) {
                val bitmap = async {
                    jniBridge.greyImage(bitmapFlow.value!!)
                }
                bitmapFlow.value = bitmap.await()
            }
        }

    }


    companion object {
        val PICTURE_KEY = "picture_key"
    }

}