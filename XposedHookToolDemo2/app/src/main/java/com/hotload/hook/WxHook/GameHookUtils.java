package com.hotload.hook.WxHook;

import android.app.Activity;
import android.app.Application;
import com.my.utils.tool.MyFileUtils;
import com.my.utils.tool.MyLog;
import java.io.File;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class GameHookUtils {
    private static boolean a;
    private static boolean b;

    public static void initHook(XC_LoadPackage.LoadPackageParam  arg6) {
        try {
            MyLog.d(" is begin hoook .... ");
            XposedHelpers.findAndHookMethod("com.tencent.tinker.loader.app.TinkerApplication", arg6.classLoader, "onCreate", new Object[]{new XC_MethodHook() {


                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    try{
                        Application v0 = (Application)param.thisObject;
                        if(appbrandIsLoad(v0.getClassLoader())) {
                            MyLog.e("JumpHook  -- %s", "TinkerApplication.onCreate " +v0.getClassLoader());
                            a=true;
                            firstMethod(v0.getClassLoader());
                        }
                        else {
                            activityOnCreate(v0.getClassLoader());
                        }

                    }catch (Exception e){
                        MyLog.e(" --- "+e.getMessage());
                    }

                }
            }});
        }
        catch(Throwable v0) {
            MyLog.d("JumpHook %s ", "" + v0.getMessage());
        }








    }





    private static void firstMethod(ClassLoader arg5) {
        try {
            XposedHelpers.findAndHookMethod(c1, arg5, "a", new Object[]{arg5.loadClass(c2), String.class, new XC_MethodHook() {
                protected void afterHookedMethod(MethodHookParam  arg6) {
                    String v0_1;
                    MyLog.d("反射中。。。。 00000  -- %s ", "args[0] " + arg6.args[0] + ", args[1]: " + arg6.args[1]);
                    if( arg6.args[0] != null) {
                        try{
                            if(((String)arg6.args[1]).contains(".js") && (((String)arg6.args[1]).contains("game.js") )){
                                String fileName=(String)arg6.args[1];
                                File js=new File("/sdcard/dump_game_js/"+fileName);
                                if(!js.getParentFile().exists()){
                                    js.getParentFile().mkdir();
                                }
                                if(!js.exists()){
                                    String result =(String) arg6.getResult();
                                    MyLog.e(result.length()+"------- ---不存在，现在开始存储--"+js.getAbsolutePath());
                                    MyFileUtils.newWriteDataToFile(result, js.getAbsolutePath());
                                }else{
                                    String result =(String) arg6.getResult();
                                    MyLog.d("不需要替换。。。 0000 --- len: "+result.length());
                                    v0_1 = new String(MyFileUtils.readFileByBytes("/sdcard/ddz_game.js"));
                                    if(v0_1!=null && v0_1.length()>100){
                                        MyLog.e("读取自定义的js  ，   定制 js .... "+v0_1.length());
                                       // arg6.setResult(v0_1);
                                    }else{
                                        MyLog.d("不需要替换。。。 ");
                                    }
                                }

                            }
                        }catch (Exception e){
                            MyLog.e("异常 --- "+e.getMessage());
                        }


                    }else {
                        MyLog.d(" is else ... 0000 ");
                    }
                }
            }});
        }
        catch(Throwable v0) {
            MyLog.d("JumpHook %s", "" + v0.getMessage());
        }
    }




    private static void activityOnCreate(ClassLoader arg5) {
        try {
            MyLog.e(" hook  activityOnCreate");
            XposedHelpers.findAndHookMethod("android.app.Activity", arg5, "onCreate", new Object[]{new XC_MethodHook() {


                @Override
                protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                    super.beforeHookedMethod(param);
                     Object v0 = param.thisObject;
                    MyLog.d("JumpHook  -- %d ", "onCreate " + v0 + ", " + ((Activity)v0).getClassLoader());
                    if(a && (appbrandIsLoad(((Activity)v0).getClassLoader()))) {
                        a=true;
                        firstMethod(((Activity)v0).getClassLoader());
                    }
                }
            }});
        }
        catch(Throwable v0) {
            MyLog.d("JumpHook  -- %s ", "" + v0);
        }
    }



   public static String c1 ="com.tencent.mm.plugin.appbrand.appcache.aq";
   public static String c2="com.tencent.mm.plugin.appbrand.i";



    private static boolean appbrandIsLoad(ClassLoader arg4) {
        boolean v0_1;
        try {
            arg4.loadClass(c1);
            arg4.loadClass(c2);
            v0_1 = true;
        }
        catch(Exception v0) {
            MyLog.d("JumpHook %s ", "" + v0.getMessage());
            v0_1 = false;
        }
        MyLog.i("is load ... appbrandIsLoad: "+v0_1);
        return v0_1;
    }

}
