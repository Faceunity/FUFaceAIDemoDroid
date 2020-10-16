package com.faceunity.fualgorithmdemo;

import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.faceunity.fualgorithmdemo.gles.core.GlUtil;
import com.faceunity.fualgorithmdemo.renderer.BaseCameraRenderer;
import com.faceunity.fualgorithmdemo.renderer.Camera1Renderer;
import com.faceunity.fualgorithmdemo.renderer.OnRendererStatusListener;
import com.faceunity.fualgorithmdemo.utils.PermissionUtil;

import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

public class PreviewActivity extends AppCompatActivity implements OnRendererStatusListener, SensorEventListener {
    private static final String TAG = "PreviewActivity";
    public static final int LANDMARK = 1;
    public static final int LANDMARK_DENSE = 1<<1;
    public static final int EXPRESSION = 1<<2;

    private DrawerLayout drawerLayout;
    private Button btnSet;
    private GLSurfaceView glSurfaceView;
    private RecyclerView rvExpressions;
    private RvExpressionAdapter expressionAdapter;
    private BaseCameraRenderer mCameraRenderer;
    private int type;

    private SensorManager mSensorManager;
    private Sensor mSensor;
    private int mRotateMode = FuAlgorithm.FI_IMAGE_ROTATION_90;
    private long pImageView;
    private long pFaceProcessor;
    private float[] mLandmarksDataArray = new float[239 * 2];
    private float[] mExpressionData = new float[46];

    public static void start(Context context, int type) {
        Intent intent = new Intent(context, PreviewActivity.class);
        intent.putExtra("type", type);
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_preview);

        PermissionUtil.checkPermission(this);
        type = getIntent().getIntExtra("type", 0);
        initView();

        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
    }

    private void initView() {
        drawerLayout = findViewById(R.id.dl_container);
        btnSet = findViewById(R.id.btn_settings);
        glSurfaceView = findViewById(R.id.glSurfaceView);
        rvExpressions = findViewById(R.id.rv_expressions);
        CheckBox cbLandMark = findViewById(R.id.cb_landmark);
        CheckBox cbDense = findViewById(R.id.cb_landmark_dense);
        CheckBox cbExpress = findViewById(R.id.cb_expression);

        drawerLayout.setScrimColor(0);
        //fix some device cannot display whole DrawerLayout
        drawerLayout.addDrawerListener(new DrawerLayout.SimpleDrawerListener() {
            @Override
            public void onDrawerSlide(View drawerView, float slideOffset) {
                drawerLayout.bringChildToFront(drawerView);
                drawerLayout.requestLayout();
            }
        });
        btnSet.setOnClickListener(v -> drawerLayout.openDrawer(GravityCompat.START));

        glSurfaceView.setEGLContextClientVersion(GlUtil.getSupportGLVersion(this));
        mCameraRenderer = new Camera1Renderer(this, glSurfaceView, this);
        glSurfaceView.setRenderer(mCameraRenderer);
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        rvExpressions.setLayoutManager(new LinearLayoutManager(this));
        expressionAdapter = new RvExpressionAdapter();
        rvExpressions.setAdapter(expressionAdapter);

        cbLandMark.setOnCheckedChangeListener((buttonView, isChecked) -> type = type ^ LANDMARK ^ LANDMARK_DENSE);
        cbDense.setOnCheckedChangeListener((buttonView, isChecked) -> type ^= LANDMARK_DENSE);
        cbExpress.setOnCheckedChangeListener((buttonView, isChecked) -> type ^= EXPRESSION);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mCameraRenderer.onResume();
        mSensorManager.registerListener(this, mSensor, SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mCameraRenderer.onPause();
        mSensorManager.unregisterListener(this);
    }

    private void showFaceInfo() {
        int trackedFaceCount = FuAlgorithm.faceProcessorGetNumResults(pFaceProcessor);
        Arrays.fill(mLandmarksDataArray, 0);
        Arrays.fill(mExpressionData, 0);
        if (trackedFaceCount > 0) {
            if ((type|EXPRESSION) == type) {
                FuAlgorithm.FaceProcessorGetResultExpression(pFaceProcessor,0, mExpressionData);
                runOnUiThread(() -> {
                    rvExpressions.setVisibility(View.VISIBLE);
                    expressionAdapter.notifyDataSetChangedSkip(12);
                });
            } else {
                runOnUiThread(() -> rvExpressions.setVisibility(View.GONE));
            }
            if ((type|LANDMARK) == type && (type|LANDMARK_DENSE) == type) {
                FuAlgorithm.getResultAllLandmarks(pFaceProcessor, 0, mLandmarksDataArray);
            } else if ((type|LANDMARK) == type) {
                FuAlgorithm.getResultLandmarks(pFaceProcessor, 0, mLandmarksDataArray);
            } else if ((type|LANDMARK_DENSE) == type) {
                FuAlgorithm.getResultAllLandmarks(pFaceProcessor, 0, mLandmarksDataArray);
                Arrays.fill(mLandmarksDataArray, 0, 149, 0);
            }
        }
        mCameraRenderer.setLandmarksDataArray(mLandmarksDataArray);
    }

    @Override
    public void onSurfaceCreated() {
        pImageView = FuAlgorithm.newImageView();
        AssetManager assetManager = getAssets();
        try {
            InputStream is = assetManager.open("ai_face_processor.bundle");
            byte[] bundle = new byte[is.available()];
            is.read(bundle);
            is.close();
            pFaceProcessor = FuAlgorithm.newFaceProcessorFromBundle(bundle);
        } catch (IOException e) {
            e.printStackTrace();
            finish();
        }
        FuAlgorithm.setUseFaceLandmark(pFaceProcessor, 1);
        FuAlgorithm.setUseDenseLandmark(pFaceProcessor, 1);
        FuAlgorithm.setUseFaceCapture(pFaceProcessor, 1);
    }

    @Override
    public void onSurfaceChanged(int viewWidth, int viewHeight) {

    }

    @Override
    public int onDrawFrame(byte[] cameraNv21Byte, int cameraTexId, int cameraWidth, int cameraHeight, float[] mvpMatrix, float[] texMatrix, long timeStamp) {
        FuAlgorithm.imageViewInit(pImageView, FuAlgorithm.FI_IMAGE_YUV_NV21, cameraWidth, cameraHeight, mRotateMode, cameraNv21Byte);
        FuAlgorithm.faceProcessorProcess(pFaceProcessor, pImageView);
        showFaceInfo();
        return cameraTexId;
    }

    @Override
    public void onSurfaceDestroy() {
        FuAlgorithm.deleteImageView(pImageView);
        FuAlgorithm.deleteFaceProcessor(pFaceProcessor);
    }

    @Override
    public void onCameraChanged(int cameraFacing, int cameraOrientation) {

    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            float x = event.values[0];
            float y = event.values[1];
            if (Math.abs(x) > 3 || Math.abs(y) > 3) {
                if (Math.abs(x) > Math.abs(y)) {
                    mRotateMode = x > 0 ? FuAlgorithm.FI_IMAGE_ROTATION_0 : FuAlgorithm.FI_IMAGE_ROTATION_180;
                } else {
                    mRotateMode = y > 0 ? FuAlgorithm.FI_IMAGE_ROTATION_90 : FuAlgorithm.FI_IMAGE_ROTATION_270;
                }
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {}

    private class RvExpressionAdapter extends RecyclerView.Adapter<RvExpressionAdapter.ViewHolder> {
        private int current;

        @NonNull
        @Override
        public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
            View view = LayoutInflater.from(PreviewActivity.this).inflate(R.layout.item_expression, parent, false);
            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
            holder.tvId.setText(String.valueOf(position));
            holder.tvNum.setText(String.format("%.4f", mExpressionData[position]));
        }

        @Override
        public int getItemCount() {
            return 46;
        }

        public void notifyDataSetChangedSkip(int skip) {
            if (current == skip) {
                current = 0;
                notifyDataSetChanged();
            } else {
                current++;
            }
        }

        private class ViewHolder extends RecyclerView.ViewHolder{
            TextView tvId, tvNum;
            public ViewHolder(@NonNull View itemView) {
                super(itemView);
                tvId = itemView.findViewById(R.id.tv_expression_id);
                tvNum = itemView.findViewById(R.id.tv_expression_num);
            }
        }
    }
}
