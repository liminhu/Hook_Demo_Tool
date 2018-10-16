package com.hotload.hook;

import android.content.Context;

import com.google.common.collect.Lists;
import com.hotload.hook.utils.SharedObject;
import com.hotload.hook.utils.XposedHotLoadCallBack;
import com.my.utils.tool.MyLog;

import org.apache.commons.io.IOUtils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Collections;
import java.util.List;

import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class HotLoadPackageEntry {

    //这里需要通过反射调用，HotLoadPackageEntry的entry的全路径不允许改变（包括方法签名），方法签名是xposed回调和热加载器的桥梁，需要满足调用接口规范
    //但是这个类的其他地方是可以修改的，因为这个代码已经是在最新插件apk的类加载器里面执行了

    @SuppressWarnings("unused")
    public static void entry(ClassLoader masterClassLoader, ClassLoader pluginClassLoader, Context context, XC_LoadPackage.LoadPackageParam loadPackageParam) {

        //将一批有用的对象放置到静态区域，方便使用
        SharedObject.context = context;
        SharedObject.loadPackageParam = loadPackageParam;
        SharedObject.masterClassLoader = masterClassLoader;
        SharedObject.pluginClassLoader = pluginClassLoader;

        List<XposedHotLoadCallBack> allCallBack = findAllCallBack();
        if (allCallBack.isEmpty()) {
            SharedObject.clear();
            return;
        }


        //加载业务代码回调
        for (XposedHotLoadCallBack callBack : allCallBack) {
            callBack.onXposedHotLoad();
        }
    }

    private static List<XposedHotLoadCallBack> findAllCallBack() {
        InputStream stream = SharedObject.pluginClassLoader.getResourceAsStream("assets/hotload_entry.txt");
        if (stream == null) {
            MyLog.e("hotload_entry 加载失败 .... ");
            return Collections.emptyList();
        }
      //  MyLog.e("hotload_entry 加载成功 .... ");
        List<XposedHotLoadCallBack> result = Lists.newLinkedList();
        try {
            BufferedReader moduleClassesReader = new BufferedReader(new InputStreamReader(stream));
            String moduleClassName;
            while ((moduleClassName = moduleClassesReader.readLine()) != null) {
                moduleClassName = moduleClassName.trim();
                //ignore blank
                if (moduleClassName.isEmpty()) {
                    continue;
                }
                //ignore for comment
                if (moduleClassName.startsWith("#")) {
                    continue;
                }
                //ignore for action
                if (moduleClassName.startsWith("action:")) {
                    continue;
                }
                try {
                    Class<?> moduleClass = SharedObject.pluginClassLoader.loadClass(moduleClassName);

                    if (!XposedHotLoadCallBack.class.isAssignableFrom(moduleClass)) {
                        MyLog.e("    This class doesn't implement any sub-interface of XposedHotLoadCallBack, skipping it");
                        continue;
                    }
                    XposedHotLoadCallBack moduleInstance = (XposedHotLoadCallBack) moduleClass.newInstance();
                    if (moduleInstance.needHook(SharedObject.loadPackageParam)) {
                        result.add(moduleInstance);
                    }

                } catch (Throwable t) {
                    MyLog.e("    Failed to load class " + moduleClassName);
                }
            }
        } catch (IOException e) {
            MyLog.e("load hot plugin failed");
            MyLog.e(e.getMessage());
        } finally {
            IOUtils.closeQuietly(stream);
        }
        return result;
    }
}
