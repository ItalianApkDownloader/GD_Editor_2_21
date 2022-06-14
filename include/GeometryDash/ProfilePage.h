#pragma once

#include <cocos2dx/CCLayer.h>
#include "FLAlertLayer.h"
#include "GJUserscore.h"

#include "GDAPI_Macros.h"

class ProfilePage : public FLAlertLayer {
public:
    CLASS_MEMBER(cocos2d::CCMenu*, modBadgeMenuThing, 0x1B8);
    CLASS_MEMBER(cocos2d::CCArray*, someArray, 0x220);
    CLASS_MEMBER(cocos2d::CCLabelBMFont*, userName, 0x208);
    CLASS_MEMBER(cocos2d::CCSprite*, modBadgeSprite, 0x1B8);
    CLASS_MEMBER(cocos2d::CCSprite*, socials, 0xA0);
	
public:

void onUpdate(CCObject*);
void userInfoChanged(GJUserScore*);
void loadPageFromUserInfo(GJUserScore*);
};