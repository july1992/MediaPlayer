//
// Created by dev on 2019/1/10.
//

#ifndef MEDIAPLAYER_LOGUTILS_H
#define MEDIAPLAYER_LOGUTILS_H
#define LOG_DEBUG true
#include "android/log.h"

#define logd(FORMAT,...) __android_log_print(ANDROID_LOG_DEBUG,"vily-----",FORMAT,##__VA_ARGS__);
#define loge(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"vily-----",FORMAT,##__VA_ARGS__);

#endif //MEDIAPLAYER_LOGUTILS_H
