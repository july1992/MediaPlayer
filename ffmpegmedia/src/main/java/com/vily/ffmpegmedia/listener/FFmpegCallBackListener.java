package com.vily.ffmpegmedia.listener;

import com.vily.ffmpegmedia.TimeInfoBean;

/**
 *  * description : 
 *  * Author : Vily
 *  * Date : 2019/1/10
 *  
 **/
public interface FFmpegCallBackListener {
    void onPrepared();

    void onLoad(boolean load);

    void onCallByte(byte[] bytes);

    void onTimeInfo(TimeInfoBean timeInfoBean);

    void onAudioData(int length);

    void onPause(boolean pause);

    void onComplete();
}
