package com.flycatcher.smartstring

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.view.View
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.SavedStateHandle
import androidx.lifecycle.ViewModel
import java.io.IOException
import java.io.InputStream

class MainActivityViewModel(
    private val state: SavedStateHandle
): ViewModel() {

    private val jniBridge = JniBridge()

    private val currentPicture: LiveData<AssetPicture> = state.getLiveData(PICTURE_KEY, AssetPicture.SMILE_GIRL)
    private val currentBitmap = MutableLiveData<Bitmap>()

    fun getCurrentPictureLiveData(): LiveData<AssetPicture> = currentPicture

    fun getCurrentBitmapLiveData(): LiveData<Bitmap> = currentBitmap

    fun setBitmap(bitmap: Bitmap) {
        currentBitmap.value = bitmap
    }

    fun setPrevPicture(v: View) {
        state[PICTURE_KEY] = currentPicture.value?.prev()
    }

    fun setNextPicture(v: View) {
        state[PICTURE_KEY] = currentPicture.value?.next()
    }

    fun generatePicture(v: View) {
        if (currentBitmap.value == null) return
        setBitmap(jniBridge.greyImage(currentBitmap.value!!))
    }


    companion object {
        val PICTURE_KEY = "picture_key"
    }

}