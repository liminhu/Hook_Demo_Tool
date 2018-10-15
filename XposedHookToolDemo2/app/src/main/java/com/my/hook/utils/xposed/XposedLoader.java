package com.my.hook.utils.xposed;

import android.app.Application;
import android.content.Context;
import com.my.utils.tool.MyLog;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Enumeration;
import dalvik.system.DexFile;
import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage.LoadPackageParam;


public class XposedLoader implements IXposedHookLoadPackage {
    @Override
    public void handleLoadPackage(final LoadPackageParam lpparam) throws Throwable {
        final String prefsValue = Common.PACKAGE_NAME;
        if (prefsValue.equals(lpparam.packageName)) {
            MyLog.e("pkg: "+lpparam.packageName);
            XposedHelpers.findAndHookMethod(Application.class, "attach", Context.class,
                    new XC_MethodHook() {
                @Override
                protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                    Context context = (Context) param.args[0];
                    hookAll(context, true, lpparam);
                }
            });
        }
    }

    private void hookAll(final Context context, boolean hookFromStart,
                         final LoadPackageParam loadPackageParam) throws IOException, ClassNotFoundException {

        MyLog.d("my_dexfile   ---- "+loadPackageParam.appInfo.sourceDir);
        DexFile dexFile = new DexFile(loadPackageParam.appInfo.sourceDir);
        Enumeration<String> classNames = dexFile.entries();
        while (classNames.hasMoreElements()) {
            final String className = classNames.nextElement();
           // MyLog.d("className   ---- "+className);

            if (UtilsTool.isClassValid(loadPackageParam.packageName, className)) {
                final Class clazz = Class.forName(className, false, loadPackageParam.classLoader);

                if(clazz.getName().contains("")){

                }

                XposedBridge.hookAllConstructors(clazz, new XC_MethodHook() {
                    @Override
                    protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                        MyLog.d("--- class : "+param.thisObject.getClass().getName());
                    }

                    @Override
                    protected void afterHookedMethod(MethodHookParam param) throws Throwable {

                    }
                });

                for (final Method method: clazz.getDeclaredMethods()) {
                    MyLog.d("method -- "+method.getName());

                    if (UtilsTool.isMethodValidNoAbstract(method)) {
                        XposedBridge.hookMethod(method, new XC_MethodHook() {
                            @Override
                            protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                                     MyLog.e("hook classs method : "+param.thisObject.getClass().getName());
                            }

                            @Override
                            protected void afterHookedMethod(MethodHookParam param) throws Throwable {

                            }
                        });
                    }
                }
            }
        }
    }
}
