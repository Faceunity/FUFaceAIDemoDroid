//
// Created by Qinyu on 2020/8/17.
//
#include "include/fuai_interface.h"
#include "authpack.h"
#include <jni.h>
#include <android/log.h>
#include <vector>
#include <fstream>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_vLogSetLevel(JNIEnv *env, jclass cls, jint level){
    return FI_VLogSetLevel(level);
}

JNIEXPORT jstring JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_getVersion(JNIEnv *env, jclass clazz) {
    const char* version = nullptr;
    FI_Version(&version);
    return env->NewStringUTF(version);
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_setup(JNIEnv *env, jclass cls, jbyteArray auth) {
    jbyte *auth_ptr = env->GetByteArrayElements(auth, 0);
    jsize auth_size = env->GetArrayLength(auth);
    //    int result = fuSetup(NULL, 0, NULL, auth_ptr, auth_size);
    int authdata_size = sizeof(g_auth_package) / sizeof(g_auth_package[0]);
    int result = FI_Setup(g_auth_package, authdata_size);
    __android_log_print(ANDROID_LOG_ERROR, "STDOUT", "fuSetup use authpack.h, result:%d",result);
    env->ReleaseByteArrayElements(auth, auth_ptr, JNI_ABORT);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_destroy(JNIEnv *env, jclass cls){
    return FI_Destroy();
}

JNIEXPORT jlong JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_newImageView(JNIEnv *env, jclass clazz) {
    FI_ImageView *pImageView = nullptr;
    int result = FI_NewImageView(&pImageView);
    __android_log_print(ANDROID_LOG_ERROR, "STDOUT", "FI_NewImageView, result:%d", result);
    return (long) pImageView;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_deleteImageView(JNIEnv *env, jclass clazz,
                                                               jlong image_view) {
    FI_ImageView* pImageView = (FI_ImageView*) image_view;
    int result = FI_DeleteImageView(pImageView);
    __android_log_print(ANDROID_LOG_ERROR, "STDOUT", "FI_DeleteImageView, result:%d", result);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_imageViewInit(JNIEnv *env, jclass clazz,
                                                             jlong image_view, jint image_format,
                                                             jint width, jint height, jint rotation,
                                                             jbyteArray buffer) {
    FI_ImageView* pImageView = (FI_ImageView*) image_view;
    int length = env->GetArrayLength(buffer);
    uint8_t *y = (uint8_t*)((long)env->GetByteArrayElements(buffer, 0));
    uint8_t *uv = y + width * height;
    int result = FI_ImageViewInit(pImageView, (FI_ImageFormat) image_format, width, height, (FI_ImageRotation) rotation
            , y, width, uv, width, nullptr, 0);
    env->ReleaseByteArrayElements(buffer, (jbyte*) y, JNI_ABORT);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_imageViewSetData(JNIEnv *env, jclass clazz,
                                                                jlong image_view, jint index,
                                                                jbyteArray buffer) {
    // TODO: implement imageViewSetData()
    return -1;
}

JNIEXPORT jlong JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_newFaceProcessorFromBundle(JNIEnv *env, jclass clazz, jbyteArray bundle) {
    char *chars = (char*)(env->GetByteArrayElements(bundle, 0));
    int chars_len = env->GetArrayLength(bundle);
    // create face ai model
    FI_FaceProcessor *face_model;
    int result = FI_NewFaceProcessorFromBundle(chars, chars_len, &face_model);
    __android_log_print(ANDROID_LOG_ERROR, "STDOUT", "FI_NewFaceProcessorFromBundle, result:%d", result);
    env->ReleaseByteArrayElements(bundle, (jbyte*) chars, JNI_ABORT);
    return (long) face_model;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_deleteFaceProcessor(JNIEnv *env, jclass clazz,
                                                                   jlong model) {
    int result = FI_DeleteFaceProcessor((FI_FaceProcessor *) model);
    __android_log_print(ANDROID_LOG_ERROR, "STDOUT", "FI_DeleteFaceProcessor, result:%d", result);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_faceProcessorProcess(JNIEnv *env, jclass clazz,
                                                                    jlong model, jlong image_view) {
    int result = FI_FaceProcessorProcess((FI_FaceProcessor*) model, (FI_ImageView*) image_view);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_faceProcessorGetNumResults(JNIEnv *env, jclass clazz,
                                                                          jlong model) {
    int num = 0;
    FI_FaceProcessorGetNumResults((FI_FaceProcessor *) model, &num);
    return num;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_setUseFaceLandmark(JNIEnv *env, jclass clazz,
                                                                  jlong model,
                                                                  jint use_face_landmark) {
    int result = FI_FaceProcessorSetUseFaceLandmark((FI_FaceProcessor *) model, use_face_landmark);
    __android_log_print(ANDROID_LOG_ERROR, "STDOUT",
                        "FI_FaceProcessorSetUseFaceLandmark, result:%d", result);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_setUseFaceCapture(JNIEnv *env, jclass clazz,
                                                                 jlong model,
                                                                 jint use_face_capture) {
    int result = FI_FaceProcessorSetUseFaceCapture((FI_FaceProcessor *) model, use_face_capture);
    __android_log_print(ANDROID_LOG_ERROR, "STDOUT", "FI_FaceProcessorSetUseFaceCapture, result:%d",
                        result);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_setUseDenseLandmark(JNIEnv *env, jclass clazz,
                                                                   jlong model,
                                                                   jint use_dense_landmark) {
    int result = FI_FaceProcessorSetUseDenseLandmark((FI_FaceProcessor*) model, use_dense_landmark);
    __android_log_print(ANDROID_LOG_ERROR, "STDOUT", "FI_FaceProcessorSetUseDenseLandmark, result:%d", result);
    return result;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_getResultLandmarks(JNIEnv *env, jclass clazz,
                                                                  jlong model, jint index,
                                                                  jfloatArray result) {
    const float *landmarks = nullptr;
    int size = 0;
    int status = FI_FaceProcessorGetResultLandmarks((FI_FaceProcessor*) model, index, &landmarks, &size);
    env->SetFloatArrayRegion(result, 0, size, landmarks);
    return size;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_getResultAllLandmarks(JNIEnv *env, jclass clazz,
                                                                     jlong model, jint index,
                                                                     jfloatArray result) {
    const float *landmarks = nullptr;
    int size = 0;
    int status = FI_FaceProcessorGetResultAllLandmarks((FI_FaceProcessor*) model, index, &landmarks, &size);
    env->SetFloatArrayRegion(result, 0, size, landmarks);
    return size;
}

JNIEXPORT jint JNICALL
Java_com_faceunity_fualgorithmdemo_FuAlgorithm_FaceProcessorGetResultExpression(JNIEnv *env,jclass clazz,
                                                                                jlong model,
                                                                                jint index,
                                                                                jfloatArray result) {
    const float *express = nullptr;
    int size = 0;
    int status = FI_FaceProcessorGetResultExpression((FI_FaceProcessor*) model, index, &express, &size);
    env->SetFloatArrayRegion(result, 0, size, express);
    return size;
}

#ifdef __cplusplus
}
#endif