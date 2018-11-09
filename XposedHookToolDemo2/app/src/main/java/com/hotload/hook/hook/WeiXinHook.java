package com.hotload.hook.hook;

import android.content.Context;
import android.text.TextUtils;

import com.hotload.hook.WxHook.GameHookUtils;
import com.hotload.hook.WxHook.HookLogUtils;
import com.hotload.hook.WxHook.WeixinVer6_7_3;
import com.hotload.hook.WxHook.WeixinVerBase;
import com.hotload.hook.utils.SharedObject;
import com.hotload.hook.utils.XposedHotLoadCallBack;
import com.my.hook.utils.xposed.Common;
import com.my.utils.tool.MyLog;

import org.apache.commons.lang3.StringUtils;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;


public class WeiXinHook implements XposedHotLoadCallBack {
    private boolean hasHook = false;
    private static final String APP_PROCESS_NAME = "com.tencent.mm:appbrand";
    private    WeixinVerBase weixin;

    @Override
    public void onXposedHotLoad() {
        if (hasHook) {
            return;
        }
        hasHook = true;
        MyLog.e("begin hook ... 000  ");
        String versinName = "";
        XC_LoadPackage.LoadPackageParam lpparam=SharedObject.loadPackageParam;
        if (lpparam.processName != null && (lpparam.processName.startsWith(APP_PROCESS_NAME))) {
            try {
                MyLog.d("XposedMain %s", lpparam.packageName + "---- lpparam.processName: " + lpparam.processName);
                Context context = null;
                try {
                    context = (Context) XposedHelpers.callMethod(XposedHelpers.callStaticMethod(XposedHelpers.findClass("android.app.ActivityThread", null), "currentActivityThread", new Object[0]), "getSystemContext", new Object[0]);
                    versinName = context.getPackageManager().getPackageInfo(lpparam.packageName, 0).versionName;
                    if (context == null) {
                        return;
                    }
                    MyLog.d(" vereion ... " + versinName + " --- flag : " );
                } catch (Exception e) {
                    MyLog.e("异 常 " + e.getMessage());
                    return;
                }
                  if (TextUtils.equals("6.6.7", versinName)) {
                    //兼容673
                    GameHookUtils.c1="com.tencent.mm.plugin.appbrand.appcache.ao";
                    GameHookUtils.c2="com.tencent.mm.plugin.appbrand.g";
                    GameHookUtils.initHook(lpparam);
                }
            } catch (Throwable v0) {
                //  MyLog.d("XposedMain  异常", "" + v0.getMessage());
            }
        }

        try {
            // TODO: 18-10-17 log目前只支持6.6.7
            if (TextUtils.equals("6.6.7", versinName)) {
                weixin=new WeixinVer6_7_3();
                MyLog.d("Hook  hook 微信log和小程序log  ");
                HookLogUtils logUtils=new HookLogUtils();
                logUtils.hookLog(weixin, SharedObject.masterClassLoader);




    /*            try {
                    // Hook小游戏修改 "转发" 菜单
                    Class<?> menuAddArg1Class = SharedObject.masterClassLoader.loadClass(weixin.ABI_CLS_APPBRAND_MENU_ADD_PARMA1);
                    Class<?> menuAddArg2Class = SharedObject.masterClassLoader.loadClass(weixin.ABI_CLS_APPBRAND_MENU_ADD_PARMA2);
                    Class<?> menuClickArg3Class =  SharedObject.masterClassLoader.loadClass(weixin.ABL_CLS_APPBRAND_MENU_CHICK_PARMA3);

                    XposedHelpers.findAndHookMethod(weixin.ABI_CLS_APPBRAND_MENU_DEBUG,  SharedObject.masterClassLoader, weixin.ABI_SIMPLE_FUN_APPBRAND_MENU_ADD, Context.class, menuAddArg1Class, menuAddArg2Class, String.class, injectCallback);
                    XposedHelpers.findAndHookMethod(weixin.ABI_CLS_APPBRAND_MENU_FORWARD, SharedObject.masterClassLoader, weixin.ABI_SIMPLE_FUN_APPBRAND_MENU_ADD, Context.class, menuAddArg1Class, menuAddArg2Class, String.class, injectCallback);
                    XposedHelpers.findAndHookMethod(weixin.ABI_CLS_APPBRAND_MENU_FORWARD,  SharedObject.masterClassLoader, weixin.ABI_SIMPLE_FUN_APPBRAND_MENU_CHICK, Context.class, menuAddArg1Class, String.class, menuClickArg3Class, new XC_MethodHook() {
                        @Override
                        protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                            MyLog.d( "修改原始 \"转发\" 菜单点击事件");

                        }
                    });
                }catch (Exception e){
                    MyLog.d( "Hook 微信小程序调试或转发菜单异常", e);
                }*/

            }
        }catch (Throwable e){
           MyLog.e("---- Hook Log 异常", e);
        }

    }







    private XC_MethodHook injectCallback = new XC_MethodHook() {
        @Override
        protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
            String name = param.method.getDeclaringClass().getName() + "." + param.method.getName();
            if (name.equals(weixin.ABI_CLS_APPBRAND_INIT_CONFIG + "." + weixin.ABI_SIMPLE_FUN_APPBRAND_INIT_CONFIG)){       // 只要在这里修改掉传入的AppBrandSysConfig即可打开调试功能
               MyLog.d( name + "() before called");
                Class<?> appBrandSysConfigClass = param.method.getDeclaringClass().getClassLoader().loadClass(weixin.ABI_CLS_APPBRAND_APPBRANDSYSCONFIG);
                Class<?> wxaPkgWrappingInfoClass = param.method.getDeclaringClass().getClassLoader().loadClass(weixin.ABI_CLS_APPBRAND_WXAPKGWRAPPINGINFO);
                Object arg0 = param.args[0];
                if (arg0 == null){
                    MyLog.d("传入的AppBrandSysConfig为空");     // 正常情况下是不为空的
                }else {
                    Field debugField = appBrandSysConfigClass.getDeclaredField(weixin.ABI_FIELD_CONFIG_APPBRAND_DEBUG);
                    debugField.setAccessible(true);
                    boolean debug = debugField.getBoolean(arg0);
                    Field appIdField = appBrandSysConfigClass.getDeclaredField(weixin.ABI_FIELD_CONFIG_APPBRAND_APPID);
                    appIdField.setAccessible(true);
                   String appId = (String) appIdField.get(arg0);
                    Field gameNameField = appBrandSysConfigClass.getDeclaredField(weixin.ABI_FIELD_CONFIG_APPBRAND_GAMENAME);
                    gameNameField.setAccessible(true);
                    String gameName = (String) gameNameField.get(arg0);
                    Field wwi_debugField = wxaPkgWrappingInfoClass.getDeclaredField(weixin.ABI_FIELD_WRAPPINGINFO_APPBRAND_DEBUG);
                    wwi_debugField.setAccessible(true);

                    Field wxapkgWrappingInfoField = appBrandSysConfigClass.getField(weixin.ABI_FIELD_CONFIG_APPBRAND_WXAPKGWRAPPINGINFO);
                    wxapkgWrappingInfoField.setAccessible(true);
                    Object wxapkgWrappingInfovObj = wxapkgWrappingInfoField.get(arg0);
                    int type = wwi_debugField.getInt(wxapkgWrappingInfovObj);
                    MyLog.d("原始AppBrandSysConfig.debug=" + Boolean.toString(debug) + " 原始WxaPkgWrappingInfo.type=" + type + " 游戏名字: " + gameName + " appId: " + appId);
                                 // 注入配置的读取不用运行在主线程中
                }
            }else if (name.equals(weixin.ABI_CLS_APPBRAND_MENU_DEBUG + "." + weixin.ABI_SIMPLE_FUN_APPBRAND_MENU_ADD)){
                hookAppBrandMenu(param, 3, "开启/关闭调试");
            }else if (name.equals(weixin.ABI_CLS_WEIXIN_CONFIG_DEBUG + "." + weixin.ABI_SIMPLE_FUN_WEIXIN_CONFIG_DEBUG)){
                // 这个函数在小游戏菜单中影响了 "appId: xxxxxx", "显示调试信息", "离开" 共三个菜单,而在其它地方调用也比较多,因此需要过滤下
                Throwable throwable = new Throwable();
                StackTraceElement[] elements = throwable.getStackTrace();
                if (elements.length > 4){
                    String s = elements[3].getClassName();
                    if (s.equals(weixin.ABI_CLS_APPBRAND_MENU_DISPLAYDEBUGGING) || s.equals(weixin.ABI_CLS_APPBRAND_MENU_APPID) || s.equals(weixin.ABI_CLS_APPBRAND_MENU_BASE_SHOW)){
                        MyLog.d("开启小游戏 appId 菜单项");
                        param.setResult(true);
                    }
                }
            }
        }
    };





    /**
     *  在微信小游戏菜单中添加菜单项
     * @param param         方法有关的参赛
     * @param id            菜单项对应的id,应该跟排序有关
     * @param name          菜单显示的名字
     * @throws Throwable
     */
    private void hookAppBrandMenu(XC_MethodHook.MethodHookParam param, int id, String name) throws Throwable{
      MyLog.d( "Hook 小程序菜单");
        Class<?> nClass = param.method.getDeclaringClass().getClassLoader().loadClass(weixin.ABI_CLS_APPBRAND_MENU_WIDGET_BASE);
        Method fMethod = nClass.getDeclaredMethod(weixin.ABI_SIMPLE_FUN_APPBRAND_MENU_ADD_WIDGET_BASE, int.class, CharSequence.class);
        fMethod.setAccessible(true);
        Object arg2 = param.args[2];
        fMethod.invoke(arg2, id, name);
       // param.setResult(null);
    }




    @Override
    public boolean needHook(XC_LoadPackage.LoadPackageParam loadPackageParam) {
        //只hook易通行
        return StringUtils.equalsIgnoreCase(loadPackageParam.packageName, Common.HOOK_PACKAGE_NAME);
    }
}
