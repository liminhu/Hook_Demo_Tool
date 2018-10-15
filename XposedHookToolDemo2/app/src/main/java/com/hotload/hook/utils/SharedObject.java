package com.hotload.hook.utils;

import android.content.Context;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class SharedObject {
    public static ClassLoader masterClassLoader;
    public static ClassLoader pluginClassLoader;
    public static Context context;
    public static XC_LoadPackage.LoadPackageParam loadPackageParam;


    public static void clear() {
        masterClassLoader = null;
        pluginClassLoader = null;
        context = null;
        //  pluginApkLocation = null;
        loadPackageParam = null;
    }
}
