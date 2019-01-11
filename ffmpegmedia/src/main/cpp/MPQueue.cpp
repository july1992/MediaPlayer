/**
 *  * description : 
 *  * Author : Vily
 *  * Date : 2019/1/10
 *  
 **/


#include "MPQueue.h"



//  初始化队列
FFmpegQueue::FFmpegQueue(PlayStatus *playStatue) {

    this->playStatus=playStatue;
    pthread_mutex_init(&mutexPacket,NULL); // 初始化线程锁
    pthread_cond_init(&condpacket,NULL);   // 初始化条件变量
}

FFmpegQueue::~FFmpegQueue() {

}

int FFmpegQueue::putAvpacket(AVPacket *packet) {

    pthread_mutex_lock(&mutexPacket);    // 加锁入队

    queuePacket.push(packet);
    logd("放入一个AVPacket到队列里去，个数为：%d",queuePacket.size());

    pthread_cond_signal(&condpacket);   // 通知消费者有入队
    pthread_mutex_unlock(&mutexPacket);  // 解锁

    return 0;
}

int FFmpegQueue::getAvpacket(AVPacket *packet) {

    pthread_mutex_lock(&mutexPacket);    // 加锁入队

    while (playStatus!=NULL && !playStatus->exit){   // 当前exit为false

        if(queuePacket.size()>0){
            AVPacket *avPacket=queuePacket.front();
            if(av_packet_ref(packet,avPacket)==0){  // ？把 avPacket的内存数据 拷贝到 packet中  这里拷贝的数据是 uint8_t *data;
                queuePacket.pop();   // 将avPacket出队
            }
            av_packet_free(&avPacket);  // 释放资源
            av_free(avPacket);
            avPacket=NULL;
            logd("从队列里取出了一个ACpacket，还剩 %d",queuePacket.size());

            break;
        }else{
            pthread_cond_wait(&condpacket, &mutexPacket);  // 等待并解锁
        }

    }


    pthread_mutex_unlock(&mutexPacket);  // 解锁
    return 0;
}

int FFmpegQueue::getQueueSize() {
    int size=0;
    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();          // 计算队列数据到时候也要记得加锁解锁
    pthread_mutex_unlock(&mutexPacket);
    return size;
}
