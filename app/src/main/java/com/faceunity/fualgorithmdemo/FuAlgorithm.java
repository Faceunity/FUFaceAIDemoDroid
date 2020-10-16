package com.faceunity.fualgorithmdemo;

public class FuAlgorithm {
    static {
        System.loadLibrary("fuai-lib");
    }

    //FI_Status
    public static final int FI_STATUS_OK = 0;                 // 表示接口正常返回
    public static final int FI_STATUS_INVALID_ARGUMENT = 1;   // 表示接口输入参数有误
    public static final int FI_STATUS_AUTH_FAILED = 2;        // 表示鉴权失败，请确认FI_Setup正常调用，否则需要检查鉴权证书的有效性
    public static final int FI_STATUS_UNKNOWN_ERROR = -1;     // 未定义错误
    //FI_ImageFormat
    public static final int FI_IMAGE_UNKNOWN = 0;     // 未知格式，不能作为图像格式传入
    public static final int FI_IMAGE_RGB = 1;         // RGB格式，每个像素3个字节[r,g,b]
    public static final int FI_IMAGE_BGR = 2;         // BGR格式，每个像素3个字节[b,g,r]
    public static final int FI_IMAGE_RGBA = 3;        // RGBA格式，每个像素4个字节[r,g,b,a]
    public static final int FI_IMAGE_BGRA = 4;        // BGRA格式，每个像素4个字节[b,g,r,a]
    public static final int FI_IMAGE_ARGB = 5;        // ARGB格式，每个像素4个字节[a,r,g,b]
    public static final int FI_IMAGE_ABGR = 6;        // ABGR格式，每个像素4个字节[a,b,g,r]
    public static final int FI_IMAGE_GRAY = 7;        // 灰度图格式，每个像素1个字节[gray]
    public static final int FI_IMAGE_YUV_NV12 = 8;    // NV12格式，首先所有像素的Y通道值依次排列，然后按2倍下采样[U,V]通道顺序间隔排列
    public static final int FI_IMAGE_YUV_NV21 = 9;    // NV12格式，首先所有像素的Y通道值依次排列，然后按2倍下采样[V,U]通道顺序间隔排列
    public static final int FI_IMAGE_YUV_I420 = 10;   // I420格式，首先所有像素的Y通道依次排列，然后U通道按2倍下采样依次排列，最后V通道按2倍下采样依次排列
    //FI_ImageRotation
    public static final int FI_IMAGE_ROTATION_0 = 0;      // 图像旋转0度
    public static final int FI_IMAGE_ROTATION_90 = 1;     // 图像旋转90度
    public static final int FI_IMAGE_ROTATION_180 = 2;    // 图像旋转180度
    public static final int FI_IMAGE_ROTATION_270 = 3;    // 图像旋转270度

    public static native int setup(byte[] authpack);

    public static native int vLogSetLevel(int level);

    public static native String getVersion();

    public static native int destroy();

    /**
     * @return pointer of imageView
     */
    public static native long newImageView();

    public static native int deleteImageView(long image_view);

    public static native int imageViewInit(long image_view, int imageFormat, int width, int height, int rotation, byte[] buffer);

//    public static native int imageViewSetData(long image_view, int index, byte[] buffer);

    /**
     * @return pointer of faceProcessor
     */
    public static native long newFaceProcessorFromBundle(byte[] bundle);

    public static native int deleteFaceProcessor(long model);

    public static native int faceProcessorProcess(long model, long image_view);

    /**
     * @return 人脸个数
     */
    public static native int faceProcessorGetNumResults(long model);

    /**
     * @param use_face_landmark 是否启用人脸基础特征点算法，默认为1
     */
    public static native int setUseFaceLandmark(long model, int use_face_landmark);

    /**
     * @param use_face_capture 是否启用人脸表情系数算法
     */
    public static native int setUseFaceCapture(long model, int use_face_capture);

    /**
     * @param use_dense_landmark 是否启用人脸密集特征点算法，默认为0
     */
    public static native int setUseDenseLandmark(long model, int use_dense_landmark);

    /**
     * @return size of array length
     */
    public static native int getResultLandmarks(long model, int index, float[] result);

    public static native int getResultAllLandmarks(long model, int index, float[] result);

    public static native int FaceProcessorGetResultExpression(long model, int index, float[] result);
}
