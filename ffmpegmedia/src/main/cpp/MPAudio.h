/**
 *  * description : 
 *  * Author : Vily
 *  * Date : 2019/1/10
 *  
 **/


#ifndef MEDIAPLAYER_AUDIO_H
#define MEDIAPLAYER_AUDIO_H

extern "C"
{
#include "libavcodec/avcodec.h"
#include <libswresample/swresample.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
};

#include "PlayStatus.h"
#include "MPQueue.h"
#include "FFmpegCallBack.h"

class MPAudio {

public:
    int streanIndex = -1;  // 设置起始索引地址
    AVCodecContext *avCodecContext = NULL;
    AVCodecParameters *codecPar = NULL;
    PlayStatus *playStatus = NULL;
    FFmpegQueue *queue = NULL;

    FFmpegCallBack *callBack = NULL;  // 回调

    int duration = 0;  // 总时长
    AVRational time_base; // 时间基数单位
    double clock;//总的播放时长
    double now_time;//当前frame时间
    double last_tiem; //上一次调用时间

    pthread_t thread_play;   // 播放线程
    int sample_rate = 0; // 采样率
    uint8_t *buffer = NULL;   // PCM 数据大小
    int data_size = 0;

    AVPacket *avPacket = NULL;   // 采样前的数据
    AVFrame *avFrame = NULL;     // 采样后的数据

    int ret = 0;


    // 引擎接口
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine = NULL;

    //混音器
    SLObjectItf outputMixObject = NULL;
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
    SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    //pcm
    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay = NULL;

    //缓冲器队列接口
    SLAndroidSimpleBufferQueueItf pcmBufferQueue = NULL;

public:
    MPAudio(PlayStatus *status,int sample_rate,FFmpegCallBack *callBack);

    ~MPAudio();

    void play(); // 开始播放

    void pause();

    void resume();

    void release();


    void initOpenSLES();  //  初始化 openSSL
    int getCurrentSampleRateForOpensles(int sample_rate);

    int resampleAudio();
};


#endif //MEDIAPLAYER_AUDIO_H
