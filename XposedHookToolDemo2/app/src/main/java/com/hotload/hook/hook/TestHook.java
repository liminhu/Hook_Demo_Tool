package com.hotload.hook.hook;

import com.hotload.hook.utils.SharedObject;
import com.hotload.hook.utils.XposedHotLoadCallBack;
import com.my.hook.utils.xposed.Common;
import com.my.utils.tool.MyLog;

import org.apache.commons.lang3.StringUtils;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

/**
 * 这是一个demo，演示如何使用热加载机制来hook代码，需要注意的是这入口需要在 /assets/hotload_entry.txt里面配置，才能被热加载模块启用
 */

public class TestHook implements XposedHotLoadCallBack {
    private boolean hasHook = false;

    @Override
    public void onXposedHotLoad() {
        if (hasHook) {
            return;
        }
        hasHook = true;
        //这里是真正hook的代码，所有需要访问的参数，都在com.virjar.xposedhooktool.hotload.SharedObject里面
        MyLog.e("hook package: " + SharedObject.loadPackageParam.packageName + " and plugin location is: success ... 123 ");
    }

    @Override
    public boolean needHook(XC_LoadPackage.LoadPackageParam loadPackageParam) {
        //只hook易通行
        Common.HOOK_PACKAGE_NAME="com.zhb.samsung.health.demo";
        return StringUtils.equalsIgnoreCase(loadPackageParam.packageName, Common.HOOK_PACKAGE_NAME);
    }
}
