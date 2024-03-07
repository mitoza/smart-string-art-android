package com.flycatcher.smartstring

import android.graphics.Bitmap
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.withContext

class JniBridge() {

    external fun showText(text: String): String

    external fun callbackJNI(listener: JNIListener)

    private external fun stringArtImage(
        width: Int, height: Int, nw: Int, nh: Int,
        pins: Int, minDistance: Int, maxLines: Int,
        lineWeight: Int,
        pixelsIn: IntArray, pixelsOut: IntArray, listener: StringArtProgress
    )

    suspend fun stringArtImage(
        bitmap: Bitmap, width: Int, height: Int, pins: Int, minDistance: Int, maxLines: Int, lineWeight: Int, callback: StringArtProgress
    ): Bitmap = withContext(Dispatchers.IO) {
        val outPixels = IntArray(width * height)
        stringArtImage(
            bitmap.width, bitmap.height, width, height,
            pins, minDistance, maxLines, lineWeight,
            bitmap.toIntArray(), outPixels, callback
        )

        val newBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        newBitmap.setPixels(outPixels, 0, width, 0, 0, width, height)

        newBitmap
    }

    init {
        System.loadLibrary("opencv_java4")
        System.loadLibrary("ImageProcessing");
    }

    interface JNIListener {
        fun sendProgress(progress: Int, msg: String)
    }

    interface StringArtProgress {
        fun sendProgress(progress: Int)
    }

}