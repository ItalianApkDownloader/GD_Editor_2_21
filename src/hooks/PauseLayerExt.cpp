#include <dlfcn.h>
#include "blaze.h"
#include "cocos2d.h"
#include "PauseLayerExt.h"
#include "GDPSManager.h"
#include "patch.h"
#include "CCMenuItemToggler.h"
#include "gd.h"

class SliderThumb
{
public:
    float getValue();
};

bool PauseLayerExt::init_hk()
{

    auto ret = init_trp(this);

    this->firstLayer = CCLayer::create();

    for (int k = 0; k < this->getChildrenCount(); k++)
    {
		auto node = reinterpret_cast<CCNode*>(this->getChildren()->objectAtIndex(k));
        this->firstLayer->addChild(node);
		//this->removeChild(node);
		
    }
	
		this->removeAllChildren();
	
	auto bg = reinterpret_cast<CCNode*>(this->firstLayer->getChildren()->objectAtIndex(0));
	
	this->addChild(bg);
	this->firstLayer->removeChild(bg);
	this->addChild(this->firstLayer);
	
	auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto miscNext = CCMenu::create();
    auto miscBack = CCMenu::create();

    auto sp = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(
        sp,
        sp,
        this,
        menu_selector(PauseLayerExt::onPrevPage));

    auto sp2 = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    sp2->setFlipX(1);
    auto nextbtn = CCMenuItemSpriteExtra::create(
        sp2,
        sp2,
        this,
        menu_selector(PauseLayerExt::onNextPage));

    auto manager = GDPSManager::sharedState();

    auto dir = CCDirector::sharedDirector();

    nextbtn->setPosition((+winSize.width / 2) - 50, 0);
    backBtn->setPosition((-winSize.width / 2) + 50, 0);

    miscBack->addChild(backBtn);
    miscNext->addChild(nextbtn);

    firstLayer->addChild(miscNext, 100);

	
	//second layer
	
	this->secondLayer = CCLayer::create();
    secondLayer->addChild(miscBack, 100);
	auto title = CCLabelBMFont::create("Other settings", "bigFont.fnt");
        title->setPosition(ccp(winSize.width/2,winSize.height - 30));
        this->secondLayer->addChild(title);


        auto txt = CCLabelBMFont::create("Platform opacity", "bigFont.fnt");
        txt->setScale(0.5);
        auto sl = Slider::create(this,menu_selector(PauseLayerExt::onPlatformOpacity),1);

        float value = (float)GDPS->opacity / (float)255;
        sl->setValue(value);
        txt->setPosition(ccp(winSize.width/2 - 100,winSize.height/2 + 100));
        sl->setPosition(ccp(winSize.width/2 - 100,winSize.height/2 + 80));
		
		
		auto txt2 = CCLabelBMFont::create("Timer Opacity", "bigFont.fnt");
        txt2->setScale(0.5);
        auto sl2 = Slider::create(this,menu_selector(PauseLayerExt::onTimerOpacity),1);

        float value2 = (float)GDPS->opacityTimer / (float)255;
        sl2->setValue(value2);
        txt2->setPosition(ccp(winSize.width/2 - 100,winSize.height/2 + 60));
        sl2->setPosition(ccp(winSize.width/2 - 100,winSize.height/2 + 40));
		
		auto menu = CCMenu::create();
		menu->setPosition(0,0);
		
//(const char*, CCPoint, CCObject*, cocos2d::SEL_MenuHandler, CCMenu*, bool, bool);

		GDPS->createToggleButton("Timer", {CCMIDX - 150 , CCMIDY}, this, menu_selector(PauseLayerExt::onToggleTimer), menu, GM->getGameVariable("100008"), true);
		secondLayer->addChild(menu);
		
		
		
        this->secondLayer->addChild(txt);
        this->secondLayer->addChild(sl);
		
		this->secondLayer->addChild(txt2);
        this->secondLayer->addChild(sl2);
		this->secondLayer->setVisible(false);
		
		this->addChild(this->secondLayer);
		


    return ret;
}

void PauseLayerExt::onNextPage(CCObject *sender)
{
//	this->removeAllChildren();
	//	auto bg = reinterpret_cast<CCNode*>(this->firstLayer->getChildren()->objectAtIndex(0));
//	this->addChild(bg);
	
	this->firstLayer->setVisible(false);
	//this->addChild(secondLayer);
	this->secondLayer->setVisible(true);
}


void PauseLayerExt::onPrevPage(CCObject *sender)
{
//this->removeAllChildren();
//	auto bg = reinterpret_cast<CCNode*>(this->firstLayer->getChildren()->objectAtIndex(0));
	//this->addChild(bg);
//this->addChild(firstLayer);
	this->firstLayer->setVisible(true);
	
	this->secondLayer->setVisible(false);

}


void PauseLayerExt::onPlatformOpacity(CCObject *sender)
{
    auto value = reinterpret_cast<SliderThumb *>(sender)->getValue();
    GDPSManager::sharedState()->opacity = 255 * value;
}

void PauseLayerExt::onToggleTimer(CCObject *sender)
{
    auto toggle = reinterpret_cast<CCMenuItemToggler*>(sender);
	GM->setGameVariable("100008", toggle->_isToggled());
	
	extern CCLabelBMFont* timerLabel;
	timerLabel->setVisible(toggle->_isToggled());
}

void PauseLayerExt::onTimerOpacity(CCObject *sender)
{
    auto value = reinterpret_cast<SliderThumb *>(sender)->getValue();
    GDPSManager::sharedState()->opacityTimer = 255 * value;
}

