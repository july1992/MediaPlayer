package com.vily.mediaplayer;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.vily.ffmpegmedia.FFmpegUtils;
import com.vily.ffmpegmedia.TimeInfoBean;
import com.vily.ffmpegmedia.listener.FFmpegCallBackListener;
import com.vily.ffmpegmedia.utils.TimeUtil;

import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    private FFmpegUtils mFFmpegUtils;
    private TextView mTv_time;

    private int count=0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mTv_time = findViewById(R.id.tv_time);


//        Timer timer=new Timer();
//        timer.schedule(new TimerTask() {
//            @Override
//            public void run() {
//
//                Log.i(TAG, "run: ---------音频帧："+count*8/1024);
//                count=0;
//
//            }
//        },1000,1000);

        mFFmpegUtils = new FFmpegUtils();
        mFFmpegUtils.setFFmpegCallBackListener(new FFmpegCallBackListener() {
            @Override
            public void onPrepared() {

                Log.i(TAG, "准备好啦，开始播放声音: -----------");
                mFFmpegUtils.start();
            }

            @Override
            public void onLoad(boolean load) {
                Log.i(TAG, "onLoad: --------正在加载中load:"+load);
            }

            @Override
            public void onCallByte(byte[] bytes) {


            }

            @Override
            public void onTimeInfo(final TimeInfoBean timeInfoBean) {

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mTv_time.setText(TimeUtil.secdsToDateFormat(timeInfoBean.getTotalTime(), timeInfoBean.getTotalTime())
                                + "/" + TimeUtil.secdsToDateFormat(timeInfoBean.getCurrentTime(), timeInfoBean.getTotalTime()));

                    }
                });


            }

            @Override
            public void onAudioData(int length) {
                Log.i(TAG, "onAudioData: -------length:"+length);
                count+=length;
            }

            @Override
            public void onPause(boolean pause) {
                Log.i(TAG, "onPause: --------当前状态 "+pause);
            }

            @Override
            public void onComplete() {
                Log.i(TAG, "onComplete: --------结束了");
            }
        });

    }


    public void begin(View view) {
//        mFFmpegUtils.setSource(Environment.getExternalStorageDirectory().getAbsolutePath()+"/wl_opensl_record.pcm");

        mFFmpegUtils.setSource(Environment.getExternalStorageDirectory().getAbsolutePath()+"/wl_live_pusher.mp4");

        if(initPerssion()){
            mFFmpegUtils.prepare();
        }

    }


    public boolean initPerssion(){

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, PERMISSIONS_STORAGE, 10);

            return false;
        }
        return true;

    }

    private static String[] PERMISSIONS_STORAGE = {
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE};

    public void pause(View view) {

        mFFmpegUtils.pause();
    }

    public void resume(View view) {
        mFFmpegUtils.resume();
    }

    public void stop(View view) {
        mFFmpegUtils.stop();
    }
}

