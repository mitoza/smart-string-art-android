package com.flycatcher.smartstring

class JniBridge() {

    external fun showText(text: String): String

    init {
        System.loadLibrary("opencv_java4")
        System.loadLibrary("SmartStringGenerator");
    }

}