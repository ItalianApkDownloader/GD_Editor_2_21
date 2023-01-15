#pragma once
#include "cocos2d.h"
#include "hooking.h"

class MultiplayerLayer : public CCLayer
{
public:

    //void createToggleButton(std::string,void (cocos2d::CCObject::*)(cocos2d::CCObject*),bool,cocos2d::CCMenu *,cocos2d::CCPoint);
    void onBtn1(CCObject* sender);
    void onBack(CCObject* sender);
    

};


