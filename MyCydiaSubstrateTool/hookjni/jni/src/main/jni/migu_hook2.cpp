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
#define LOG_TAG "hook_mg_anti"
#define LOG_TAG_SUB "hook_sub_mg_anti"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

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
static int OnloadAddr = 0x21FF0; //JNI_OnLoad
static int MemCheckAddr = 0x906F0;//0x5B590;
static int ReadLR = 0x9EC + 1; //where to hook read function

//cn.qtt.caimanapp.duowanmotu - 5708
static char
		* AppStat =
		"28881 (com.xuzhourd.pb) S 24802 24802 0 0 -1 4194624 36711 1999 375 27 703 216 7 3 5 -15 25 0 9519232 596160512 14366 4294967295 1074708480 1074713612 3203042080 3203039808 1074982692 0 4612 0 38120 4294967295 0 0 17 0 0 0 0 0 0 1074720240 1074720768 1085734912";
		//"18166 (junchuangmanhua) S 15959 15959 0 0 -1 4194624 21990 0 40 0 246 95 0 0 5 -15 15 0 9316046 584499200 12512 4294967295 1074098176 1074103308 3201690400 3201688128 1074372388 0 4612 0 38120 4294967295 0 0 17 0 0 0 0 0 0 1074109936 1074110464 1084051456";
//"5708 (napp.duowanmotu) S 502 502 0 0 -1 4194624 45058 2019 1194 11 21443 2047 7 4 5 -15 32 0 10507026 672133120 13829 4294967295 1 1 0 0 0 0 4612 0 38120 4294967295 0 0 17 0 0 0 0 0 0 0 0 0";
//"18154 (m.xuzhourd.rdmh) S 4942 4942 0 0 -1 4194624 57647 2024 515 3 1585 590 7 3 5 -15 24 0 555136 600313856 11030 4294967295 1 1 0 0 0 0 4612 0 38120 4294967295 0 0 17 0 0 0 0 0 0 0 0 0";
static char* SoPath =
		//"/data/data/com.gmw.gmylh/files/libmgp_03.00.03_01.so";
		//"/data/data/cn.qtt.caimanapp.duowanmotu/files/libmgp_03.00.03_01.so";
       //"/data/data/com.xuzhourd.rdmh/files/libmgp_03.00.03_01.so";
		//"/data/data/com.lectek.junchuangmanhua/files/libmgp_03.00.03_01.so";
        "/data/data/com.xuzhourd.pb/files/libmgp_03.00.03_01.so";

int antiMemCheck = 1;
const int copyLen = 0xC6EEC;//0xFE1FB;	//soFile's length
static char soCopy[copyLen + 1];
char* soPath = "/sdcard/tmp/libmgp_03.00.03_01.so";
int isHook = 1;
int isHook_75A70 = 1;
int isHook_6C5C4 = 1;
int isHook_4EF88 = 1;
int isHook_94180 = 1;

int isHook_7098C=1;
int isHook_764B0=1;
int isHook_66B68=1;
int isHook_7139C=1;
int isHook_75638=1;
int isHook_begin=1;

int time_6C5C4=0;
int time_7098C=0;
int time_764B0=0;
int time_66B68=0;
int time_7139C=0;
int time_75638=0;

typedef struct _HookStruct {
	char SOName[SOINFO_NAME_LEN];
	char FunctionName[SOINFO_NAME_LEN];
	void *NewFunc;
	void *OldFunc;
	void *occPlace;
} HookStruct;

void (*TK_HookImportFunction)(HookStruct *pHookStruct);
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

void* (*old_724B4)(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6);
void* my_724B4(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6) {
	unsigned int lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook sub_724B4 hooked:LR:0x%x IN a4:%x a6:0x%x", lr,
			(unsigned int) (a4), (unsigned int) (a6));
	//str2hex((char *)(*(unsigned int* )(a2 + 8)), 32 , "hook 724B4_a2+8 IN");
	str2hex((char *) a4, 16, "hook 724B4_a4 IN");
	str2hex((char *) a6, 16, "hook 724B4_a6 IN");
	void * v = old_724B4(a1, a2, a3, a4, a5, a6);
	return v;
}

void* (*old_71CF4)(void* a1, void* a2, void* a3, void* a4);
void* my_71CF4(void* a1, void* a2, void* a3, void* a4) {
	unsigned int lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_71CF4 hooked:LR:0x%x IN a1:%x a2:0x%x,a3:%x a4:0x%x",
			lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3),
			(unsigned int) (a4));
	str2hex((char *) a4, 32, "hook_mg_71CF4_a4_32 IN");
	void * v = old_71CF4(a1, a2, a3, a4);
	str2hex((char *) a1, 32, "hook_mg_71CF4_a1_32 OUT");
	return v;
}

void* (*old_75A70)(void* a1, void* a2);
void* my_75A70(void* a1, void* a2) {
	unsigned int lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_75A70 hooked:LR:0x%x IN a2:%x a2+12:0x%x", lr,
			*(unsigned int*) (a2), *(unsigned int*) (a2 + 12));
	str2hex((char *) (*(unsigned int*) (a2 + 12)), 32, "hook_mg_75A70_a2+12 IN");
	void * v = old_75A70(a1, a2);
	str2hex((char *) (*(unsigned int*) (a2 + 12)), 32,
			"hook_mg_75A70_a2+12 OUT");
	return v;
}

void* (*old_6C5C4)(void* a1, void* a2, void* a3, void* a4);
void* my_6C5C4(void* a1, void* a2, void* a3, void* a4) {
	unsigned int lr;
	GETLR(lr);
	time_6C5C4++;
	__android_log_print(ANDROID_LOG_INFO, "native_mg_free_time_6C5C4 ",
						"time_6C5C4: excute_%d",time_6C5C4);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_6C5C4 hooked:LR:0x%x IN a1:%x a2:0x%x a3:%x a4:0x%x ",
			lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3),
			(unsigned int) (a4));
	str2hex((char *) a3, 16, "hook_mg_6C5C4_a3 IN");
	str2hex((char *) (*(unsigned int*)(*(unsigned int *)(a1 + 4) + 4)), 16,
			"hook_mg_6C5C4_(char *)(*(a1+4)+4) IN");
	void * v = old_6C5C4(a1, a2, a3, a4);
	str2hex((char *) a3, 16, "hook_mg_6C5C4_a3 OUT");
	return v;
}

void* (*old_4EF88)(void* a1);
void* my_4EF88(void* a1) {
	unsigned int lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_4EF88 hooked:LR:0x%x IN a1:%x", lr,
			*(unsigned int*) (a1));
	void * v = old_4EF88(a1);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_4EF88 hooked:LR:0x%x OUT result:%x", lr,
			*(unsigned int*) (v));
	return v;
}

void* (*old_94180)(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6,
		void* a7, void* a8);
void* my_94180(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6,
		void* a7, void* a8) {
	unsigned int lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook sub_94180 hooked:LR:0x%x IN a1:0x%x a2:0x%x a7:0x%x", lr,
			(unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a7));
	str2hex((char *) a1, (int) (a2), "hook 94180_a1 IN");
	str2hex((char *) a3, (int) (a4), "hook 94180_a3 IN");
	str2hex((char *) a5, (int) (a6), "hook 94180_a5 IN");
	void * v = old_94180(a1, a2, a3, a4, a5, a6, a7, a8);
	str2hex((char *) a7, 32, "hook 94180_a7 OUT");
	return v;
}

void* (*oldmemCheck)(void * a1, void* a2, void* a3);
void* mymemCheck(void * a1, void* a2, void* a3) {
	int add = a1;
	if (a1 >= baseAdd - 1 && a1 <= (baseAdd - 1 + copyLen))
		add = a1 - ((int) baseAdd - 1) + (int) soCopy;
	return oldmemCheck(add, a2, a3);
}

void* (*old_7098C)(void* a1, void* a2, void* a3, void* a4);
void* my_7098C(void* a1, void* a2, void* a3, void* a4) {
	unsigned int lr;
	GETLR(lr);
	time_7098C++;
	__android_log_print(ANDROID_LOG_INFO, "native_mg_free_time_7098C ",
						"time_7098C: excute_%d",time_7098C);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_7098C hooked:LR:0x%x IN a1:%x a2:0x%x a3:%x a4:0x%x ",
			lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3),
			(unsigned int) (a4));
	str2hex((char *) a1, 32, "hook_mg_7098C_a1 IN");
	str2hex((char *) a2, 32, "hook_mg_7098C_a2 IN");
	str2hex((char *) a3, 32, "hook_mg_7098C_a3 IN");
	void * v = old_7098C(a1, a2, a3, a4);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
				"hook_mg_sub_7098C hooked:LR:0x%x OUT a1:%x a2:0x%x a3:%x a4:0x%x ",
				lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3),
				(unsigned int) (a4));
	return v;
}

void* (*old_764B0)(void* a1, void* a2, void* a3, void* a4, void* a5);
void* my_764B0(void* a1, void* a2, void* a3, void* a4,void* a5) {
	unsigned int lr;
	GETLR(lr);
	time_764B0++;
	__android_log_print(ANDROID_LOG_INFO, "native_mg_free_time_764B0 ",
						"time_764B0: excute_%d",time_764B0);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_764B0 hooked:LR:0x%x IN a1:%x a2:0x%x a3:%x a4:0x%x a5:0x%x",
			lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3),
			(unsigned int) (a4),(unsigned int) (a5));
	str2hex((char *) a1, 32, "hook_mg_764B0_a1 IN");
	str2hex((char *)(*(unsigned int*)a1), 32, "hook_mg_764B0_*a1 IN");
	str2hex((char *) a2, 32, "hook_mg_764B0_a2 IN");
	str2hex((char *) a4, 64, "hook_mg_764B0_a4 IN");
	str2hex((char *) a5, 32, "hook_mg_764B0_a5 IN");
	void * v = old_764B0(a1, a2, a3, a4, a5);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
				"hook_mg_sub_764B0 hooked:LR:0x%x OUT a1:%x a2:0x%x a3:%x a4:0x%x, a5:0x%x",
				lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3),
				(unsigned int) (a4),(unsigned int) (a5));
	return v;
}

void* (*old_66B68)(void* a1, void* a2, void* a3);
void* my_66B68(void* a1, void* a2, void* a3) {
	unsigned int lr;
	GETLR(lr);
	time_66B68++;
	__android_log_print(ANDROID_LOG_INFO, "native_mg_free_time_66B68 ",
						"time_66B68: excute_%d",time_66B68);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_66B68 hooked:LR:0x%x IN a1:%x a2:0x%x[%s] a3:%x",
			lr, (unsigned int) (a1), (unsigned int) (a2), a2, (unsigned int) (a3));
	str2hex((char *) a1, 32, "hook_mg_66B68_a1 IN");
	str2hex((char *) a2, 32, "hook_mg_66B68_a2 IN");
	str2hex((char *) a3, 32, "hook_mg_66B68_a3 IN");
	void * v = old_66B68(a1, a2, a3);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
				"hook_mg_sub_66B68 hooked:LR:0x%x OUT a1:%x a2:0x%x a3:%x",
				lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3)
			);
	str2hex((char *) a1, 32, "hook_mg_66B68_a1 OUT");
	str2hex((char *) a2, 32, "hook_mg_66B68_a2 OUT");
	str2hex((char *) a3, 32, "hook_mg_66B68_a3 OUT");
	return v;
}


void* (*old_7139C)(void* a1, void* a2, void* a3);
void* my_7139C(void* a1, void* a2, void* a3) {
	unsigned int lr;
	GETLR(lr);
	time_7139C++;
	__android_log_print(ANDROID_LOG_INFO, "native_mg_free_time_7139C ",
						"time_7139C: excute_%d",time_7139C);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_7139C hooked:LR:0x%x IN a1:%x a2:0x%x[%s] a3:%x[%s]",
			lr, (unsigned int) (a1), (unsigned int) (a2), a2, (unsigned int) (a3),a3);
	str2hex((char *) a1, 32, "hook_mg_7139C_a1 IN");
	str2hex((char *) a2, 32, "hook_mg_7139C_a2 IN");
	str2hex((char *) a3, 64, "hook_mg_7139C_a3 IN");
	void * v = old_7139C(a1, a2, a3);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
				"hook_mg_sub_7139C hooked:LR:0x%x OUT a1:%x a2:0x%x a3:%x",
				lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3)
			);
	str2hex((char *) a1, 32, "hook_mg_7139C_a1 OUT");
	str2hex((char *) a2, 32, "hook_mg_7139C_a2 OUT");
	str2hex((char *) a3, 64, "hook_mg_7139C_a3 OUT");
	return v;
}


void* (*old_75638)(void* a1, void* a2, void* a3, void* a4);
void* my_75638(void* a1, void* a2, void* a3, void* a4) {
	unsigned int lr;
	GETLR(lr);
	time_75638++;
	__android_log_print(ANDROID_LOG_INFO, "native_mg_free_time_75638 ",
						"time_75638: excute_%d",time_75638);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
			"hook_mg_sub_75638 hooked:LR:0x%x IN a1:%x a2:0x%x a3:%x a4:0x%x",
			lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3),
			(unsigned int) (a4));
	str2hex((char *) a1, 32, "hook_mg_75638_a1 IN");
	//str2hex((char *) a2, 32, "hook_mg_75638_a2 IN");
	//str2hex((char *) a3, 32, "hook_mg_75638_a3 IN");
	str2hex((char *) a4, 64, "hook_mg_75638_a4 IN");
	void * v = old_75638(a1, a2, a3, a4);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG_SUB,
				"hook_mg_sub_75638 hooked:LR:0x%x OUT a1:%x a2:0x%x a3:%x a4:0x%x ",
				lr, (unsigned int) (a1), (unsigned int) (a2), (unsigned int) (a3),
				(unsigned int) (a4));
	return v;
}

void baseAddressFunc() {
	if (isSoLoaded)
		return;
	void * handle = dlopen(SoPath, RTLD_NOW); //com.joemusic JOEmusic
	if (handle != NULL) {
		void* Function = dlsym(handle, "JNI_OnLoad");
		baseAdd = Function - OnloadAddr; // 对应版本so的jni_onload函数的地址
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "baseAdd:0x%X", baseAdd);

		if (antiMemCheck) {
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "antidebug in");

			memset(soCopy, 0, copyLen + 1);
			int fd = open(soPath, O_RDONLY, 0666);
			int len = read(fd, soCopy, 0xC6EEC);//0xCCB28	//0xCCB28 equals to copyLen's value
			LOGI("read so file len:0x%x", len);
			TK_InlineHookFunction((void*) (baseAdd + MemCheckAddr - 1),
					&mymemCheck, &oldmemCheck); //how to find 0x906F0:please read readme.doc
			antiMemCheck = 0;
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "antidebug out");
			isSoLoaded = 1;
		}
	} else
		__android_log_print(ANDROID_LOG_INFO, LOG_TAG,
				"libmgp_03.00.03_01.so not found");
	return;
}


int (*oldwrite)(int fd, const void * buf, size_t count);
int mywrite(int fd, const void * buf, size_t count) {
	unsigned lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, "native_hook_mg_c_write",
			"fd:%d buf[0x%x] count:%d dress[0x%x]", fd, buf, count, lr);
	str2hex((char *) buf, count, "hook_mg_write_default_data IN");
	return oldwrite(fd, buf, count);
}

int (*oldfwrite)(const void* buffer, size_t size, size_t count, void* stream);
int myfwrite(const void* buffer, size_t size, size_t count, void* stream) {
	unsigned lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, "native_mg_hook_c_fwrite",
			" FILE-stream:0x%x buffer[0x%x] size:%d count:%d dress[0x%x]",
			stream, buffer, size, count, lr);
	str2hex((char *) buffer, size, "hook_mg_fwrite_default IN");
	return oldfwrite(buffer, size, count, stream);
}

int (* oldread)(int handle, void *buf, int nbyte);
int myread(int handle, void *buf, int nbyte) {
	unsigned lr;
	GETLR(lr);
	static char statBuf[256] = { 0 };
	static int ret = 0;
	if ((lr & 0xFFF) != ReadLR) { ////how to find 0x9ED:please read readme.doc
		int read = oldread(handle, buf, nbyte);
		__android_log_print(ANDROID_LOG_INFO, "native_mg_read",
				"handle:%d buf[0x%x]:%.*s nbyte:%u dress[0x%x]", handle, buf,
				read, buf, nbyte, lr);
		return read;
	} else {
		static char* tmp = AppStat;
		//how to find tmp:please read readme.doc
		ret = 197;
		memcpy(buf, tmp, ret);
		return ret;
	}
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
		__android_log_print(ANDROID_LOG_INFO, "native_mg_gettimeofday",
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
	__android_log_print(ANDROID_LOG_INFO, "native_mg_strlen",
			"Addr[0x%x] str[0x%x]:%s", lr, str, str);
	if (strcmp(str, "MOENRAKQTY") == 0 && isHook_75A70 == 1) {
		__android_log_print(ANDROID_LOG_INFO, "native_mg_strlen_sub_75A70",
				"hook_mg_sub_75A70");
		//TK_ARM_INLINEHOOK(75A70);
		isHook_75A70 = 0;
		if (isHook_94180 == 1) {
			__android_log_print(ANDROID_LOG_INFO, "native_mg_strlen_sub_94180",
					"hook_mg_sub_94180");
			//TK_ARM_INLINEHOOK(94180);
			isHook_94180 = 0;
		}
	}

	if (lr == (baseAdd + 0x5C556) && baseAdd > 0 && isHook_begin==0) {
			__android_log_print(ANDROID_LOG_INFO, "native_mg_strlen_5C556",
					"Addr[0x%x] strlen[0x%x]:%s", lr, str, str);
			if (isHook_7139C == 1) {
				__android_log_print(ANDROID_LOG_INFO, "native_mg_strlen_sub_7139C",
						"hook_mg_sub_7139C");
				TK_ARM_INLINEHOOK(7139C);
				isHook_7139C = 0;
			}

			if (isHook_75638 == 1) {
				 __android_log_print(ANDROID_LOG_INFO, "native_mg_strlen_sub_75638",
						"hook_mg_sub_75638");
				TK_ARM_INLINEHOOK(75638);
				isHook_75638 = 0;
		   }
	}

	if (lr == (baseAdd + 0x5C556) && baseAdd > 0 && isHook_begin==0) {
			__android_log_print(ANDROID_LOG_INFO, "native_mg_strlen_5C556",
						"Addr[0x%x] strlen[0x%x]:%s", lr, str, str);
			if (isHook_66B68 == 1) {
				__android_log_print(ANDROID_LOG_INFO, "native_mg_strlen_sub_66B68",
						"hook_mg_sub_66B68");
				//TK_ARM_INLINEHOOK(66B68);
				isHook_66B68 = 0;
		}

	/*		if (isHook_6C5C4 == 1) {
						__android_log_print(ANDROID_LOG_INFO, "native_mg_strlen_sub_6C5C4",
								"hook_mg_sub_6C5C4");
						TK_ARM_INLINEHOOK(6C5C4);
						isHook_6C5C4 = 0;
			}*/

			if (isHook == 1) {
				//TK_ARM_INLINEHOOK(71CF4); //打印第三个参数
				isHook = 0;
			}
	}
	return oldstrlen(str);
}

void (*oldfree)(void *ptr);
void myfree(void *ptr) {
	unsigned lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, "native_mg_free",
			"Addr[0x%x] free[0x%x]:%s", lr, ptr, ptr);
	str2hex((char *)ptr, 32, "hook_mg_free_default_data IN");

	if (lr == (baseAdd + 0x70BC8 - 1) && baseAdd > 0) {
		str2hex((char *) ptr, 16, "hook_mg_70BC8_free data");
	}
	if (lr == (baseAdd + 0x70D50 - 1) && baseAdd > 0) {
		str2hex((char *) ptr, 16, "hook_mg_70D50_free data");
	}
	//
	if (lr == (baseAdd + 0x720F4 - 1) && baseAdd > 0) {
		str2hex((char *) ptr, 32, "hook_mg_720F4_free data");
	}
	if (time_6C5C4 == 3) {
		__android_log_print(ANDROID_LOG_INFO, "native_mg_free_time_6C5C4 ",
					"time_6C5C4 :3");

		//72D28
		if (lr == (baseAdd + 0x72D28 - 1) && baseAdd > 0) {
			str2hex((char *) ptr, 32, "hook_mg_72D28_free data");
		}
		//6844C
		if (lr == (baseAdd + 0x6844C - 1) && baseAdd > 0) {
			str2hex((char *) ptr, 8, "hook_mg_6844C_free data");
		}
		//68444
		if (lr == (baseAdd + 0x68444 - 1) && baseAdd > 0) {
			str2hex((char *) ptr, 104, "hook_mg_68444_free data");
		}
	}

	//72D27
	if (lr == (baseAdd + 0x72D27 - 1) && baseAdd > 0) {
		__android_log_print(ANDROID_LOG_INFO, "native_mg_free_72D27",
					"Addr[0x%x] free[0x%x]:%s", lr, ptr, ptr);
		if (isHook_764B0 == 1) {
			__android_log_print(ANDROID_LOG_INFO, "native_mg_free_sub_764B0",
					"hook_mg_sub_764B0");
			//TK_ARM_INLINEHOOK(764B0);
			isHook_764B0 = 0;
		}
	}
	return oldfree(ptr);
}

FILE * (*oldfopen)(const char* path, const char * mode);
FILE * myfopen(const char* path, const char * mode) {
	unsigned lr;
	GETLR(lr);
	FILE * f = oldfopen(path, mode);
	__android_log_print(ANDROID_LOG_INFO, "native_mg_fopen",
			"File:0x%x path[0x%x]:%s mode:%s dress[0x%x]", (unsigned int) f,
			path, path, mode, lr);
	return f;
}

void* (*oldmalloc)(unsigned int num_bytes);
void* mymalloc(unsigned int num_bytes) {
	unsigned lr;
	GETLR(lr);
	void* tmp = oldmalloc(num_bytes);
	__android_log_print(ANDROID_LOG_INFO, "native_mg_malloc",
			"malloc[0x%x] num_bytes:%u dress[0x%x]", tmp, num_bytes, lr);
	if (lr == (baseAdd + 0x72338 - 1) && baseAdd > 0) {
		__android_log_print(ANDROID_LOG_INFO, "native_mg_malloc_72338",
				"malloc[0x%x] num_bytes:%u dress[0x%x]", tmp, num_bytes, lr);
/*		if (isHook == 1) {
		    TK_ARM_INLINEHOOK(71CF4); //打印第三个参数
			isHook = 0;
		}*/
	}

	if (lr == (baseAdd + 0x765B0 - 1) && baseAdd > 0) {
		__android_log_print(ANDROID_LOG_INFO, "native_mg_malloc_765B0",
				"malloc[0x%x] num_bytes:%u dress[0x%x]", tmp, num_bytes, lr);
/*		if (isHook_6C5C4 == 1) {
			__android_log_print(ANDROID_LOG_INFO, "native_mg_malloc_sub_6C5C4",
					"hook_mg_sub_6C5C4");
			TK_ARM_INLINEHOOK(6C5C4);
			isHook_6C5C4 = 0;
		}*/
	}

	//66688
	if (lr == (baseAdd + 0x66688 - 1) && baseAdd > 0 && isHook_begin==0) {
		__android_log_print(ANDROID_LOG_INFO, "native_mg_malloc_66688",
				"malloc[0x%x] num_bytes:%u dress[0x%x]", tmp, num_bytes, lr);
		if (isHook_7098C == 1) {
			__android_log_print(ANDROID_LOG_INFO, "native_mg_malloc_sub_7098C",
					"hook_mg_sub_7098C");
			//TK_ARM_INLINEHOOK(7098C);
			isHook_7098C = 0;
		}

		if (isHook_764B0 == 1) {
				__android_log_print(ANDROID_LOG_INFO, "native_mg_free_sub_764B0",
							"hook_mg_sub_764B0");
				//TK_ARM_INLINEHOOK(764B0);
				isHook_764B0 = 0;
		}
	}
	return tmp;
}

int (* oldsprintf)(char *str, const char *fmt, ...);
int mysprintf(char *str, const char *fmt, void* arg1, void* arg2, void* arg3,
		void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9,
		void* arg10, void* arg11, void* arg12, void* arg13, void* arg14,
		void* arg15, void* arg16, void* arg17, void* arg18, void* arg19,
		void* arg20, void* arg21, void* arg22, void* arg23, void* arg24) {
	unsigned lr;
	GETLR(lr);
	int ret = oldsprintf(str, fmt, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
			arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17,
			arg18, arg19, arg20, arg21, arg22, arg23, arg24);
	__android_log_print(ANDROID_LOG_INFO, "native_mg_sprintf_fmt",
			"Addr[0x%x] fmt[0x%x]:%s str[0x%x]:%s", lr, fmt, fmt, str, str);
	return ret;
}

void *(*oldmemcpy)(void *dest, const void *src, size_t n);
void *mymemcpy(void *dest, const void *src, size_t n) {
	unsigned lr;
	GETLR(lr);
	__android_log_print(ANDROID_LOG_INFO, "native_mg_memcpy",
			"dest[0x%x] src[0x%x]:%.*s size:%d dress[0x%x]", dest, src, n, src,
			n, lr);
	return oldmemcpy(dest, src, n);
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

myHookStruct myHookLibc[] = { HOOKEXPORT(read,MODE_EXPORT),
		HOOKEXPORT(gettimeofday,MODE_EXPORT), HOOKEXPORT(free, MODE_EXPORT),
		HOOKEXPORT(strlen, MODE_EXPORT), HOOKEXPORT(sprintf, MODE_EXPORT),
		HOOKEXPORT(malloc, MODE_EXPORT), HOOKEXPORT(fopen, MODE_EXPORT),
       //HOOKEXPORT(memcpy, MODE_EXPORT),
		HOOKEXPORT(fwrite, MODE_EXPORT),
		HOOKEXPORT(write, MODE_EXPORT)
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
	__android_log_print(ANDROID_LOG_INFO, "mg_anti_clazz",
			"Hooked into the application.");

	jmethodID onCreate = jniPtr->GetMethodID(clazz, "onCreate", "()V");
	if (onCreate != NULL) {
		__android_log_print(ANDROID_LOG_ERROR, "mg_anti_OnClazzLoad", "find");
		MSJavaHookMethod(jniPtr, clazz, onCreate, &newonCreate, &oldonCreate);
	} else
		__android_log_print(ANDROID_LOG_ERROR, "mg_anti_OnClazzLoad",
				"not find");
	MSJavaHookClassLoad(NULL, "java/lang/StringBuilder", &onString);
}

MSInitialize
{
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Initialize start");
	  //MSJavaHookClassLoad(NULL, "com/xuzhourd/rdmh/RdmhApplication", &OnClazzLoad);

	MSJavaHookClassLoad(NULL, "com/xuzhourd/rdmh/RdmhApplication", &OnClazzLoad);
	//MSJavaHookClassLoad(NULL, "com/lectek/junchuangmanhua/MiguApplication", &OnClazzLoad);
	//MSJavaHookClassLoad(NULL, "com/gmw/gmylh/MusicApplication", &OnClazzLoad);
	//MSJavaHookClassLoad(NULL, "cn/qtt/caimanapp/duowanmotu/MiguApplication", &OnClazzLoad);
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Initialize end");

}
