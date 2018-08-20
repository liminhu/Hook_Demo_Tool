#include <android/log.h>
#include "substrate.h"
#include <sys/stat.h> 
#include <unistd.h> 
#include <memory.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <jni.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/inotify.h>

#include<stdio.h>
#include <string.h>
#include <jni.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <pthread.h>
#include <dirent.h>
#include "substrate.h"

#ifndef __ASM_SH_LOCAL_H
#define __ASM_SH_LOCAL_H
#endif
#define LOG_TAG "hook_my_"
#define LOG_TAG_SUB "hook_sub_my_"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)


static int  recode=0;

MSConfig(MSFilterExecutable, "/system/bin/app_process")

#define GETLR(store_lr)	\
	__asm__ __volatile__(	\
		"mov %0, lr\n\t"	\
		:	"=r"(store_lr)	\
	)

#define SOINFO_NAME_LEN 128
#define TK_INLINEHOOK(addr) TK_InlineHookFunction((void*)(baseAdd + 0x ## addr),(void*)&my_ ## addr,(void **)&old_ ## addr);
#define TK_ARM_INLINEHOOK(addr) TK_InlineHookFunction((void*)((unsigned int)baseAdd + 0x ## addr -1),(void*)&my_ ## addr,(void **)&old_ ## addr);

#define MODE_EXPORT 1
#define MODE_INLINE 2
#define MODE_IMPORT 3
#define HOOKEXPORT(FUNCNAME,MODE) {#FUNCNAME,(void*)my ## FUNCNAME,(void**)&old ## FUNCNAME ,MODE}

typedef struct {
	const char* FunctionName;
	void* myfunction;
	void** oldfunction;
	int mode;
}myHookStruct;


static void* baseAdd = 0;
static int isSoLoaded = 0;

/*
//庞然巨物
static int OnloadAddr =0x46EB00; // 0x184420;  //JNI_OnLoad
static char*SoPath="/data/data/com.halfbrick.colossatron/lib/libmortargame.so";
*/

//static int OnloadAddr =0x184420; // 0x184420;  //JNI_OnLoad
//static char*SoPath="/data/data/com.halfbrick.colossatron/lib/libgame.so";


static int OnloadAddr =0x245424;
static char*SoPath="/data/data/com.bistudio.minidayz.st/lib/libxwalkcore.so";



int antiMemCheck = 1;


const int copyLen = 0xA0EF4; //0xF1F34;
static char soCopy[copyLen + 1];

typedef struct _HookStruct {
	char SOName[SOINFO_NAME_LEN];
	char FunctionName[SOINFO_NAME_LEN];
	void *NewFunc;
	void *OldFunc;
	void *occPlace;
} HookStruct;

typedef struct _Sub_9122C_Struct {
	int data_a0;
	int a1;
	int data_a2;
	char *data_a3;
	int a3_len;
	int a6_len;
	char* keyword_data_a6;
	int a7;
	int a8;
	int a9;
	int a10;
	int a11;
	int a12;
	int a13;
	int a14;
	int a15;
	int a16;
	int a18_len;
	char *a18_verify; //存校验的结果值
	char *nextAddress;
} Sub_9122C_Struct;

int (*TK_HookImportFunction)(HookStruct *pHookStruct);
int (*TK_HookExportFunction)(HookStruct *pHookStruct);
int (*TK_InlineHookFunction)(void *TargetFunc, void *NewFunc, void** OldFunc);

int init_TKHookFunc() {
	void * handle = dlopen(
			"/data/data/com.hu.anti.dmxw.data/lib/libTKHooklib.so", RTLD_NOW);
	if (handle != NULL) {
		TK_HookExportFunction = dlsym(handle, "TK_HookExportFunction");
		TK_InlineHookFunction = dlsym(handle, "TK_InlineHookFunction");
		TK_HookImportFunction = dlsym(handle, "TK_HookImportFunction");
		if (TK_HookImportFunction != NULL)
			return 1;
	}
	return 0;
}

int str2hex(char *str, int j, char * parameter) {
	int i;
	// char* str = "我们的爱";
	if (str == NULL) {
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
				"sub_str2hex_%s hooked dress:[0x%s] len:[%d]  \n", parameter,
				"error", j);
		return 0;
	}
	if (strlen(str) > 0) {
		char res[1025];
		if (j > 512) {
			j = 512;
		}
		for (i = 0; i < j; i++) {
			int a = *(str + i) >> 4 & 0x0F;
			int b = *(str + i) & 0x0F;
			if (a > 9) {
				a = a + 0x37;
			} else {
				a = a + 0x30;
			}
			if (b > 9) {
				b = b + 0x37;
			} else {
				b = b + 0x30;
			}
			res[2 * i] = a;
			res[2 * i + 1] = b;
		}
		res[2 * i] = '\0';
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
				"sub_str2hex_%s hooked dress:[0x%x] len:[%d] Hex:[%s] \n",
				parameter, str, j, res);
	}else{
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
					"sub_str2hex_%s hooked dress:[0x%x] reaal_len:[0] \n",
					parameter, str);
	}
}


void* (*oldmemCheck)(void * a1, void* a2, void* a3);
void* mymemCheck(void * a1, void* a2, void* a3) {
	int add = a1;
	if (a1 >= baseAdd - 1 && a1 <= (baseAdd - 1 + copyLen))
		add = a1 - ((int) baseAdd - 1) + (int) soCopy;
	return oldmemCheck(add, a2, a3);
}


void baseAddressFunc() {
	if (isSoLoaded)
		return;
	void * handle = dlopen(SoPath, RTLD_NOW); //com.joemusic JOEmusic
	if (handle != NULL) {
		void* Function = dlsym(handle, "JNI_OnLoad");
		baseAdd = Function - OnloadAddr; // 对应版本so的jni_onload函数的地址
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "baseAdd:0x%X", baseAdd);
		isSoLoaded = 1;
	} else
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,
				"so not found");
	return;
}

int (*oldwrite)(int fd, const void * buf, size_t count);
int mywrite(int fd, const void * buf, size_t count) {
	unsigned lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, "native_hook_my_c_write",
			"fd:%d buf[0x%x] count:%d dress[0x%x-0x%x]", fd, buf, count, lr, lr-(int)baseAdd);
	str2hex((char *) buf, count, "hook_my_write_default_data IN");
	return oldwrite(fd, buf, count);
}

int (*oldfwrite)(const void* buffer, size_t size, size_t count, void* stream);
int myfwrite(const void* buffer, size_t size, size_t count, void* stream) {
	unsigned lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, "native_my_hook_c_fwrite",
			" FILE-stream:0x%x buffer[0x%x] size:%d count:%d dress[0x%x-0x%x]",
			stream, buffer, size, count, lr, lr-(int)baseAdd);
	str2hex((char *) buffer, size, "hook_my_fwrite_default IN");
	return oldfwrite(buffer, size, count, stream);
}

int (* oldread)(int handle, void *buf, int nbyte);
int myread(int handle, void *buf, int nbyte) {
	unsigned lr;
	GETLR(lr);
	static char statBuf[256] = { 0 };
	static int ret = 0;
     int read = oldread(handle, buf, nbyte);
		__android_log_print(ANDROID_LOG_INFO, "native_my_read",
				"handle:%d buf[0x%x]:%.*s nbyte:%u dress[0x%x-0x%x]", handle, buf,
				read, buf, nbyte, lr, lr-(int)baseAdd);
	return read;

}

int (* oldgettimeofday)(struct timeval*tv, struct timezone *tz);
int mygettimeofday(struct timeval*tv, struct timezone *tz) {
	unsigned lr;
	GETLR(lr);
	static long sec = 0;
	static long usec = 0;
	if (sec == 0) {
		int ret = oldgettimeofday(tv, tz);
		sec = tv->tv_sec;
		usec = tv->tv_usec;
		baseAddressFunc();
		__android_log_print(ANDROID_LOG_INFO, "native_my_gettimeofday",
				"tv_sec:%ld,tv_usec:%ld,dress[0x%x]", tv->tv_sec, tv->tv_usec,
				lr);
		return ret;
	} else {
		tv->tv_sec = sec;
		tv->tv_usec = usec;
		return 0;
	}
}

int (* oldstrlen)(const char *str);
int mystrlen(const char *str) {
	unsigned lr;
	GETLR(lr);
	baseAddressFunc();
	__android_log_print(ANDROID_LOG_INFO, "native_my_strlen",
			"Addr[0x%x-0x%x] str[0x%x]:%s", lr, lr-(int)baseAdd, str, str);
/*	if (lr == (baseAdd + 0x33810) && baseAdd > 0) {
		__android_log_print(ANDROID_LOG_INFO, "native_my_sub_9122C",
				"hook sub_9122C ");
		TK_ARM_INLINEHOOK(9122C);
		TK_ARM_INLINEHOOK(B4828);
		TK_ARM_INLINEHOOK(B4688);

       // TK_INLINEHOOK(35C70);
       // TK_INLINEHOOK(8EB0);
    	//TK_ARM_INLINEHOOK(9C24C);
    	//TK_ARM_INLINEHOOK(A0E34);
	}*/
	return oldstrlen(str);
}

void (*oldfree)(void *ptr);
void myfree(void *ptr) {
	unsigned lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, "native_my_free",
			"Addr[0x%x-0x%x] free[0x%x]:%s recode[%d]", lr, lr-(int)baseAdd,ptr, ptr, recode++);
	return oldfree(ptr);
}

FILE * (*oldfopen)(const char* path, const char * mode);
FILE * myfopen(const char* path, const char * mode) {
	unsigned lr;
	GETLR(lr);
	FILE * f = oldfopen(path, mode);
	__android_log_print(ANDROID_LOG_INFO, "native_my_fopen",
			"File:0x%x path[0x%x]:%s mode:%s dress[0x%x-0x%x]", (unsigned int) f,
			path, path, mode, lr,lr-(int)baseAdd);
	return f;
}


void* (*oldmalloc)(unsigned int num_bytes);
void* mymalloc(unsigned int num_bytes) {
	unsigned lr;
	GETLR(lr);
	void* tmp = oldmalloc(num_bytes);
	__android_log_print(ANDROID_LOG_INFO, "native_my_malloc",
			"malloc[0x%x] num_bytes:%u dress[0x%x-0x%x] recode[%d]", tmp, num_bytes, lr, lr-(int)baseAdd, recode++);
	return tmp;
}

int (* oldsprintf)(char *str, const char *fmt, ...);
int mysprintf(char *str, const char *fmt, void* arg1, void* arg2, void* arg3,
		void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9,
		void* arg10, void* arg11, void* arg12, void* arg13, void* arg14,
		void* arg15, void* arg16, void* arg17, void* arg18, void* arg19,
		void* arg20, void* arg21, void* arg22, void* arg23, void* arg24, void* arg25, void* arg26, void* arg27, void* arg28,void* arg29, void* arg30, void* arg31, void* arg32) {
	unsigned lr;
	GETLR(lr);
	if(baseAdd>0 && lr==(0x20f7c+baseAdd)){
	__android_log_print(ANDROID_LOG_INFO, "native_my_sprintf_fmt-IN-0x20f7c",
			"Addr[0x%x-0x%x] fmt[0x%x]:%s str[0x%x]:%s a12[0x%x]-a13[0x%x]-a28[%x:%s]", lr, lr-(int)baseAdd, fmt, fmt, str, str,(int)arg12, (int )arg13,(int)arg28-(int)baseAdd,(char *)arg28);
	}
	int ret = oldsprintf(str, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
			arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17,
			arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25, arg26, arg27, arg28,  arg29, arg30, arg31, arg32);
	__android_log_print(ANDROID_LOG_INFO, "native_my_sprintf_fmt-after",
			"Addr[0x%x-0x%x] fmt[0x%x]:%s str[0x%x]:%s", lr, lr-(int)baseAdd, fmt, fmt, str, str);

	//if(baseAdd>0 && strlen((char*)(baseAdd+0xF29CD))>0){
		//	__android_log_print(ANDROID_LOG_INFO, "native_my_sprintf_0xF29CD",
		//				"0xF29CE[%s]  dress[0x%x-0x%x] rec
      //de[%d]", (char *)(baseAdd+0xF29CD), lr, lr-(int)baseAdd, recode++);
	//}
	return ret;
}

void *(*oldmemcpy)(void *dest, const void *src, size_t n);
void *mymemcpy(void *dest, const void *src, size_t n) {
	unsigned lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, "native_my_memcpy",
			"dest[0x%x] src[0x%x]:%.*s size:%d dress[0x%x-0x%x]", dest, src, n, src,
			n, lr,lr-(int)baseAdd);
	//if(baseAdd>0 && strlen((char*)(baseAdd+0xF29CD))>0){
		//	__android_log_print(ANDROID_LOG_INFO, "native_my_memcpy_0xF29CD",
		//				"0xF29CE[%s]  dress[0x%x-0x%x] recode[%d]", (char *)(baseAdd+0xF29CD), lr, lr-(int)baseAdd, recode++);
	//}
	return oldmemcpy(dest, src, n);
}


void *(*oldstrncpy)(char*dest,char*src, size_t n);
void *mystrncpy(char*dest, char*src, size_t n){
	unsigned lr;
	GETLR(lr);
	if(baseAdd>0){
		__android_log_print(ANDROID_LOG_INFO, "native_my_strncpy_sub_83764",
				"dest[0x%x] src[0x%x]:%.*s size:%d dress[0x%x-0x%x]", dest, src, n, src,
				n, lr,lr-(int)baseAdd);
		sleep(60);
	}
	return oldstrncpy(dest, src, n);
}

static int time1=1;
int (*oldmemset)(void *s, int ch, unsigned n);
int mymemset(void *s, int ch, unsigned n){
	unsigned lr;
	GETLR(lr);
	baseAddressFunc();
	//__android_log_print(ANDROID_LOG_INFO, "native_hook_my_c_memset",
	return oldmemset(s, ch , n);
}

static void HookFunction(const char* FunctionName, void* myfunction,
		void** oldfunction, int mode, HookStruct* entity, MSImageRef image) {
	switch (mode) {
	case 1: {
		strcpy(entity->FunctionName, FunctionName);
		entity->NewFunc = (void*) myfunction;
		if (!TK_HookExportFunction(entity)) {
			*oldfunction = entity->OldFunc;
			goto success;
		} else {
			goto failed;
		}
	}
		break;
	case 2: {
		void * Function = MSFindSymbol(image, FunctionName);
		if (Function != NULL) {
			MSHookFunction(Function, (void*) &myfunction, (void **) oldfunction);
			goto success;
		} else {
			goto failed;
		}
	}
		break;
	}
	success: LOGI("hook %s,mode %d, success", FunctionName, mode);
	return;
	failed: LOGI("hook %s,mode %d, failed", FunctionName, mode);
	return;
}

myHookStruct myHookLibc[] = {
	//	HOOKEXPORT(read,MODE_EXPORT),
	//	HOOKEXPORT(gettimeofday,MODE_EXPORT),

		//HOOKEXPORT(free, MODE_EXPORT),
		HOOKEXPORT(strlen, MODE_EXPORT),
		//HOOKEXPORT(sprintf, MODE_EXPORT),
		//HOOKEXPORT(malloc, MODE_EXPORT),
		HOOKEXPORT(fopen, MODE_EXPORT),
       // HOOKEXPORT(memset, MODE_EXPORT),
		//HOOKEXPORT(fwrite, MODE_EXPORT),
		//HOOKEXPORT(strncpy, MODE_EXPORT),

		//HOOKEXPORT(write, MODE_EXPORT)
	    HOOKEXPORT(memcpy, MODE_EXPORT),
		//	HOOKEXPORT(time, MODE_EXPORT),
		};

static void (*oldonCreate)(JNIEnv *jni, jobject _this, ...);
static void newonCreate(JNIEnv *jni, jobject _this) {
	MSImageRef image;
	HookStruct entity;

	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "gogogo");

	strcpy(entity.SOName, "libc.so");

	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "start");
	image = MSGetImageByName("/system/lib/libc.so");

	if (image != NULL) {
		for (int i = 0; i < sizeof(myHookLibc) / sizeof(myHookStruct); i++) {
			LOGD("newOnCreate for loop:%d", i);
			HookFunction(myHookLibc[i].FunctionName, myHookLibc[i].myfunction,
					myHookLibc[i].oldfunction, myHookLibc[i].mode, &entity,
					image);
		}
	} else
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "no no no");
	oldonCreate(jni, _this);
}

static jstring (*oldToString)(JNIEnv *jni, jobject thiz, ...);
static jstring newToString(JNIEnv *jni, jobject thiz, ...) {
	jstring r = oldToString(jni, thiz);
	char* str = jni->GetStringUTFChars(r, NULL);
	__android_log_print(ANDROID_LOG_INFO, "mg_toString", "%s", str);
	jni->ReleaseStringUTFChars(r, str);
	return r;
}

static void onString(JNIEnv *jni, jclass clazz) {
	__android_log_print(ANDROID_LOG_INFO, "mg_tips",
			"Hooked into StringBuilder");
	jmethodID toString = jni->GetMethodID(clazz, "toString",
			"()Ljava/lang/String;");
	if (toString != NULL)
		MSJavaHookMethod(jni, clazz, toString, &newToString, &oldToString);
}

static void OnClazzLoad(JNIEnv *jniPtr, jclass clazz, void *data) {
	init_TKHookFunc();
	// Let user know that the class has been hooked
	LOGI("TK_HookExportFunction:0x%X", (unsigned int)TK_HookExportFunction);
	__android_log_print(ANDROID_LOG_INFO, "my_anti_clazz",
			"Hooked into the application.");

	//jmethodID onCreate = jniPtr->GetMethodID(clazz, "onCreate", "()V");
	jmethodID onCreate = jniPtr->GetMethodID(clazz, "onCreate", "(Landroid/os/Bundle;)V");  //测试用的

	if (onCreate != NULL) {
		__android_log_print(ANDROID_LOG_ERROR, "my_anti_OnClazzLoad", "find");
		MSJavaHookMethod(jniPtr, clazz, onCreate, &newonCreate, &oldonCreate);
	} else
		__android_log_print(ANDROID_LOG_ERROR, "my_anti_OnClazzLoad",
				"not find");
//	MSJavaHookClassLoad(NULL, "java/lang/StringBuilder", &onString);
}

MSInitialize
{
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Initialize start");
	//MSJavaHookClassLoad(NULL, "com/halfbrick/mortar/MortarApplication", &OnClazzLoad);
	//MSJavaHookClassLoad(NULL, "com/playrix/township/GPlayApplication", &OnClazzLoad);
	//MSJavaHookClassLoad(NULL, "com/bistudio/minidayz/st", &OnClazzLoad);

	MSJavaHookClassLoad(NULL, "com/bistudio/minidayz/MiniDAYZ", &OnClazzLoad);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Initialize end");

}
