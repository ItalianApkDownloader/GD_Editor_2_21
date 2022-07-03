#pragma once
#include "FLAlertLayer.h"

//seems like all triggers inherit this class now, i like it

class SetupTriggerPopup : public FLAlertLayer
{
public:
	
	CLASS_MEMBER(CCMenu*, menu, 0x1B0);
	CLASS_MEMBER(EffectGameObject*, obj, 0x1F4);
	CLASS_MEMBER(CCArray*, objects, 0x1F8);
	CLASS_MEMBER(float, xSize, 0x210);
	CLASS_MEMBER(float, ySize, 0x214);
	CLASS_MEMBER(CCArray*, someArray, 0x250);
	
	public:
	
	void addObjectsToPage(cocos2d::CCArray *objects, int page);

};