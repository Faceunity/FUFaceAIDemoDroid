// Copyright (C) 2019 FaceUnity Inc. All rights reserved.

#ifndef FUAI_INTERFACE_FUAI_INTERFACE_H_
#define FUAI_INTERFACE_FUAI_INTERFACE_H_

#include <stdint.h>

#if defined(_WIN32)
#ifdef FUAI_BUILD_SHARED_LIB
#define FI_EXPORT __declspec(dllexport)
#else
#define FI_EXPORT
#endif
#else
#define FI_EXPORT __attribute__((visibility("default")))
#endif  // _WIN32

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FI_Status {
  FI_STATUS_OK = 0,
  FI_STATUS_INVALID_ARGUMENT = 1,
  FI_STATUS_AUTH_FAILED = 2,
  FI_STATUS_UNKNOWN_ERROR = -1
} FI_Status;

FI_EXPORT extern FI_Status FI_Setup(const char* authpack, int size);
FI_EXPORT extern FI_Status FI_Destroy();
FI_EXPORT extern FI_Status FI_VLogSetLevel(int level);
FI_EXPORT extern FI_Status FI_Version(const char** version);

// --------------------------------------------------------------------------
// ImageView

typedef enum FI_ImageFormat {
  FI_IMAGE_UNKNOWN = 0,
  FI_IMAGE_RGB = 1,
  FI_IMAGE_BGR = 2,
  FI_IMAGE_RGBA = 3,
  FI_IMAGE_BGRA = 4,
  FI_IMAGE_ARGB = 5,
  FI_IMAGE_ABGR = 6,
  FI_IMAGE_GRAY = 7,
  FI_IMAGE_YUV_NV12 = 8,
  FI_IMAGE_YUV_NV21 = 9,
  FI_IMAGE_YUV_I420 = 10,
} FI_ImageFormat;

typedef enum FI_ImageRotation {
  FI_IMAGE_ROTATION_0 = 0,
  FI_IMAGE_ROTATION_90 = 1,
  FI_IMAGE_ROTATION_180 = 2,
  FI_IMAGE_ROTATION_270 = 3,
} FI_ImageRotation;

typedef struct FI_ImageView FI_ImageView;

FI_EXPORT extern FI_Status FI_NewImageView(FI_ImageView** image_view);

FI_EXPORT extern FI_Status FI_DeleteImageView(FI_ImageView* image_view);

FI_EXPORT extern FI_Status FI_ImageViewInit(
    FI_ImageView* image_view, FI_ImageFormat format, int width, int height,
    FI_ImageRotation rotation, const uint8_t* data0, int stride0,
    const uint8_t* data1, int stride1, const uint8_t* data2, int stride2);

FI_EXPORT extern FI_Status FI_ImageViewSetData(FI_ImageView* image_view,
                                               int index, const uint8_t* data);

// --------------------------------------------------------------------------
// FaceProcessor
typedef struct FI_FaceProcessor FI_FaceProcessor;

typedef enum FI_FaceDetectMode {
  FI_FACE_DETECT_MODE_IMAGE = 0,
  FI_FACE_DETECT_MODE_VIDEO = 1
} FI_FaceDetectMode;

FI_EXPORT extern FI_Status FI_NewFaceProcessorFromBundle(
    const char* data, int size, FI_FaceProcessor** model);

FI_EXPORT extern FI_Status FI_DeleteFaceProcessor(FI_FaceProcessor* model);

FI_EXPORT extern FI_Status FI_FaceProcessorProcess(
    FI_FaceProcessor* model, const FI_ImageView* image_view);

FI_EXPORT extern FI_Status FI_FaceProcessorReset(FI_FaceProcessor* model);

FI_EXPORT extern FI_Status FI_FaceProcessorSetDetectMode(
    FI_FaceProcessor* model, FI_FaceDetectMode detect_mode);

FI_EXPORT extern FI_Status FI_FaceProcessorSetMaxFaces(FI_FaceProcessor* model,
                                                       int max_faces);

FI_EXPORT extern FI_Status FI_FaceProcessorResetModules(
    FI_FaceProcessor* model);

FI_EXPORT extern FI_Status FI_FaceProcessorSetUseFaceLandmark(
    FI_FaceProcessor* model, int use_face_landmark);

FI_EXPORT extern FI_Status FI_FaceProcessorSetUseDenseLandmark(
    FI_FaceProcessor* model, int use_dense_landmark);

FI_EXPORT extern FI_Status FI_FaceProcessorSetUseFaceDde(
    FI_FaceProcessor* model, int use_face_dde);

FI_EXPORT extern FI_Status FI_FaceProcessorSetUseRotationTranslationFilter(
    FI_FaceProcessor* model, int use_filter);

FI_EXPORT extern FI_Status FI_FaceProcessorSetUseFaceCapture(
    FI_FaceProcessor* model, int use_face_capture);

FI_EXPORT extern FI_Status FI_FaceProcessorSetFaceCaptureUseTongueExpression(
    FI_FaceProcessor* model, int face_capture_use_tongue);

FI_EXPORT extern FI_Status FI_FaceProcessorSetMinFaceRatio(
    FI_FaceProcessor* model, float min_face_ratio);

FI_EXPORT extern FI_Status FI_FaceProcessorGetFov(const FI_FaceProcessor* model,
                                                  float* result);

FI_EXPORT extern FI_Status FI_FaceProcessorSetFov(FI_FaceProcessor* model,
                                                  float fov_degree);

FI_EXPORT extern FI_Status FI_FaceProcessorGetNumResults(
    const FI_FaceProcessor* model, int* result);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultRect(
    const FI_FaceProcessor* model, int index, const float** result);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultTrackId(
    const FI_FaceProcessor* model, int index, int* result);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultConfidenceScore(
    const FI_FaceProcessor* model, int index, float* result);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultLandmarks(
    const FI_FaceProcessor* model, int index, const float** result,
    int* result_size);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultAllLandmarks(
    const FI_FaceProcessor* model, int index, const float** result,
    int* result_size);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultExpression(
    const FI_FaceProcessor* model, int index, const float** result,
    int* result_size);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultTongueExpression(
    const FI_FaceProcessor* model, int index, const float** result,
    int* result_size);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultRotation(
    const FI_FaceProcessor* model, int index, const float** result,
    int* result_size);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultTranslation(
    const FI_FaceProcessor* model, int index, const float** result,
    int* result_size);

FI_EXPORT extern FI_Status FI_FaceProcessorGetResultEyesRotation(
    const FI_FaceProcessor* model, int index, const float** result,
    int* result_size);
#ifdef __cplusplus
}
#endif

#endif  // FUAI_INTERFACE_FUAI_INTERFACE_H_
