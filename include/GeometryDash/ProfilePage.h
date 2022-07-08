#pragma once

#include <cocos2dx/CCLayer.h>
#include "FLAlertLayer.h"
#include "GJUserscore.h"

#include "GDAPI_Macros.h"

class ProfilePage : public FLAlertLayer {
public:

	CLASS_MEMBER(int, accountID, 0x208);
	CLASS_MEMBER(cocos2d::CCLabelBMFont*, someThingWentWrong, 0x218);
	CLASS_MEMBER(cocos2d::CCLabelBMFont*, somelabel, 0x218);
	CLASS_MEMBER(CCMenu*, menu, 0x1B0);
	CLASS_MEMBER(CCArray*, someArray, 0x230);
	CLASS_MEMBER(CCMenuItemSpriteExtra*, followingBtn, 0x228);
//	CLASS_MEMBER(LoadingCircle*, LoadingCircle, 0x248);
	CLASS_MEMBER(CCMenuItemSpriteExtra*, refreshBtn, 0x250);
	CLASS_MEMBER(GJUserScore*, userScore, 0x204);

	

	

	//old
	/*
    CLASS_MEMBER(cocos2d::CCMenu*, modBadgeMenuThing, 0x1B8);
    CLASS_MEMBER(cocos2d::CCArray*, someArray, 0x220);
    CLASS_MEMBER(cocos2d::CCLabelBMFont*, userName, 0x208);
    CLASS_MEMBER(cocos2d::CCSprite*, modBadgeSprite, 0x1B8);
    CLASS_MEMBER(cocos2d::CCSprite*, socials, 0xA0);
	*/
	
public:

void onUpdate(CCObject*);
void userInfoChanged(GJUserScore*);
void loadPageFromUserInfo(GJUserScore*);
};