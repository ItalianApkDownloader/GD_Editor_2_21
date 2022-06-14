#pragma once

#include <cocos2dx/CCLayer.h>

#include "CCMenuItemSpriteExtra.h"

class MapSelectLayer : public cocos2d::CCLayer 
{
public:


public:
    MapSelectLayer();
    virtual ~MapSelectLayer();
	  static cocos2d::CCScene* scene(int);
    bool init();
};