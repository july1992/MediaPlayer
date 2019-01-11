/**
 *  * description :   在这里做准备工作  开启编码线程  初始化出入栈
 *  * Author : Vily
 *  * Date : 2019/1/10
 *  
 **/



#include "FFmpeg.h"


FFmpeg::FFmpeg(PlayStatus *playstatus, FFmpegCallBack *callJava, const char *url) {

    this->playStatus = playstatus;
    this->callBack = callJava;
    this->url = url;

}


void *decodeFFmpeg(void *data) {
    logd("创建编码线程的回调");
    FFmpeg *fFmpeg = (FFmpeg *) data;
    fFmpeg->decodeFFmpegThread();
    pthread_exit(&fFmpeg->decodeThread);
}

void FFmpeg::parpared() {

    // 创建编码线程
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);
}

void FFmpeg::decodeFFmpegThread() {
    logd("开始编码线程");

    // 注册解码器  并初始化网络
    av_register_all();
    avformat_network_init();

    // 打开文件或网络流
    pFormatCtx = avformat_alloc_context();

    // avformat_open_input 返回0是成功
    if (avformat_open_input(&pFormatCtx, url, NULL, NULL) != 0) {

        loge("url打不开 :%s", url);

        return;
    }
    logd("获取到上下文");

    // 获取流信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        loge(" 获取不到流信息 %s", url);
        return;
    }
    logd("获取到了流信息");
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        logd("----i: %d", i); // 0是音频  1是视频

        // AVMEDIA_TYPE_AUDIO 是音频流
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)//得到音频流
        {
            if (mpAudio == NULL) {
                mpAudio = new MPAudio(playStatus, pFormatCtx->streams[i]->codecpar->sample_rate,
                                      callBack);
                mpAudio->streanIndex = i;
                mpAudio->codecPar = pFormatCtx->streams[i]->codecpar;
                mpAudio->duration = pFormatCtx->duration/AV_TIME_BASE;   // AV_TIME_BASE  这个获取到到是网络音频到总时长，如果是直播就没有这个
                mpAudio->time_base = pFormatCtx->streams[i]->time_base;

            }
        }
        // AVMEDIA_TYPE_VEDIO 是视频流
    }

    logd("audio 初始化成功----%d", mpAudio->streanIndex);   // =0
    // 获取解码器
    AVCodec *dec = avcodec_find_decoder(mpAudio->codecPar->codec_id);
    if (!dec) {
        logd("找不到编码器")
        return;
    }
    logd("找到了编码器")
    // 获取上下文
    mpAudio->avCodecContext = avcodec_alloc_context3(dec);
    if (!mpAudio->avCodecContext) {
        logd("解码器上下文获取失败")
        return;
    }
    logd("解码器上下文获取成功")
    if (avcodec_parameters_to_context(mpAudio->avCodecContext, mpAudio->codecPar) < 0) {
        loge("can not fill decodecctx");
        return;
    }
    if (avcodec_open2(mpAudio->avCodecContext, dec, 0) != 0) {
        loge("不能打开一个音频流")
        return;
    }
    loge("成功打开一个音频流")
    callBack->onCallPrepared(CHILD_THREAD);  // 音频解码器已经准备好啦
}

// 开始播放
void FFmpeg::start() {

    if (mpAudio == NULL) {

        loge("audio 为空")
        return;
    }

    int count = 0;    // 计算 解码到帧

    // 先初始化播放器等等 在去循环入队，播放的时候  从队列取出来即可
    mpAudio->play(); // 开始播放
    //  开始解码操作
    while (playStatus != NULL && !playStatus->exit) {

        AVPacket *avPacket = av_packet_alloc();
        if (av_read_frame(pFormatCtx, avPacket) == 0) {   // 读取音视频

            if (avPacket->stream_index == mpAudio->streanIndex) {

                count++;
                logd("解码第  %d 帧：", count)

                mpAudio->queue->putAvpacket(avPacket); // 入队其实是将数据入队
            } else {
                av_packet_free(&avPacket);   // 释放资源
                av_free(avPacket);

            }

        } else {

            logd("解码结束 ， 结束循环,但是还要判断下缓存里是不是还有数据")
            av_packet_free(&avPacket);
            av_free(avPacket);

//            break;

            // 判断是否还有缓存
            while (playStatus != NULL && !playStatus->exit) {

                if (mpAudio->queue->getQueueSize() > 0) {  // 当出队完全后在去结束

                    continue;
                } else {
                    logd("结束语：adda");
                    playStatus->exit = true;
                    break;
                }

            }

        }
    }

//    while (1){
//        AVPacket *avPacket = av_packet_alloc();
//        if(mpAudio->queue->getQueueSize()>0){
//
//            mpAudio->queue->getAvpacket(avPacket);
//
//
//        }else{
//            break;
//        }
//    }

    logd("解码结束")
}

FFmpeg::~FFmpeg() {

}

void FFmpeg::pause() {

    if(mpAudio!=NULL){
        mpAudio->pause();  // 暂停
    }

}

void FFmpeg::resume() {

    if(mpAudio!=NULL){
        mpAudio->resume();   // 恢复播放
    }

}


