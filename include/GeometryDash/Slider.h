#pragma once
#include "GDAPI_Macros.h"
#include <cocos2dx/CCLayer.h>
#include <cocos2dx/CCNode.h>


class Slider : public cocos2d::CCSprite {
public:
    static Slider* create(cocos2d::CCNode *,void (cocos2d::CCObject::*)(cocos2d::CCObject*),float);
    void setValue(float);
    void setBarVisibility(bool);
    float getValue();
};