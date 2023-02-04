#include "SwingIconFix.h"
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"
#include "patch.h"
#include "GDPSHelper.h"
#include "FunctionHelper.h"
#include "gd.h"


FUNCTIONHOOK(bool, PlayLayer_init, PlayLayer* self, GJGameLevel* level, bool a2, bool a3) {
	
	
	int iconID = MBO(int, GM, 0x224);
	
	//MOVS R1, #1 -> MOVS R1, iconID
//	string patchstr = FunctionHelper::itohex(iconID) + "21";
	string patchstr;
	if(10 > iconID)
		patchstr = fmt::format("0{}21", iconID); 
	else if(16 > iconID)
		patchstr = fmt::format("0{}21", FunctionHelper::itohex(iconID));
	else
		patchstr = fmt::format("{}21", FunctionHelper::itohex(iconID));

	
	
	constexpr std::array<uintptr_t, 5> addrs = {0x299484, 0x29948C, 0x299498, 0x2994A8, 0x299552};
	patch p;
	
	for(auto adr : addrs)
		p.addPatch("libcocos2dcpp.so", adr, patchstr);
	
	p.Modify();
	
	return PlayLayer_initO(self, level, a2, a3);
}


void SwingIconFix::ApplyHooks() 
{
	HOOK("_ZN9PlayLayer4initEP11GJGameLevelbb", PlayLayer_initH, PlayLayer_initO);
}


uintptr_t loadIcon = 0x299484;
uintptr_t color1 = 0x29948C;
uintptr_t color2 = 0x299498;
uintptr_t glow = 0x2994A8;
uintptr_t extra = 0x299552;

/*
int *__fastcall PlayerObject::updatePlayerSwingFrame(int this, int a2)
{
	// [COLLAPSED LOCAL DECLARATIONS. PRESS KEYPAD CTRL-"+" TO EXPAND]

	v3 = (GameManager *)GameManager::sharedState((GameManager *)this);
	GameManager::loadIcon(v3, 1, 7, *(_DWORD *)(this + 0xBE0));
	v4 = (cocos2d::CCString *)cocos2d::CCString::createWithFormat((cocos2d::CCString *)"swing_%02d_001.png",(const char *)&dword_0 + 1);
	str1 = (const char *)cocos2d::CCString::getCString(v4);
	v6 = (cocos2d::CCString *)cocos2d::CCString::createWithFormat((cocos2d::CCString *)"swing_%02d_2_001.png",(const char *)&dword_0 + 1);
	str2 = (const char *)cocos2d::CCString::getCString(v6);
	v8 = (cocos2d::CCString *)cocos2d::CCString::createWithFormat((cocos2d::CCString *)"swing_%02d_glow_001.png",(const char *)&dword_0 + 1);
	str3 = (cocos2d::CCSpriteFrameCache *)cocos2d::CCString::getCString(v8);
	v10 = *(_DWORD *)(this + 0x5A0);
	v11 = *(int (__fastcall **)(int, int))(*(_DWORD *)v10 + 0x23C);
	str3_1 = (const char *)str3;
	v13 = (cocos2d::CCSpriteFrameCache *)cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache(str3);
	v16 = cocos2d::CCSpriteFrameCache::spriteFrameByName(v13, str1, v14, v15);
	v17 = (cocos2d::CCSpriteFrameCache *)v11(v10, v16);
	v18 = *(_DWORD *)(this + 0x5A4);
	v19 = *(int (__fastcall **)(int, int))(*(_DWORD *)v18 + 0x23C);
	v20 = (cocos2d::CCSpriteFrameCache *)cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache(v17);
	v23 = cocos2d::CCSpriteFrameCache::spriteFrameByName(v20, str2, v21, v22);
	v24 = (cocos2d::CCSpriteFrameCache *)v19(v18, v23);
	v25 = *(_DWORD *)(this + 0x5AC);
	v26 = *(void (__fastcall **)(int, int))(*(_DWORD *)v25 + 0x23C);
	v27 = (cocos2d::CCSpriteFrameCache *)cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache(v24);
	v30 = cocos2d::CCSpriteFrameCache::spriteFrameByName(v27, str3_1, v28, v29);
	v26(v25, v30);
	v31 = (const cocos2d::CCSize *)(*(int (__fastcall **)(_DWORD))(**(_DWORD **)(this + 0x5A0) + 0x9C))(*(_DWORD *)(this + 0x5A0));
	cocos2d::CCSize::CCSize((cocos2d::CCSize *)v38, v31);
	v32 = *(_DWORD *)(this + 0x5A4);
	v33 = *(void (__fastcall **)(int, int *))(*(_DWORD *)v32 + 0x5C);
	cocos2d::CCPoint::CCPoint((cocos2d::CCPoint *)v39, v38[0] * 0.5, v38[1] * 0.5);
	v33(v32, v39);
	v34 = (cocos2d::CCString *)cocos2d::CCString::createWithFormat(
	 (cocos2d::CCString *)"swing_%02d_extra_001.png",
	 (const char *)&dword_0 + 1);
	v35 = (char *)cocos2d::CCString::getCString(v34);
	std_string_constructor(v39, v35, (int)&v37);
	PlayerObject::updatePlayerSpriteExtra((cocos2d::CCSpriteFrameCache *)this, (const char **)v39);
	return sub_62B744(v39);
}
*/