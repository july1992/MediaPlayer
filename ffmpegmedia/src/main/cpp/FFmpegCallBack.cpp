//
// Created by dev on 2019/1/10.    回调方法  用于调用Java 的方法  并将C++里面产生的数据反馈回去
//

#include "FFmpegCallBack.h"



// 初始化
FFmpegCallBack::FFmpegCallBack(_JavaVM *javaVM, JNIEnv *env, jobject *obj) {
    this->javaVM=javaVM;
    this->jniEnv=env;
    this->jobj=*obj;
    this->jobj=env->NewGlobalRef(jobj);

    jclass jlz=jniEnv->GetObjectClass(jobj);
    if(!jlz){

        if(LOG_DEBUG){

            logd("jclass 获取错误")
        }
        return ;
    }
    // 获取到 Java层定义到方法，用于回调   onCallParpared  是方法到名字    ()V是签名
    jmid_prepared=env->GetMethodID(jlz,"onCallPrepared","()V");
    jmid_load=env->GetMethodID(jlz,"onCallLoad","(Z)V");
    jmid_timeInfo=env->GetMethodID(jlz,"onCallTimeInfo","(II)V");
    jmid_audioData=env->GetMethodID(jlz,"onCallAudioData","(I)V");
}

// 析构函数
FFmpegCallBack::~FFmpegCallBack() {

}

void FFmpegCallBack::onCallPrepared(int type) {
    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj, jmid_prepared);    // 调用java层到方法
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            if(LOG_DEBUG)
            {
                loge("子线程到env获取失败");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_prepared);
        javaVM->DetachCurrentThread();
    }
}

void FFmpegCallBack::onCallLoad(int type, bool load) {
    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj, jmid_load,load);    // 调用java层到方法
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            if(LOG_DEBUG)
            {
                loge("子线程到env获取失败");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_load,load);
        javaVM->DetachCurrentThread();
    }

}

void FFmpegCallBack::onCallTimeInfo(int type,int curr, int total) {

    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj, jmid_timeInfo,curr,total);    // 调用java层到方法
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            if(LOG_DEBUG)
            {
                loge("子线程到env获取失败");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_timeInfo,curr,total);
        javaVM->DetachCurrentThread();
    }
}

void FFmpegCallBack::onCallAudioData(int type, int length) {
    if(type == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj, jmid_audioData,length);    // 调用java层到方法
    }
    else if(type == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            if(LOG_DEBUG)
            {
                loge("子线程到env获取失败");
            }
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_audioData,length);
        javaVM->DetachCurrentThread();
    }
}
