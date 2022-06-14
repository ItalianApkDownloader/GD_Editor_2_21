#pragma once

#include "cocos2d.h"
#include "CCSprite.h"
#include "GameManager.h"
#include "CCLayer.h"
#include "CreatorLayer.h"

class WorldSelectLayer : public cocos2d::CCLayer 
{

public:
    WorldSelectLayer();
    virtual ~WorldSelectLayer();

    static CCScene* scene( int a1 );
};

