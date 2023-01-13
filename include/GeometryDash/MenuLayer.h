#pragma once

#include <cocos2dx/CCLayer.h>

#include "CCMenuItemSpriteExtra.h"

class MenuLayer : public cocos2d::CCLayer 
{
public:
    CLASS_MEMBER(CCLabelBMFont*, playerUsernameLabel, 0x148);
    CLASS_MEMBER(CCMenuItemSpriteExtra*, profileBtn, 0x14C);

public:
    MenuLayer();
    virtual ~MenuLayer();
	  static cocos2d::CCScene* scene(bool);
    bool init( );
	void onOptions(CCObject*);
	void onOptionsInstant(CCObject*);
	void onGarage(CCObject*);
	void onStats(CCObject*);
	void onAchievements(CCObject*);
	void onMyProfile(CCObject*);
	void onQuit(CCObject*);
};