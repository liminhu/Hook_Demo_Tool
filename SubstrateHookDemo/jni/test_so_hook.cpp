#include <android/log.h>
#include <stdio.h>
#include "substrate.h"

#define LOG_TAG "my_c_hook"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define GETLR(store_lr)	\
	__asm__ __volatile__(	\
		"mov %0, lr\n\t"	\
		:	"=r"(store_lr)	\
	)
#define SOINFO_NAME_LEN 128
#define TK_INLINEHOOK(addr) TK_InlineHookFunction((void*)(baseAdd + 0x ## addr),(void*)&my_ ## addr,(void **)&old_ ## addr);
#define TK_ARM_INLINEHOOK(addr) TK_InlineHookFunction((void*)((unsigned int)baseAdd + 0x ## addr -1),(void*)&my_ ## addr,(void **)&old_ ## addr);

typedef struct _HookStruct {
	char SOName[SOINFO_NAME_LEN];
	char FunctionName[SOINFO_NAME_LEN];
	void *NewFunc;
	void *OldFunc;
	void *occPlace;
} HookStruct;


static int OnloadAddr =0x15510;
static char*SoPath="/data/data/com.dn.test/lib/libsandbox.so";
//static char*SoPath="/data/data/com.example.gg/lib/libsandbox.so";

void cigi_hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr)
{
	MSHookFunction(orig_fcn, new_fcn, orig_fcn_ptr);
}

MSConfig(MSFilterExecutable, "/system/bin/app_process")


static void* baseAdd = 0;
static int isSoLoaded = 0;

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



void* (*old_141CC)(void* a1, void* a2, void* a3, void* a4);
void* my_141CC(void* a1, void* a2, void* a3, void* a4){
	unsigned lr;
	GETLR(lr);
	int real_add=lr-(int)baseAdd;
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG,
			"(my_141CC) hook  hooked:LR:0x%x[0x%x] IN a1:0x%x a2:%x a3:0x%s a4:0x%x ", lr,real_add,
			(unsigned int) (a1), (unsigned int) (a2), (char *) (a3), (unsigned int) (a4));
	void* ret = old_141CC(a1, a2, a3, a4);
	LOGI("(my_141CC) ret:%x", ret);
	return ret;
}



void* (*old_141EC)(void* a1);
void* my_141EC(void* a1){
	unsigned lr;
	GETLR(lr);
	int real_add=lr-(int)baseAdd;
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG,
			"(my_141EC) hook  hooked:LR:0x%x[0x%x] IN a1:0x%x  ", lr,real_add,
			(unsigned int) (a1));
	//void* ret = old_141EC(a1);
	void* ret=(int)0x67458900;
	LOGI("(my_141EC) ret:%x   --- baseAdd:%x", ret, baseAdd);
	return ret;
}

void baseAddressFunc() {
	if (isSoLoaded)
		return;
	void * handle = dlopen(SoPath, RTLD_NOW); //com.joemusic JOEmusic
	if (handle != NULL) {
		void* Function = dlsym(handle, "JNI_OnLoad");
		baseAdd = Function - OnloadAddr; // 瀵瑰簲鐗堟湰so鐨刯ni_onload鍑芥暟鐨勫湴鍧�
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "baseAdd:0x%X", baseAdd);
		isSoLoaded = 1;
	} else
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,
				"so not found");
	return;
}





static int k=1;


int (*original_getAge)(char *a1);
int replaced_getAge(char *a1) {
	unsigned  lr;
	GETLR(lr);
	LOGI("(set age )TK_ARM_INLINEHOOK  bassAdd:%x  -- lr:%x ", baseAdd, lr-(int)baseAdd);
	baseAddressFunc();
	if(baseAdd>0 && k==1){
		TK_INLINEHOOK(141EC);
		k=0;
	}

	//TK_ARM_INLINEHOOK(141CC);

	if(a1!=NULL){
		LOGI("(set age ) pragram:%s ", a1);
	}
	int ret=original_getAge(a1);
	LOGI("(set age ) ret:%d ", ret);
	return ret;
}





void* lookup_symbol(char* libraryname,char* symbolname)
{
	void *imagehandle = dlopen(libraryname, RTLD_GLOBAL | RTLD_NOW);
	if (imagehandle != NULL){
		void * sym = dlsym(imagehandle, symbolname);
		if (sym != NULL){
			return sym;
		}
		else{
			LOGI("(lookup_symbol) dlsym didn't work");
			return NULL;
		}
	}
	else{
		LOGI("(lookup_symbol) dlerror: %s",dlerror());
		return NULL;
	}
}





static void (*oldonCreate)(JNIEnv *jni, jobject _this, ...);
static void newonCreate(JNIEnv *jni, jobject _this) {
	LOGI("(lookup_symbol) oldonCreate ... ");
	void * getAgeSym = lookup_symbol(SoPath, "strlen");
	cigi_hook(getAgeSym,(void*)&replaced_getAge,(void**)&original_getAge);
	oldonCreate(jni, _this);
}



static void OnClazzLoad(JNIEnv *jniPtr, jclass clazz, void *data) {
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG,
			"Hooked into the application.");
	init_TKHookFunc();
	jmethodID onCreate = jniPtr->GetMethodID(clazz, "onCreate", "()V");

	if (onCreate != NULL) {
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "find");
		MSJavaHookMethod(jniPtr, clazz, onCreate, &newonCreate, &oldonCreate);
	} else
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG,
				"not find");
}



MSInitialize{
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Initialize start");
	MSJavaHookClassLoad(NULL, "com/example/gg/MyApp", &OnClazzLoad);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Initialize end");

}
