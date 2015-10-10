package com.example.myndktest;

public class GameLibJNIWrapper {
    static {
        System.loadLibrary("game");
    }
 
    public static native void on_surface_created();
 
    public static native void on_surface_changed(int width, int height);
 
    public static native void on_draw_frame();
    
    public static native void key_points_with_corners(float origin_points[][], float target_points[][], String origin_pic_path, String target_pic_path);
}