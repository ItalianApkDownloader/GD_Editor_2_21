#include "AdvancedLevelInfo.h"
#include <map>
#include <vector>
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"
#include "GDPSHelper.h"
#include "FunctionHelper.h"
#include "gd.h"

#include "CCGLProgram.h"

using namespace cocos2d;


#include <memory>
#include <string>
#include <stdexcept>
#include <cmath>



#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

#define format(str, ...) CCString::createWithFormat(str, __VA_ARGS__)->getCString()


int activatedTriggers = 0;
CCLabelBMFont* label;


FUNCTIONHOOK(bool, UILayer_init3, UILayer* self) {
	
	if(!UILayer_init3O(self)) return false;
	
	activatedTriggers = 0;
	if(!GM->getGameVariable("1000013")) return true;
	
	
	label = CCLabelBMFont::create(FunctionHelper::itos(activatedTriggers).c_str(), "bigFont.fnt");
	label->setScale(.6);
	label->setPosition(CCLEFT + 165, CCTOP - 50);
	self->addChild(label);
	
	return true;
}

FUNCTIONHOOK(void, EffectGameObject_triggerActivated, void* self, void* baselayer, int idk) {
	
	EffectGameObject_triggerActivatedO(self, baselayer, idk);

	if(GM->_inEditor() || !label) return;
	
	activatedTriggers++;
	label->CCLabelBMFont::setString(FunctionHelper::itos(activatedTriggers).c_str());
	
}

FUNCTIONHOOK(void, GJBaseGameLayer_stopTriggersInGroup, void* self, void* a1, void* a2) {
	
	GJBaseGameLayer_stopTriggersInGroupO(self, a1, a2);
	
	CCLog("stop triggers in group called!, a1: %d", a1);
	
}

FUNCTIONHOOK(void, PlayLayer_onQuit, void* self) {
	PlayLayer_onQuitO(self);
	label = nullptr;
}

template <typename T>
std::string to_string(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}


class GroupCommandObject {};


FUNCTIONHOOK(void*, GroupCommandObject_updateTweenActionInt, void* self, void* a1, void* a2 ) {
	return GroupCommandObject_updateTweenActionIntO(self, a1, a2);
	
}
/*
FUNCTIONHOOK(void*, test, void* self, float delta) {
	auto ret = testO(self, delta);
	
	
	auto f = MBO(float, self, 0x128);
	
	
	auto fstr = format("%f", f);
	CCLog(fstr);
	
	if(containss(fstr, ".000000"))
		if(!contains(fstr, "0.000000"))
			CCLog("trigger ended");
	return ret;
}
*/
void AdvancedLevelInfo::ApplyHooks() 
{	
	//HOOK("_ZN18GroupCommandObject20updateTweenActionIntEfi", GroupCommandObject_updateTweenActionIntH, GroupCommandObject_updateTweenActionIntO);
//	HOOK("_ZN18GroupCommandObject4stepEf", testH, testO);
//	HOOK("_ZN15GJBaseGameLayer19stopTriggersInGroupEi15GJActionCommand", GJBaseGameLayer_stopTriggersInGroupH, GJBaseGameLayer_stopTriggersInGroupO);
	HOOK("_ZN9PlayLayer6onQuitEv", PlayLayer_onQuitH, PlayLayer_onQuitO);
	HOOK("_ZN16EffectGameObject13triggerObjectEP15GJBaseGameLayeri", EffectGameObject_triggerActivatedH, EffectGameObject_triggerActivatedO);
	HOOK("_ZN7UILayer4initEv", UILayer_init3H, UILayer_init3O);

}