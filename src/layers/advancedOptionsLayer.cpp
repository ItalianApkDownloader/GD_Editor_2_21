#include "advancedOptionsLayer.h"
#include "cocos2d.h"
#include "gd.h"
#include <cocos2dx_bak/extensions/GUI/CCControlExtension/CCScale9Sprite.h>

using namespace cocos2d;

void advancedOptionsLayer::show()
{
	auto scene = CCDirector::sharedDirector()->getRunningScene();
	this->addChild(scene);
	auto fadeto = CCFadeTo::create(0.14, 100);
	this->runAction(fadeto);
}

bool advancedOptionsLayer::init()
{

	bool result = this->initWithColor(ccc4(0, 0, 0, 0));
	if (result)
	{
		auto dir = CCDirector::sharedDirector();
		dir->getTouchDispatcher()->incrementForcePrio(2);
		this->setTouchEnabled(true);
		this->setKeypadEnabled(true);
		this->setKeyboardEnabled(true);

		auto layer = CCLayer::create();
		layer->setContentSize(dir->getWinSize());
		this->addChild(layer);
		auto d = dir->getTouchDispatcher();
		d->addTargetedDelegate(this, -500, true);
	}
	return result;
}

void advancedOptionsLayer::keyBackClicked()
{

	this->close();
}

void advancedOptionsLayer::close()
{
	this->removeFromParentAndCleanup(true);
}

advancedOptionsLayer *advancedOptionsLayer::create()
{

	advancedOptionsLayer *pRet = new advancedOptionsLayer();
	if (pRet)
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}
