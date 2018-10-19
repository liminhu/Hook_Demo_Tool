package com.hotload.hook.WxHook;

public abstract class WeixinVerBase {

    // 目前Hook的点分为三类, H5内核框架层, 微信日志及小程序日志层, 小游戏注入层

    // x5内核
    public String X5_CLS_WEBVIEW_WEBVIEW = "com.tencent.smtt.sdk.WebView";   // X5 WebView基类
    public String X5_CLS_WEBVIEW_VALUECALLBACK;                              // X5 WebView回调类
    public String X5_SIMPLE_FUN_WEBVIEW_LOADURL;                             // X5 loadurl
    public String X5_SIMPLE_FUNC_WEBVIEW_EVALUATEJAVASCRIPT;                 // X5 evaluateJavaScript
    public String X5_SIMPLE_FUN_WEBVIEW_LOADDATA;                            // X5 loadData
    public String X5_SIMPLE_FUN_WEBVIEW_LOADDATAWITHBASEURL;                 // X5 loadDataWithBaseURL

    /**
     * 配置X5内核
     */
    protected abstract void initX5();

    // 微信Log及小程序Log
    public String WXLOG_CLS_PLATFORMTOOLS_LOG="com.tencent.mm.sdk.platformtools.w";               // 微信Log类
    public String WXLOG_SIMPLE_FUN_LOG_F;                                                        // 微信Log方法名
    public String WXLOG_SIMPLE_FUN_LOG_E;
    public String WXLOG_SIMPLE_FUN_LOG_W;
    public String WXLOG_SIMPLE_FUN_LOG_I;
    public String WXLOG_SIMPLE_FUN_LOG_D;
    public String WXLOG_SIMPLE_FUN_LOG_V;
    public String WXLOG_SIMPLE_FUN_LOG_K;
    public String WXLOG_SIMPLE_FUN_LOG_L;
    public String ABLOG_CLS_JSAPI_PARMA0;                                                     // 小程序Log方法的参数0
    public String ABLOG_CLS_JSAPI_LOG;                                                        // 小程序Log类
    public String ABLOG_SIMPLE_FUN_JSAPI_LOG;                                                 // 小程序Log方法名

    /**
     * 配置微信log及小程序log
     */
    protected abstract void initLog();

    // 小程序AppBrandSysConfig获取及修改
    public String ABI_CLS_APPBRAND_APPBRANDSYSCONFIG;                             // AppBrandSysConfig 类
    public String ABI_CLS_APPBRAND_WXAPKGWRAPPINGINFO;                   // WxaPkgWrappingInfo 类
    public String ABI_CLS_APPBRAND_INIT_CONFIG;                          // 初始化AppBrandSysConfig类
    public String ABI_SIMPLE_FUN_APPBRAND_INIT_CONFIG;                   // 初始化AppBrandSysConfig方法
    public String ABI_FIELD_CONFIG_APPBRAND_DEBUG;                       // AppBrandSysConfig中debug字段
    public String ABI_FIELD_CONFIG_APPBRAND_WXAPKGWRAPPINGINFO;          // AppBrandSysConfig中WxaPkgWrappingInfo字段
    public String ABI_FIELD_CONFIG_APPBRAND_APPID;                       // AppBrandSysConfig中appId字段
    public String ABI_FIELD_CONFIG_APPBRAND_GAMENAME;                    // AppBrandSysConfig中游戏名字字段
    public String ABI_FIELD_WRAPPINGINFO_APPBRAND_DEBUG;                 // WxaPkgWrappingInfo中debug字段

    /**
     *  配置小程序 AppBrandSysConfig 获取基础config
     */
    protected abstract void initAppBrandSysConfig();


    // 微信整体Debug控制
    public String ABI_CLS_WEIXIN_CONFIG_DEBUG;                           // 微信整体上的debug配置,如果全部修改会影响很多地方
    public String ABI_SIMPLE_FUN_WEIXIN_CONFIG_DEBUG;                    // 微信debug配置方法

    /**
     * 配置 微信整体debug
     */
    protected abstract void initWeiXinDebug();

    // 小程序菜单修改有关
    public String ABI_CLS_APPBRAND_MENU_ADD_PARMA1;                      // 小程序添加菜单项参数1
    public String ABI_CLS_APPBRAND_MENU_ADD_PARMA2;                      // 小程序添加菜单项参数2
    public String ABI_SIMPLE_FUN_APPBRAND_MENU_ADD;                      // 小程序添加菜单方法名
    public String ABI_CLS_APPBRAND_MENU_CHICK_PARMA1;                    // 小程序菜单点击参数1
    public String ABL_CLS_APPBRAND_MENU_CHICK_PARMA3;                    // 小程序菜单点击参数3
    public String ABI_SIMPLE_FUN_APPBRAND_MENU_CHICK;                    // 小程序菜单点击方法名
    public String ABI_CLS_APPBRAND_MENU_WIDGET_BASE;                     // 小程序菜单UI基类
    public String ABI_SIMPLE_FUN_APPBRAND_MENU_ADD_WIDGET_BASE;          // 小程序菜单UI基类添加控件方法名
    /**
     * 配置 小程序菜单添加及点击方法
     */
    protected abstract void initAppBrandMenu();

    // 具体修改小程序有关的菜单
    public String ABI_CLS_APPBRAND_MENU_DEBUG;                           // 小程序 开启/关闭调试 菜单
    public String ABI_CLS_APPBRAND_MENU_FORWARD;                         // 小程序 转发 菜单
    public String ABI_CLS_APPBRAND_MENU_DISPLAYDEBUGGING;                // 小程序 显示调试信息 菜单
    public String ABI_CLS_APPBRAND_MENU_APPID;                           // 小程序 appId 菜单
    public String ABI_CLS_APPBRAND_MENU_BASE_SHOW;                       // 小程序菜单显示 调试控制 基类
    public String ABI_SIMPLE_FUN_APPBRAND_MENU_BASE_SHOW;                // 小程序菜单显示控制 方法名

    /**
     * 配置修改的具体菜单
     */
    protected abstract void initAppBrandMenuItem();

    // 小程序挂载脚本
    public String ABI_CLS_APPBRAND_MOUNT_JS;                             // 小程序 挂载脚本类
    public String ABI_SIMPLE_FUN_APPBRAND_MOUNT_JS;                      // 小程序 挂载脚本方法名字
    public String ABI_CLS_APPBRAND_MOUNT_JS_PARMA0;                      // 小程序 挂载脚本参数0
    public String ABI_CLS_APPBRAND_MOUNT_JS_PARMA3;                      // 小程序 挂载脚本参数3

    /**
     * 配置 Js脚本挂载
     */
    protected abstract void initAppBrandMountJs();

    // 获取小程序WebView实例,用于注入
    public String ABI_CLS_APPBRAND_WEBVIEW_CLIENT0;                      // 第一个 WebView实例获取的类
    public String ABI_FIELD_APPBRAND_WEBVIEW_CLIENT0;                    // 获取实例要访问的字段
    public String ABI_CLS_APPBRAND_WEBVIEW_CLIENT1;                      // 第二个 WebView实例获取的类
    public String ABI_SIMPLE_FUN_APPBRAND_WEBVIEW_CLIENT1;               // 获取实例访问的方法

    /**
     * 配置 WebView实例获取
     */
    protected abstract void initAppBrandWebViewClient();
    // 小程序注入

    public String ABI_CLS_APPBRAND_WEBVIEW_INJECT;                      // 小程序注入类
    public String ABI_SIMPLE_FUN_APPBRAND_WEBVIEW_EVALUATEJAVASCRIPT;   // 小程序4.4以上注入脚本方法名

    protected abstract void initAppBrandWebViewInject();
}
