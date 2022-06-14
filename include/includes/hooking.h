#define hook(x, y, z) 	HookManager::do_hook(getPointerFromSymbol(cocos2d, "x"), (void*)y, (void**)&z);

class HookManager {
	public:
		static void do_hook(void* origin, void * newfunc, void* trampoline,bool hookzz = false);
};

