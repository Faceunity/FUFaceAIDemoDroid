package com.faceunity.fualgorithmdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(new String[] {Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE
                    , Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO}, 0);
        }

        findViewById(R.id.btn_landmark).setOnClickListener(v ->
                PreviewActivity.start(MainActivity.this, PreviewActivity.LANDMARK));
        findViewById(R.id.btn_dense_landmark).setOnClickListener(v ->
                PreviewActivity.start(MainActivity.this, PreviewActivity.LANDMARK_DENSE));
        findViewById(R.id.btn_expression).setOnClickListener(v ->
                PreviewActivity.start(MainActivity.this, PreviewActivity.EXPRESSION));
    }
}
