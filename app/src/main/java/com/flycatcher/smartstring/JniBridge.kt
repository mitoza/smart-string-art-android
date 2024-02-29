package com.flycatcher.smartstring

import android.graphics.Bitmap
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.withContext

class JniBridge() {

    external fun showText(text: String): String

    external fun callbackJNI(listener: JNIListener)

    private external fun greyImage(
        width: Int, height: Int,
        pins: Int, minDistance: Int, maxLines: Int,
        lineWeight: Int,
        pixelsIn: IntArray, pixelsOut: IntArray
    )

    suspend fun greyImage(
        bitmap: Bitmap, pins: Int, minDistance: Int, maxLines: Int, lineWeight: Int
    ): Bitmap = withContext(Dispatchers.IO) {
        val outPixels = IntArray(bitmap.width * bitmap.height)
        greyImage(
            bitmap.width, bitmap.height,
            pins, minDistance, maxLines, lineWeight,
            bitmap.toIntArray(), outPixels
        )
        val newBitmap =
            Bitmap.createBitmap(bitmap.width, bitmap.height, Bitmap.Config.ARGB_8888)
        newBitmap.setPixels(outPixels, 0, bitmap.width, 0, 0, bitmap.width, bitmap.height)
        newBitmap
    }

    init {
        System.loadLibrary("opencv_java4")
        System.loadLibrary("ImageProcessing");
    }

    interface JNIListener {
        fun sendProgress(progress: Int, msg: String)
    }

}