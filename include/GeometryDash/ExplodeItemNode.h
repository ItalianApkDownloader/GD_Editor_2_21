#pragma once
#include "GDAPI_Macros.h"
#include "CCNode.h"

class ExplodeItemNode : public cocos2d::CCNode
{
public:
    static ExplodeItemNode* create(CCRenderTexture*);
	void createSprites(int,int,float,float,float,float,float,float,ccColor4F,ccColor4F,bool);
};