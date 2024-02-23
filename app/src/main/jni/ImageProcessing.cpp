//
//  StringArtGenerator.cpp
//  smArt StringArt
//
//  Created by Dmitry on 21/02/24.
//  Copyright © 2024 Flycatcher Corporation Ltd. All rights reserved.
//

#include "com_flycatcher_smartstring_JniBridge.h"
#include "StringArtGenerator.h"

using namespace std;
using namespace cv;

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
    FlyCatcher::StringArtGenerator generator = FlyCatcher::StringArtGenerator();
    generator.generateCircle(src, 288).copyTo(result);

    // Release resources

    if (pPixelsIn != NULL) {
        env->ReleaseIntArrayElements(pixelsIn, pPixelsIn, JNI_ABORT);
    }
    if (pOutPixels != NULL) {
        env->ReleaseIntArrayElements(pixelsOut, pOutPixels, JNI_ABORT);
    }
}
