package com.flycatcher.smartstring

import android.view.View
import androidx.lifecycle.LiveData
import androidx.lifecycle.SavedStateHandle
import androidx.lifecycle.ViewModel

class MainActivityViewModel(
    private val state: SavedStateHandle
): ViewModel() {

    val currentPicture: LiveData<AssetPicture> = state.getLiveData(PICTURE_KEY, AssetPicture.SMILE_GIRL)

    fun setPrevPicture(v: View) {
        state[PICTURE_KEY] = currentPicture.value?.prev()
    }

    fun setNextPicture(v: View) {
        state[PICTURE_KEY] = currentPicture.value?.next()
    }

    companion object {
        val PICTURE_KEY = "picture_key"
    }

}