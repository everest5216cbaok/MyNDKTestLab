package com.example.myndktest;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.widget.Toast;

public class MainActivity extends ActionBarActivity {

    private GLSurfaceView glSurfaceView;
    private boolean rendererSet;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
     
        ActivityManager activityManager
            = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
     
        final boolean supportsEs2 =
            configurationInfo.reqGlEsVersion >= 0x20000 || isProbablyEmulator();
     
        if (supportsEs2) {
            glSurfaceView = new GLSurfaceView(this);
     
            if (isProbablyEmulator()) {
                // Avoids crashes on startup with some emulator images.
                glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
            }
     
            glSurfaceView.setEGLContextClientVersion(2);
            glSurfaceView.setRenderer(new RendererWrapper());
            rendererSet = true;
            setContentView(glSurfaceView);
        } else {
            // Should never be seen in production, since the manifest filters
            // unsupported devices.
            Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
                    Toast.LENGTH_LONG).show();
            return;
        }
        
        float origin_path[][] = {{0.00f, 0.00f}, {1.00f, 0.00f}, {0.00f, 1.00f}, {1.00f, 1.00f},
                {0.21f, 0.51f}, {0.39f, 0.52f}, {0.25f, 0.75f}, {0.39f, 0.76f}, {0.29f, 0.68f}};
        float target_path[][] = {{0.00f, 0.00f},
                {1.00f, 0.00f},
                {0.00f, 1.00f},
                {1.00f, 1.00f},
                {0.37f, 0.39f},
                {0.59f, 0.40f},
                {0.38f, 0.54f},
                {0.56f, 0.54f},
                {0.47f, 0.49f}};
        
        GameLibJNIWrapper.key_points_with_corners(origin_path, target_path, "/sdcard/keyPoints/image-fbb.png", "/sdcard/keyPoints/image-lh.png");
    }
    
    private boolean isProbablyEmulator() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
                && (Build.FINGERPRINT.startsWith("generic")
                        || Build.FINGERPRINT.startsWith("unknown")
                        || Build.MODEL.contains("google_sdk")
                        || Build.MODEL.contains("Emulator")
                        || Build.MODEL.contains("Android SDK built for x86"));
    }
    
    @Override
    protected void onPause() {
        super.onPause();
     
        if (rendererSet) {
            glSurfaceView.onPause();
        }
    }
     
    @Override
    protected void onResume() {
        super.onResume();
     
        if (rendererSet) {
            glSurfaceView.onResume();
        }
    }
}
