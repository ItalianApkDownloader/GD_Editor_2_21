#pragma once
#include "cocos2d.h"

class MultiplayerLayer : public CCLayer
{
public:

    MultiplayerLayer();
    virtual ~MultiplayerLayer();

    static MultiplayerLayer* scene();
    static MultiplayerLayer* create();
    bool init();

    void onBtn1(CCObject* sender);
    void onBtn2(CCObject* sender);
    void onBack(CCObject* sender);
};