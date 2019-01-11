#include <jni.h>
#include <string>
#include "LogUtils.h"
#include "FFmpegCallBack.h"
#include "FFmpeg.h"


extern "C"
{
#include <libavformat/avformat.h>
}


_JavaVM *javaVM = NULL;
FFmpegCallBack *callJava = NULL;
FFmpeg *fFmpeg = NULL;
PlayStatus *playstatus = NULL;

bool nexit = true;

// 获取到线程到ENV  当回调当时候会用到这个  JVM是进程相关到   ENV 是线程相关到
extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    javaVM = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {

        return result;
    }
    return JNI_VERSION_1_4;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_vily_ffmpegmedia_FFmpegUtils_n_1parpared(JNIEnv *env, jobject instance, jstring source_) {

    const char *source = env->GetStringUTFChars(source_, 0);

    // TODO
    logd("-----------开始准备工作");

    if (fFmpeg == NULL) {
        if (callJava == NULL) {

            callJava = new FFmpegCallBack(javaVM, env, &instance);
        }

        playstatus = new PlayStatus();

        fFmpeg = new FFmpeg(playstatus, callJava, source);
        fFmpeg->parpared();
    }
    env->ReleaseStringUTFChars(source_, source);
    return;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_vily_ffmpegmedia_FFmpegUtils_n_1start(JNIEnv *env, jobject instance) {

    // TODO

    if (fFmpeg != NULL) {

        logd("开始准备解码")
        fFmpeg->start();
    }

    return;

}

//  暂停
extern "C"
JNIEXPORT void JNICALL
Java_com_vily_ffmpegmedia_FFmpegUtils_n_1pause(JNIEnv *env, jobject instance) {

    // TODO


    if (fFmpeg != NULL) {
        fFmpeg->pause();
    }

    return;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_vily_ffmpegmedia_FFmpegUtils_n_1resume(JNIEnv *env, jobject instance) {

    // TODO
    if (fFmpeg != NULL) {
        fFmpeg->resume();
    }


    return;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_vily_ffmpegmedia_FFmpegUtils_n_1stop(JNIEnv *env, jobject instance) {


    logd("走这里吗")


    // TODO

    if (!nexit) {
        return;
    }
    nexit = false;   // 防止重复去退出
    if (fFmpeg != NULL) {
        fFmpeg->release();
        delete(fFmpeg);
        fFmpeg=NULL;
        if(callJava != NULL)
        {
            delete(callJava);
            callJava = NULL;
        }
        if(playstatus != NULL)
        {
            delete(playstatus);
            playstatus = NULL;
        }
    }
    nexit = true;
}