package com.hotload.hook.utils;

import de.robv.android.xposed.callbacks.XC_LoadPackage;

public interface XposedHotLoadCallBack {
    void onXposedHotLoad();

    boolean needHook(XC_LoadPackage.LoadPackageParam loadPackageParam);
}
