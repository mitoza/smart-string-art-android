package com.flycatcher.smartstring

import android.graphics.Bitmap
import java.io.ByteArrayOutputStream

fun Bitmap.toIntArray(): IntArray {
    val argb = IntArray(width * height)
    getPixels(argb, 0, width, 0,0,width, height)
    return argb
}