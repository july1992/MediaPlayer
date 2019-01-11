/**
 *  * description :    播放状态
 *  * Author : Vily
 *  * Date : 2019/1/10
 *  
 **/


#ifndef MEDIAPLAYER_PLAYSTATUS_H
#define MEDIAPLAYER_PLAYSTATUS_H


class PlayStatus {

public:
    bool exit= false;
    bool load= true; // 正在加载中

public:
    PlayStatus();
};


#endif //MEDIAPLAYER_PLAYSTATUS_H
