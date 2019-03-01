-optimizationpasses 6
-dontusemixedcaseclassnames
-ignorewarnings
-keepattributes *Annotation*,EnclosingMethod,InnerClasses,Signature,Exceptions
-optimizations code/removal/simple,code/removal/advanced
-dontpreverify
-ignorewarnings                # 抑制警告
-dontoptimize

## 入口
-keep class  com.google.** {*;}
##-keep class  androidx.** {*;}
-keep class  android.** {*;}
-keep class  org.** {*;}