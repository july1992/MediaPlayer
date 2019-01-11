package com.vily.ffmpegmedia.log;

import android.util.Log;

/**
 *  * description : 
 *  * Author : Vily
 *  * Date : 2019/1/10
 *  
 **/
public class MyLog {

    private static final String TAG = "MyLog";
    public static void d(String msg)
    {
        Log.d(TAG, msg);
    }
}
