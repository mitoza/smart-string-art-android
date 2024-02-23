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

using namespace std;
using namespace cv;

//#include <jni.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_flycatcher_smartstring_JniBridge_showText
(JNIEnv *env, jobject thiz, jstring text) {
    return text;
}

extern "C" JNIEXPORT void JNICALL
Java_com_flycatcher_smartstring_JniBridge_greyImage
(JNIEnv *env, jobject thiz, jint width, jint height, jintArray pixelsIn, jintArray pixelsOut) {
    // Input
    jint *pPixelsIn = env->GetIntArrayElements(pixelsIn, 0);
    Mat src(height, width, CV_8UC4, (unsigned char *) pPixelsIn);

    // Output grey
    jint *pOutPixels = env->GetIntArrayElements(pixelsOut, 0);
    Mat result(height, width, CV_8UC4, (unsigned char *) pOutPixels);

    // Filter
    Mat grey;
    cvtColor(src, grey, COLOR_RGB2GRAY);
    cvtColor(grey, result, COLOR_GRAY2RGBA);


    // Release resources
    if (!grey.empty()) grey.release();

    if (pPixelsIn != NULL) {
        env->ReleaseIntArrayElements(pixelsIn, pPixelsIn, JNI_ABORT);
    }
    if (pOutPixels != NULL) {
        env->ReleaseIntArrayElements(pixelsOut, pOutPixels, JNI_ABORT);
    }
}
