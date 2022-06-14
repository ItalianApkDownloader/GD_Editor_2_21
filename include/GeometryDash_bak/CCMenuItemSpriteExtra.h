#ifndef __CCMENUITEMSPRITEEXTRA_H__
#define __CCMENUITEMSPRITEEXTRA_H__

#include "GeometryDash.h"
#include "cocos2d.h"

USING_NS_CC;


enum MenuAnimationType : int{
	tipo0,tipo1
};




class CCMenuItemSpriteExtra : public CCMenuItemSprite {
public:
	~CCMenuItemSpriteExtra();
	
	static CCMenuItemSpriteExtra *create(CCNode *normalSprite, CCNode *selectedSprite, CCObject *target, SEL_MenuHandler selector);
	bool init(CCNode *normalSprite, CCNode *selectedSprite, CCObject *target, SEL_MenuHandler selector);
	
	void activate();
	
	void useAnimationType(MenuAnimationType);
	
	
	void selected();
	void unselected();
	
	CC_SYNTHESIZE(float, scaleVar_, ScaleVar);
	CC_SYNTHESIZE(float, originalScale_, OriginalScale);
	/* If the sprite should change on select */
	CC_SYNTHESIZE(float, shouldAnimate_, ShouldAnimate);
	/* If the brightness should change on click */
	CC_SYNTHESIZE(float, darkenClick_, DarkenClick);
	CC_SYNTHESIZE(float, volume_, Volume);
	CC_SYNTHESIZE(const char*, clickSound_, ClickSound);
};

#endif //__CCMENUITEMSPRITEEXTRA_H__
