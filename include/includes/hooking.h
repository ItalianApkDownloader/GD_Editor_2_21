#pragma once
#include <jni.h>
#include <dlfcn.h>
#include <cstdio>
#include <pthread.h>




class HookManager {
	public:
		
		static void do_hook(void* origin, void * newfunc, void* trampoline,bool hookzz = false);
		
	template < class T>
	static void *getPointer(T value)
	{
		auto val = reinterpret_cast< void *&> (value);
		return val;
	}
	
	static void *getPointerFromSymbol(void *handle, const char *symbol)
	{	
		return reinterpret_cast<void *>(dlsym(handle, symbol));
	}
		
};

class HookWrap {
	
	public:
	
	static void nh(const char* a, void* b, void* c, bool hookzz = false) {
		

		auto cocos2d = dlopen(("libcocos2dcpp.so") != "" ? ("libcocos2dcpp.so") : NULL, RTLD_LAZY);
		HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, a), b, c);
	}
	
	static void sh(const char* a, void* b, void* c, bool hookzz = false) {

		auto cocos2d = dlopen(("libcocos2dcpp.so") != "" ? ("libcocos2dcpp.so") : NULL, RTLD_LAZY);
		HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, a), b, c);

	}
	
	#define HOOK(a, b, c) \
HookWrap::nh(a, \
(void*)b, \
(void**)&c);

#define HOOK_STATIC(a, b, c) \
HookWrap::sh(a, \
HookManager::getPointer(&b), \
(void **)&c)

};


