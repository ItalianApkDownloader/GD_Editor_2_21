#pragma once

#include <cocos2dx/CCLayer.h>

class GaragePage : public cocos2d::CCLayer 
{
public:
    GaragePage();
    virtual ~GaragePage();
	void updateSelect(cocos2d::CCNode *);
	//void listButtonBarSwitchedPage(ListButtonBar *,int);
	void onSelect(cocos2d::CCObject *);
	void onGlow(cocos2d::CCObject *);
	bool init(IconType,GJGarageLayer *,void (cocos2d::CCObject::*)(cocos2d::CCObject*));
	static GaragePage* create(IconType,GJGarageLayer *,void (cocos2d::CCObject::*)(cocos2d::CCObject*));
};

