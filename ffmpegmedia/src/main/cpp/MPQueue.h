/**
 *  * description :   队列
 *  * Author : Vily
 *  * Date : 2019/1/10
 *  
 **/


#ifndef MEDIAPLAYER_QUEUE_H
#define MEDIAPLAYER_QUEUE_H

#include "queue"
#include "pthread.h"
#include "PlayStatus.h"
#include "LogUtils.h"

extern "C"
{
#include "libavcodec/avcodec.h"
};


class queue;

class FFmpegQueue {

public:
    std::queue<AVPacket *> queuePacket;  // 初始化一个队列
    pthread_mutex_t mutexPacket;    // 线程锁
    pthread_cond_t condpacket;   // 线程条件变量
    PlayStatus *playStatus=NULL;

public:
    FFmpegQueue(PlayStatus *playStatue);
    ~FFmpegQueue();

    int putAvpacket(AVPacket *packet);   //入队
    int getAvpacket(AVPacket *packet);   //出队

    int getQueueSize();
};


#endif //MEDIAPLAYER_QUEUE_H
