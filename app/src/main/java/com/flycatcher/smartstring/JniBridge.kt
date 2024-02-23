package com.flycatcher.smartstring

import android.graphics.Bitmap

class JniBridge() {

    external fun showText(text: String): String

    private external fun greyImage(
        width: Int, height: Int, pixelsIn: IntArray,
        pixelsOut: IntArray
        )

    fun greyImage(bitmap: Bitmap): Bitmap {
        val outPixels = IntArray(bitmap.width * bitmap.height)
        greyImage(bitmap.width, bitmap.height, bitmap.toIntArray(), outPixels)
        val newBitmap = Bitmap.createBitmap(bitmap.width, bitmap.height, Bitmap.Config.ARGB_8888)
        newBitmap.setPixels(outPixels, 0, bitmap.width, 0, 0, bitmap.width, bitmap.height)
        return newBitmap
    }

    init {
        System.loadLibrary("opencv_java4")
        System.loadLibrary("ImageProcessing");
    }

}