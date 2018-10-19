package com.my.xpoosed.hook.demo;

import android.app.ActivityManager;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.hu.myxposeddemo.R;
import com.my.utils.tool.MyLog;

import org.apache.commons.io.IOUtils;
import org.apache.commons.lang3.StringUtils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        new PluginLoadTask().start();


        new Thread() {
            @Override
            public void run() {
                try {
                    //10秒后自杀
                    Thread.sleep(10000);
                    MyLog.e("10秒后自杀");
                    MainActivity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            MainActivity.this.finish();
                            System.exit(0);
                        }
                    });
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }.start();
    }





    private class PluginLoadTask extends Thread {
        PluginLoadTask() {
            super("restart-master-thread");
        }

        @Override
        public void run() {
            super.run();
            InputStream inputStream =null;
            try{
                inputStream = getAssets().open("hotload_entry.txt");
                if (inputStream == null) {
                    MainActivity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            MyLog.e("hotload_entry 加载失败 .... ");
                            Toast.makeText(MainActivity.this, "hotload_entry 加载失败", Toast.LENGTH_LONG).show();
                        }
                    });
                    return;
                }
            }catch (Exception e){
                MyLog.e("hotload_entry 加载失败 111  .... ");
                Toast.makeText(MainActivity.this, "hotload_entry 加载失败 111", Toast.LENGTH_LONG).show();
                return;
            }

          //  MyLog.e("hotload_entry 加载成功。。 .... ");
            try {
                BufferedReader moduleClassesReader = new BufferedReader(new InputStreamReader(inputStream));
                String taskName;
                while ((taskName = moduleClassesReader.readLine()) != null) {
                    taskName = taskName.trim();
                    //ignore for action
                    if (!taskName.startsWith("action:")) {
                        continue;
                    }
                    taskName = taskName.substring("action:".length());
                    int i = taskName.indexOf(":");
                    if (i <= 0) {
                        //illegal
                        continue;
                    }
                    String action = taskName.substring(0, i);
                    String param = taskName.substring(i + 1);
                    MyLog.d("param: "+param);
                    if (StringUtils.equalsIgnoreCase(action, "kill-package")) {
                        restartPackage(param);
                    }

                }
            } catch (IOException e) {
                Log.e("xposedhooktool", "读取 hotload_entry 失败", e);
            } finally {
                IOUtils.closeQuietly(inputStream);
            }
        }
    }




    private void restartPackage(String packageName) {
        if (StringUtils.isBlank(packageName)) {
            return;
        }
        ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        if (activityManager == null) {
            Log.e("xposedhooktool", "can not get activityManager instance");
            return;
        }
        //先杀掉后台进程
        activityManager.killBackgroundProcesses(packageName);
    }


}
