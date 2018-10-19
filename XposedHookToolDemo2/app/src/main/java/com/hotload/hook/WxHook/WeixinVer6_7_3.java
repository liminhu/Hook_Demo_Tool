package com.hotload.hook.WxHook;

import android.webkit.ValueCallback;

public class WeixinVer6_7_3 extends WeixinVerBase {

    public WeixinVer6_7_3(){
        initX5();
        initLog();
        initAppBrandSysConfig();
        initWeiXinDebug();
        initAppBrandMenu();
        initAppBrandMenuItem();
        initAppBrandMountJs();
        initAppBrandWebViewClient();
        initAppBrandWebViewInject();
    }

    @Override
    protected void initX5() {
        X5_CLS_WEBVIEW_WEBVIEW = "com.tencent.smtt.sdk.WebView";
        X5_CLS_WEBVIEW_VALUECALLBACK = ValueCallback.class.getName();
        X5_SIMPLE_FUN_WEBVIEW_LOADURL = "loadUrl";
        X5_SIMPLE_FUNC_WEBVIEW_EVALUATEJAVASCRIPT = "evaluateJavascript";
        X5_SIMPLE_FUN_WEBVIEW_LOADDATA = "loadData";
        X5_SIMPLE_FUN_WEBVIEW_LOADDATAWITHBASEURL = "loadDataWithBaseURL";
    }

    @Override
    protected void initLog() {
        WXLOG_CLS_PLATFORMTOOLS_LOG = "com.tencent.mm.sdk.platformtools.x";
        WXLOG_SIMPLE_FUN_LOG_F = "f";
        WXLOG_SIMPLE_FUN_LOG_E = "e";
        WXLOG_SIMPLE_FUN_LOG_W = "w";
        WXLOG_SIMPLE_FUN_LOG_I = "i";
        WXLOG_SIMPLE_FUN_LOG_D = "d";
        WXLOG_SIMPLE_FUN_LOG_V = "v";
        WXLOG_SIMPLE_FUN_LOG_K = "k";
        WXLOG_SIMPLE_FUN_LOG_L = "l";
        ABLOG_CLS_JSAPI_PARMA0 = "com.tencent.mm.plugin.appbrand.l";
        ABLOG_CLS_JSAPI_LOG = "com.tencent.mm.plugin.appbrand.jsapi.ak";
        ABLOG_SIMPLE_FUN_JSAPI_LOG = "a";
    }

    @Override
    protected void initAppBrandSysConfig() {
        ABI_CLS_APPBRAND_APPBRANDSYSCONFIG = "com.tencent.mm.plugin.appbrand.config.AppBrandSysConfig";
        ABI_CLS_APPBRAND_WXAPKGWRAPPINGINFO = "com.tencent.mm.plugin.appbrand.appcache.WxaPkgWrappingInfo";
        ABI_CLS_APPBRAND_INIT_CONFIG = "com.tencent.mm.plugin.appbrand.g$15";
        ABI_SIMPLE_FUN_APPBRAND_INIT_CONFIG = "b";
        ABI_FIELD_CONFIG_APPBRAND_DEBUG = "fqw";
        ABI_FIELD_CONFIG_APPBRAND_WXAPKGWRAPPINGINFO = "frm";
        ABI_FIELD_CONFIG_APPBRAND_APPID = "appId";
        ABI_FIELD_CONFIG_APPBRAND_GAMENAME = "bKC";
        ABI_FIELD_WRAPPINGINFO_APPBRAND_DEBUG = "fih";
    }

    @Override
    protected void initWeiXinDebug() {
        ABI_CLS_WEIXIN_CONFIG_DEBUG = "com.tencent.mm.sdk.a.b";
        ABI_SIMPLE_FUN_WEIXIN_CONFIG_DEBUG = "chp";
    }

    @Override
    protected void initAppBrandMenu() {
        ABI_CLS_APPBRAND_MENU_ADD_PARMA1 = "com.tencent.mm.plugin.appbrand.page.p";
        ABI_CLS_APPBRAND_MENU_ADD_PARMA2 = "com.tencent.mm.ui.base.l";
        ABI_SIMPLE_FUN_APPBRAND_MENU_ADD = "a";
        ABI_CLS_APPBRAND_MENU_CHICK_PARMA1 = ABI_CLS_APPBRAND_MENU_ADD_PARMA1;
        ABL_CLS_APPBRAND_MENU_CHICK_PARMA3 = "com.tencent.mm.plugin.appbrand.menu.k";
        ABI_SIMPLE_FUN_APPBRAND_MENU_CHICK = "a";
        ABI_CLS_APPBRAND_MENU_WIDGET_BASE = ABI_CLS_APPBRAND_MENU_ADD_PARMA2;
        ABI_SIMPLE_FUN_APPBRAND_MENU_ADD_WIDGET_BASE = "e";
    }

    @Override
    protected void initAppBrandMenuItem() {
        ABI_CLS_APPBRAND_MENU_DEBUG = "com.tencent.mm.plugin.appbrand.menu.MenuDelegate_EnableDebug";
        ABI_CLS_APPBRAND_MENU_FORWARD = "com.tencent.mm.plugin.appbrand.menu.g";
        ABI_CLS_APPBRAND_MENU_DISPLAYDEBUGGING = "com.tencent.mm.plugin.appbrand.menu.i";
        ABI_CLS_APPBRAND_MENU_APPID = "com.tencent.mm.plugin.appbrand.menu.c";
        ABI_CLS_APPBRAND_MENU_BASE_SHOW = "com.tencent.mm.plugin.appbrand.page.p";
        ABI_SIMPLE_FUN_APPBRAND_MENU_BASE_SHOW = "a";
    }

    @Override
    protected void initAppBrandMountJs() {
        ABI_CLS_APPBRAND_MOUNT_JS = "com.tencent.mm.plugin.appbrand.r.i";
        ABI_SIMPLE_FUN_APPBRAND_MOUNT_JS = "a";
        ABI_CLS_APPBRAND_MOUNT_JS_PARMA0 = "com.tencent.mm.plugin.appbrand.g.b";
        ABI_CLS_APPBRAND_MOUNT_JS_PARMA3 = "com.tencent.mm.plugin.appbrand.r.i$a";
    }

    @Override
    protected void initAppBrandWebViewClient() {
        ABI_CLS_APPBRAND_WEBVIEW_CLIENT0 = "com.tencent.mm.plugin.appbrand.l";
        ABI_FIELD_APPBRAND_WEBVIEW_CLIENT0 = "fdQ";
        ABI_CLS_APPBRAND_WEBVIEW_CLIENT1 = "com.tencent.mm.plugin.appbrand.game.d";
        ABI_SIMPLE_FUN_APPBRAND_WEBVIEW_CLIENT1 = "aaE";
    }

    @Override
    protected void initAppBrandWebViewInject() {
        ABI_CLS_APPBRAND_WEBVIEW_INJECT = "com.tencent.mm.plugin.appbrand.game.g";
        ABI_SIMPLE_FUN_APPBRAND_WEBVIEW_EVALUATEJAVASCRIPT = "evaluateJavascript";
    }
}
