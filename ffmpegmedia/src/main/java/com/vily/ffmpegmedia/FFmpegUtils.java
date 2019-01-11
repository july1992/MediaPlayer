package com.vily.ffmpegmedia;

import android.text.TextUtils;

import com.vily.ffmpegmedia.listener.FFmpegCallBackListener;
import com.vily.ffmpegmedia.log.MyLog;

/**
 *  * description : 
 *  * Author : Vily
 *  * Date : 2019/1/10
 *  
 **/
public class FFmpegUtils {
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("postproc-54");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
    }

    private TimeInfoBean mTimeInfoBean;

    public FFmpegUtils() {
    }

    private String source;//数据源



    public void setSource(String source) {
        this.source=source;
    }


    public void prepare() {
        if(TextUtils.isEmpty(source)){

            MyLog.d("-----资源文件地址不能为空");
            return;

        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_parpared(source);
            }
        }).start();
    }
    public void start() {
        if(TextUtils.isEmpty(source))
        {
            MyLog.d("资源文件为空");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                n_start();
            }
        }).start();
    }


    // 定义回调接口
    private FFmpegCallBackListener mFFmpegCallBackListener;

    public void setFFmpegCallBackListener(FFmpegCallBackListener FFmpegCallBackListener) {
        mFFmpegCallBackListener = FFmpegCallBackListener;
    }

    // C++ 回调方法
    public void onCallPrepared(){
        if(mFFmpegCallBackListener!=null){
            mFFmpegCallBackListener.onPrepared();
        }
    }

    public void onCallLoad(boolean load){
        if(mFFmpegCallBackListener!=null){
            mFFmpegCallBackListener.onLoad(load);
        }
    }

    public void onCallByte(byte[] bytes){
        if(mFFmpegCallBackListener!=null){
            mFFmpegCallBackListener.onCallByte(bytes);
        }
    }

    public void onCallTimeInfo(int currentTime, int totalTime){
        if(mFFmpegCallBackListener!=null){
            if(mTimeInfoBean == null)
            {
                mTimeInfoBean = new TimeInfoBean();
            }
            mTimeInfoBean.setCurrentTime(currentTime);
            mTimeInfoBean.setTotalTime(totalTime);
            mFFmpegCallBackListener.onTimeInfo(mTimeInfoBean);
        }
    }

    public void onCallAudioData(int length){
        if(mFFmpegCallBackListener!=null){
            mFFmpegCallBackListener.onAudioData(length);
        }
    }

    public native void n_parpared(String source);

    private native void n_start();

    private native void n_pause();

    private native void n_resume();


    public void pause() {
        n_pause();
        if(mFFmpegCallBackListener != null)
        {
            mFFmpegCallBackListener.onPause(true);
        }
    }

    public void resume() {
        n_resume();
        if(mFFmpegCallBackListener != null)
        {
            mFFmpegCallBackListener.onPause(false);
        }
    }
}
