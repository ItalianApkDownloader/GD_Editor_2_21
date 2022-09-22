#include "SpeedrunTimer.h"
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

#define TIMER_ENABLED GM->getGameVariable("1000011")

#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

CCLabelBMFont* timerLabel;
float timer = 0;
bool fix1;
bool isTimeValid;


UILayer* getUILayer() {
	return MBO(UILayer*, GM->_playLayer(), 0x2CA0);
}

std::string formatSecond(float value)
{
    /*
        r - remainder
        m - minute
        s - second
    */

    float r = value;

    int m = r / 60;
    r = fmod(r, 60.0);

    int s = r;

    std::string mm = FunctionHelper::itos(m);
    std::string ss = m > 0 ? std::string(std::max(2 - static_cast<int>(FunctionHelper::itos(s).length()), 0), '0') + FunctionHelper::itos(s) : FunctionHelper::itos(s);
    std::string msms = std::string(CCString::createWithFormat("%.3f", r - floor(r))->getCString()).erase(0, 2);

    std::string result;

    if (m != 0)
    {
        result = mm + ":" + ss + "." + msms;
    }
    else
    {
        result = ss + "." + msms;
    }

    return result;
}

void SpeedrunTimer::UpdateTimer(float delta)
{
    timer += delta;
    timerLabel->CCLabelBMFont::setString(formatSecond(timer).c_str());
//	CCLog("update timer");

};

FUNCTIONHOOK(void, PlayLayer_levelComplete, PlayLayer* self) 
{
	//CCLog("PlayLayer_levelComplete");
	PlayLayer_levelCompleteO(self);

	if(TIMER_ENABLED) {
        getUILayer()->unschedule(schedule_selector(SpeedrunTimer::UpdateTimer));
	}
}

FUNCTIONHOOK(void, PlayLayer_togglePracticeMode, PlayLayer* self, bool practiceMode) {
	
	//CCLog("PlayLayer_togglePracticeMode");
	PlayLayer_togglePracticeModeO(self, practiceMode);
			if(TIMER_ENABLED) {

	cocos2d::_ccColor3B	color = practiceMode ? ccc3(255, 0, 0) : ccc3(255, 255, 255);
	isTimeValid = !practiceMode;
	fix1 = false;

	timerLabel->CCLabelBMFont::setColor(color);
		}
}


FUNCTIONHOOK(void, PauseLayer_onResume, void* self, CCObject* sender) {
	
	//CCLog("PauseLayer_onResume");
	PauseLayer_onResumeO(self, sender);
	
	if(TIMER_ENABLED && fix1) 
	{
		cocos2d::_ccColor3B	color = ccc3(255, 0, 0);
		timerLabel->CCLabelBMFont::setColor(color);
		timerLabel->setVisible(true);
		isTimeValid = false;
	}
	//if disable timer in pause menu
	else if(!TIMER_ENABLED) 
	{
		getUILayer()->unschedule(schedule_selector(SpeedrunTimer::UpdateTimer));
		timerLabel->setVisible(false);
		isTimeValid = false;
	}
		
	
}

FUNCTIONHOOK(void, PauseLayer_init, void* self) {
	fix1 = true;
	PauseLayer_initO(self);
}


FUNCTIONHOOK(void, PlayLayer_resetLevel, PlayLayer* self)
{
	//CCLog("PlayLayer_resetLevel");
    PlayLayer_resetLevelO(self);
	if(TIMER_ENABLED) {

		bool practiceMode = MBO(bool, self, 0x29A0);
	//	CCLog("practice mode: %d", practiceMode);
		
		if(!practiceMode) {
			timer = 0;
			
			cocos2d::_ccColor3B	color = ccc3(255, 255, 255);

			timerLabel->CCLabelBMFont::setColor(color);
			isTimeValid = true;
		}
		
    getUILayer()->schedule(schedule_selector(SpeedrunTimer::UpdateTimer), 0.01);
	}
}

//destroyPlayer is acting weird so i use this which is more reliable
FUNCTIONHOOK(void, PlayerObject_playDeathEffect, PlayerObject* self)
{
	CCLog("PlayerObject_playDeathEffect");
    PlayerObject_playDeathEffectO(self);
	
	if(TIMER_ENABLED) {
		getUILayer()->unschedule(schedule_selector(SpeedrunTimer::UpdateTimer));
	}
}
/*
//since PlayLayer::Update was separated into different funcs ill just take updateVisbility
//will be used later on to upate timer opacity/color/position once i gete into it lol
FUNCTIONHOOK(void, PlayLayer_updateVisiblity, PlayLayer* self, float delta)
{
    PlayLayer_updateVisiblityO(self, delta);
}
*/
//chad libhooking lib supports multihook so i can just hook uilayer init again and nothing will happen
FUNCTIONHOOK(bool, UILayer_init2, UILayer* self) {
	
	if(!UILayer_init2O(self)) return false;
		
	timerLabel = CCLabelBMFont::create("0", "bigFont.fnt");
	timerLabel->setScale(.5);
	timerLabel->setPosition(CCLEFT + 60, CCTOP - 10);
	self->addChild(timerLabel);
	

	TIMER_ENABLED ? self->schedule(schedule_selector(SpeedrunTimer::UpdateTimer), 0.01) : timerLabel->setVisible(false);
	isTimeValid = TIMER_ENABLED;
	return true;
}


FUNCTIONHOOK(void, EndLevelLayer_customSetup, CCLayer* self) {
	
	EndLevelLayer_customSetupO(self);
	if(!isTimeValid) return;
	
	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto reference = (CCNode*)layer->getChildren()->objectAtIndex(7);
	
	auto label = CCLabelBMFont::create(CCString::createWithFormat("Speedrun: %s", formatSecond(timer).c_str())->getCString(), "goldFont.fnt");
	label->setPosition(reference->GPX(), reference->GPY() - 20);
	label->setScale(0.8);
	layer->addChild(label);
	
	//GDPSHelper::createLabels(layer, layer->getChildren(), {0, 0}, true);
	
}

void SpeedrunTimer::ApplyHooks() 
{	
	HOOK("_ZN10PauseLayer11customSetupEv", PauseLayer_initH, PauseLayer_initO);
	HOOK("_ZN9PlayLayer13levelCompleteEv", PlayLayer_levelCompleteH, PlayLayer_levelCompleteO);
	HOOK("_ZN9PlayLayer18togglePracticeModeEb", PlayLayer_togglePracticeModeH, PlayLayer_togglePracticeModeO);
	HOOK("_ZN10PauseLayer8onResumeEPN7cocos2d8CCObjectE", PauseLayer_onResumeH, PauseLayer_onResumeO);
	HOOK("_ZN9PlayLayer10resetLevelEv", PlayLayer_resetLevelH, PlayLayer_resetLevelO);
	HOOK("_ZN12PlayerObject15playDeathEffectEv", PlayerObject_playDeathEffectH, PlayerObject_playDeathEffectO);
	//HOOK("_ZN9PlayLayer16updateVisibilityEf", PlayLayer_updateVisiblityH, PlayLayer_updateVisiblityO);
	HOOK("_ZN7UILayer4initEv", UILayer_init2H, UILayer_init2O);
	HOOK("_ZN13EndLevelLayer11customSetupEv", EndLevelLayer_customSetupH, EndLevelLayer_customSetupO);
}