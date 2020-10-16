package com.faceunity.fualgorithmdemo;

import android.app.Application;
import android.util.Log;

public class FuApp extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        FuAlgorithm.setup(authpack.A());
        FuAlgorithm.vLogSetLevel(4);
        String version = FuAlgorithm.getVersion();
        Log.e("STDOUT", "version: " + version);
    }
}
