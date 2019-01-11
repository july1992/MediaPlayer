//
// Created by dev on 2019/1/10.    头文件    定义回调的方法名 对象等
//

#ifndef MEDIAPLAYER_FFMPEGCALLBACK_H
#define MEDIAPLAYER_FFMPEGCALLBACK_H

#define MAIN_THREAD 0    // 主线程
#define CHILD_THREAD 1   // 子线程

#include <jni.h>
#include <linux/stddef.h>
#include "LogUtils.h"


class FFmpegCallBack {

public:
    _JavaVM *javaVM = NULL;      // 进程JVM
    JNIEnv *jniEnv = NULL;       // 线程环境ENV
    jobject jobj; // 定义全局对象

    jmethodID jmid_prepared;   // prepare的回调
    jmethodID jmid_load;   // prepare的回调
    jmethodID jmid_timeInfo;   // prepare的回调
    jmethodID jmid_audioData;   // data的回调

public:

    FFmpegCallBack(_JavaVM *javaVM,JNIEnv *jniEnv,jobject *obj);
    ~FFmpegCallBack();

    void onCallPrepared(int type);    //  type用来区分当前是在线程里还是线程外
    void onCallLoad(int type, bool load);   // 加载回调
    void onCallTimeInfo(int type,int curr,int total);
    void onCallAudioData(int type,int length);
};


#endif //MEDIAPLAYER_FFMPEGCALLBACK_H
