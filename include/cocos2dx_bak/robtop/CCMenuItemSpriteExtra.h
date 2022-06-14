#ifndef _CCMenuItemSpriteExtra_h
#define _CCMenuItemSpriteExtra_h

#include "cocos2d.h"

NS_CC_BEGIN

enum MenuAnimationType : int{
	tipo0,tipo1,tipo2
};


class CC_DLL CCMenuItemSpriteExtra : public CCMenuItemSprite
{
public:
	//bool init();
	//static CCMenuItemSpriteExtra * CDECL create(CCNode* normalSprite, CCObject *target, SEL_MenuHandler selector);
	static CCMenuItemSpriteExtra * create(CCNode* normalSprite, CCObject *target, SEL_MenuHandler selector);
	//void setSizeMult(float);
	void useAnimationType(MenuAnimationType);
	//void activate();
	//void selected();
	//void unselected();
	void * operator new(size_t);
};

NS_CC_END


#endif