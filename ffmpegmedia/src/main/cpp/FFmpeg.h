/**
 *  * description :  这个好像是用来保存所有相关对象的
 *  * Author : Vily
 *  * Date : 2019/1/10
 *  
 **/


#ifndef MEDIAPLAYER_FFMPEG_H
#define MEDIAPLAYER_FFMPEG_H


#include <sys/types.h>
#include "FFmpegCallBack.h"
#include "PlayStatus.h"
#include <pthread.h>
#include "LogUtils.h"
#include "MPAudio.h"

extern "C"
{
#include "libavformat/avformat.h"
#include <libavutil/time.h>
};


class FFmpeg {


public:
    FFmpegCallBack *callBack=NULL;   // 回调
    const char *url = NULL;
    pthread_t decodeThread;   // 编码线程
    AVFormatContext *pFormatCtx=NULL;    // 上下文
    PlayStatus *playStatus=NULL;
    MPAudio *mpAudio=NULL;

    pthread_mutex_t init_mutex;
    bool exit = false;
    int duration = 0;


public:
    FFmpeg(PlayStatus *playstatus, FFmpegCallBack *callJava, const char *url);
    ~FFmpeg();

    void parpared();
    void decodeFFmpegThread();
    void start();
    void pause();
    void resume();

    void release();

};


#endif //MEDIAPLAYER_FFMPEG_H
