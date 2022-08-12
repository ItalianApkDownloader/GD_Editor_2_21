#pragma once
#include "cocos2d.h"
#include "hooking.h"

#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)


class MultiplayerLayer : public CCLayer
{
public:

    //void createToggleButton(std::string,void (cocos2d::CCObject::*)(cocos2d::CCObject*),bool,cocos2d::CCMenu *,cocos2d::CCPoint);
    void onBtn1(CCObject* sender);
    void onBack(CCObject* sender);
    

};


