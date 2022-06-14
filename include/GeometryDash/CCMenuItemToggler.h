#pragma once

#include "cocos2d.h"

#include "GDAPI_Macros.h"

class CCMenuItemToggler : public cocos2d::CCMenuItem {
public:
    CLASS_MEMBER(bool, isToggled, 0x120);

public:
    static CCMenuItemToggler* create(cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
    void setSizeMult(float mult);
    void toggle(bool toggle);
};