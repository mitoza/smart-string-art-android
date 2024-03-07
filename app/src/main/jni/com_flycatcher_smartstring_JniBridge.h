/* Header for class com_flycatcher_smartsketcher_util_JniBridge */
#include <jni.h>
#include "OpenCV-4.9.0-android-sdk/sdk/native/jni/include/opencv2/core/core.hpp"
#include "OpenCV-4.9.0-android-sdk/sdk/native/jni/include/opencv2/imgproc/imgproc.hpp"

//#include <OpenCV-4.9.0-android-sdk/sdk/native/jni/include/opencv2/highgui/highgui.hpp>
//#include <OpenCV-4.9.0-android-sdk/sdk/native/jni/include/opencv2/imgcodecs/imgcodecs.hpp>
//#include <OpenCV-4.9.0-android-sdk/sdk/native/jni/include/opencv2/imgproc/types_c.h>

#ifndef _Included_com_flycatcher_smartstring_util_JniBridge
#define _Included_com_flycatcher_smartstring_util_JniBridge
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     Java_com_flycatcher_smartstring_JniBridge
 * Method:    showText
 */
JNIEXPORT jstring JNICALL Java_com_flycatcher_smartstring_JniBridge_showText
        (JNIEnv *, jobject thiz, jstring text);

/*
 * Class:     Java_com_flycatcher_smartstring_JniBridge
 * Method:    greyImage
 */
JNIEXPORT void JNICALL Java_com_flycatcher_smartstring_JniBridge_stringArtImage
        (JNIEnv *, jobject, jint, jint, jint, jint,
         jint, jint, jint, jint,
         jintArray, jintArray, jobject);

/*
 * Class:     Java_com_flycatcher_smartstring_JniBridge
 * Method:    callbackJNI
 */
JNIEXPORT void JNICALL Java_com_flycatcher_smartstring_JniBridge_callbackJNI
        (JNIEnv *, jobject, jobject);


bool sendProgress(int progress);

#ifdef __cplusplus
}
#endif
#endif
