#pragma once
#include "cocos2d.h"
#include "CCTextInputNode.h"

class AccountLoginLayer : public cocos2d::CCLayerColor
{
public:

public:
    AccountLoginLayer();
    virtual ~AccountLoginLayer();
	
	
	CLASS_MEMBER(CCTextInputNode*, inputPassword, 0x1FC);


};
