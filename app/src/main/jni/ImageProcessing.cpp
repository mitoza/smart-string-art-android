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

JavaVM *jvm = nullptr;
jweak WListener = nullptr;
StringArtGenerator gen;

extern "C" JNIEXPORT jstring JNICALL
Java_com_flycatcher_smartstring_JniBridge_showText
        (JNIEnv *env, jobject thiz, jstring text) {
    return text;
}


extern "C" JNIEXPORT void JNICALL
Java_com_flycatcher_smartstring_JniBridge_stringArtImage
        (JNIEnv *env, jobject thiz, jint width, jint height, jint nw, jint nh,
         jint sizeOfPins, jint minDistance, jint maxLines,
         jint lineWeight,
         jintArray pixelsIn, jintArray pixelsOut, jobject callback) {
    // Input
    jint *pPixelsIn = env->GetIntArrayElements(pixelsIn, 0);
    Mat src(height, width, CV_8UC4, (unsigned char *) pPixelsIn);

    // Output grey
    jint *pOutPixels = env->GetIntArrayElements(pixelsOut, 0);
    Mat result(nh, nw, CV_8UC4, (unsigned char *) pOutPixels);

    // Filter
    //gen = StringArtGenerator();
    env->GetJavaVM(&jvm);
    if (nullptr != callback && nullptr != jvm) {
        WListener = env->NewWeakGlobalRef(callback);
        gen.addCallback(sendProgress);
    }

    gen.setSize(nw, nh);
    gen.setSizeOfPins(sizeOfPins);
    gen.setMinDistance(minDistance);
    gen.setMaxLines(maxLines);
    gen.setLineWeight(lineWeight);

    gen.generateCircle(src).copyTo(result);
    //gen.removeCallback();
    //gen.release();

    // Release resources
    if (nullptr != pPixelsIn) {
        env->ReleaseIntArrayElements(pixelsIn, pPixelsIn, JNI_ABORT);
    }
    if (nullptr != pOutPixels) {
        env->ReleaseIntArrayElements(pixelsOut, pOutPixels, JNI_ABORT);
    }
    if (nullptr != WListener) {
        env->DeleteWeakGlobalRef(WListener);
    }
    if (nullptr != jvm) {
        jvm = nullptr;
    }
}

bool sendProgress(int progress) {
    if (nullptr == jvm) return false;
    // Get JNIEnv from JavaVM
    JNIEnv *env;
    int getEnvStat = jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (JNI_OK != getEnvStat || nullptr == WListener) return false;

    jclass clazz = env->GetObjectClass(WListener);
    jmethodID store_method = env->GetMethodID(clazz, "sendProgress", "(I)V");
    env->CallVoidMethod(WListener, store_method, progress);

    return true;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_flycatcher_smartstring_JniBridge_callbackJNI
        (JNIEnv *env, jobject thiz, jobject listener) {
    // Check JVM 
    JavaVM *_jvm = nullptr;
    env->GetJavaVM(&_jvm);
    if (nullptr == _jvm) return;

    jweak store_Wlistener = env->NewWeakGlobalRef(listener);
    jclass clazz = env->GetObjectClass(store_Wlistener);
    jmethodID store_method = env->GetMethodID(clazz, "sendProgress", "(ILjava/lang/String;)V");

    jstring msg;
    for (int i = 0; i < 10; i++) {
        if (i == 0) msg = env->NewStringUTF("Started");
        else if (i < 9) msg = env->NewStringUTF("Progress");
        else msg = env->NewStringUTF("Finished");
        env->CallVoidMethod(store_Wlistener, store_method, i, msg);
    }

}
