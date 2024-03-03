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
using namespace fc;

extern "C" JNIEXPORT jstring JNICALL
Java_com_flycatcher_smartstring_JniBridge_showText
        (JNIEnv *env, jobject thiz, jstring text) {
    return text;
}

extern "C" JNIEXPORT void JNICALL
Java_com_flycatcher_smartstring_JniBridge_greyImage
        (JNIEnv *env, jobject thiz, jint width, jint height,
         jint sizeOfPins, jint minDistance, jint maxLines,
         jint lineWeight,
         jintArray pixelsIn, jintArray pixelsOut) {
    // Input
    jint *pPixelsIn = env->GetIntArrayElements(pixelsIn, 0);
    Mat src(height, width, CV_8UC4, (unsigned char *) pPixelsIn);

    // Output grey
    jint *pOutPixels = env->GetIntArrayElements(pixelsOut, 0);
    Mat result(height, width, CV_8UC4, (unsigned char *) pOutPixels);

    // Filter
    StringArtGenerator gen = StringArtGenerator();
    gen.generateCircle(src, sizeOfPins, minDistance, maxLines, lineWeight).copyTo(result);


    // Release resources

    if (pPixelsIn != NULL) {
        env->ReleaseIntArrayElements(pixelsIn, pPixelsIn, JNI_ABORT);
    }
    if (pOutPixels != NULL) {
        env->ReleaseIntArrayElements(pixelsOut, pOutPixels, JNI_ABORT);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_flycatcher_smartstring_JniBridge_callbackJNI
        (JNIEnv *env, jobject thiz, jobject listener) {
    // Check JVM 
    JavaVM *jvm = nullptr;
    env->GetJavaVM(&jvm);
    if (nullptr == jvm) return;

    jweak store_Wlistener = env ->NewWeakGlobalRef(listener);
    jclass clazz = env->GetObjectClass(store_Wlistener);
    jmethodID store_method = env->GetMethodID(clazz, "sendProgress", "(ILjava/lang/String;)V");

    jstring msg;
    for(int i=0; i<10; i++) {
        if (i==0) msg = env->NewStringUTF("Started");
        else if (i<9) msg = env->NewStringUTF("Progress");
        else msg = env->NewStringUTF("Finished");
        env->CallVoidMethod(store_Wlistener, store_method, i, msg);
    }

}
