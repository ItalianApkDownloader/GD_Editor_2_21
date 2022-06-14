#pragma once

#include <cocos2dx/CCLayer.h>

class GJGarageLayer : public cocos2d::CCLayer
{
public:
    GJGarageLayer();
    virtual ~GJGarageLayer();

    bool init();
    static cocos2d::CCScene* scene();

    void selectPage(IconType);
    void updatePlayerColors();
};