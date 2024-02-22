//
//  StringArtGenerator.cpp
//  smArt StringArt
//
//  Created by Dmitry on 21/02/24.
//  Copyright © 2024 FlyCatcher. All rights reserved.
//

#include "com_flycatcher_smartstring_JniBridge.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/types_c.h>
//#include <android/bitmap.h>
//#include <android/log.h>

using namespace std;
using namespace cv;

//#include <jni.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_flycatcher_smartstring_JniBridge_showText
(JNIEnv *env, jobject thiz, jstring text) {

    return text;
}
