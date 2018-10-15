package com.my.hook.utils.xposed;


import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

public class UtilsTool {

    private UtilsTool() {
        // no instance
    }

    public static boolean isClassValid(String packageName, String className) {
        return className.startsWith(packageName) // Only listen to package classes
                && !className.contains("BuildConfig") // Android class that isn't actually used
                && !className.startsWith(packageName + ".R$") // ^ same here
                && !className.equals(packageName + ".R"); // another one...
    }

    public static boolean isMethodValidNoAbstract(Method method) {
        return !Modifier.isAbstract(method.getModifiers());
    }
}
