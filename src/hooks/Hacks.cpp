#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"
#include "patch.h"
#include "GDPSHelper.h"
#include "GDPSManager.h"
#include "FunctionHelper.h"
#include "gd.h"
#include "../definitions.h"



FUNCTIONHOOK(void, MoreOptionsLayer_onToggle, MoreOptionsLayer* self, CCMenuItemToggler* sender) {
	
	bool on = !sender->_isToggled();
	int gamevariable = sender->getTag();
	
	if(gamevariable > 200000) {
		
		patch p;
		
		switch(gamevariable) 
		{		
			case 200001: // Safe NoClip
				p.addPatch(0x00276df8, on ? "7047" : "2DE9"); // PlayLayer::destroyPlayer
			//	p.addPatch(0x00276934, on ? "CFE6" : "2DE9"); // PlayLayer::levelComplete
			break;
			/*
			case 200002: // Safe Mode
				p.addPatch(0x00276934, on ? "CFE6" : "2DE9"); // PlayLayer::levelComplete
			break;
			

            case 200003: // No Death Effect (I hope this doesnt effect any codes here since i make the PlayerObject::playerDestroyed to (null)
				p.addPatch(0x0029c23c, on ? "7047" : "90F8"); // PlayerObject::playerDestroyed
			break;
			*/
		}
		p.Modify();
	}
	
	MoreOptionsLayer_onToggleO(self, sender);
	
}

bool isHackActive() {
	auto gm = GM;
	return gm->ggv("200001") || gm->ggv("200002") || gm->ggv("200003");
}

FUNCTIONHOOK(void, PlayLayer_levelCompleted, PlayLayer* self) {
	
	if(!isHackActive())
		PlayLayer_levelCompletedO(self);
	else {
		self->onQuit();
	}
}

FUNCTIONHOOK(void*, CCTransitionFade_create, float time, void* scene, void* color) {
	return CCTransitionFade_createO(GM->ggv("200004") ? 0.0f : time, scene, color);
}

FUNCTIONHOOK(void, LeveInfoLayer_onPlay, void* self, void* sender) {

	if(GDPS->acceptedHackAlert || GM->ggv("200005") || !isHackActive())
		return LeveInfoLayer_onPlayO(self, sender);
	
	GDPS->acceptedHackAlert = true;
	FLAlertLayer::create( nullptr, "Hacks enabled", "You will be <cr>kicked out</c> from the level at 100%!", "OK", nullptr, 380., false, 150. )->show( );
}
void Hacks::ApplyHooks() 
{
	HOOK2("_ZN14LevelInfoLayer6onPlayEPN7cocos2d8CCObjectE", LeveInfoLayer_onPlay);
	HOOK2("_ZN7cocos2d16CCTransitionFade6createEfPNS_7CCSceneERKNS_10_ccColor3BE", CCTransitionFade_create);
	HOOK("_ZN9PlayLayer13levelCompleteEv", PlayLayer_levelCompletedH, PlayLayer_levelCompletedO);
	HOOK("_ZN16MoreOptionsLayer8onToggleEPN7cocos2d8CCObjectE", MoreOptionsLayer_onToggleH, MoreOptionsLayer_onToggleO);
}