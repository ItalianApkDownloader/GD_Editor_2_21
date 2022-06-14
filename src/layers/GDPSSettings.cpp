#include "Includes/Logger.h"
#include "gd.h"
#include <dirent.h>
#include <paths.h>
#include "GDPSSettings.h"

#define ARROW CCSprite::createWithSpriteFrameName("edit_rightBtn_001.png")

CCScene *underScene;
float screenWidth = 0;

void GDPSSettings::show()
{
    auto scene = CCDirector::sharedDirector()->getRunningScene();
    underScene = scene;
    this->setZOrder(9999);
    scene->addChild(this);
    auto fadeto = CCFadeTo::create(0.14, 100);
    this->runAction(fadeto);
}

bool GDPSSettings::init()
{

    bool result = this->initWithColor(ccc4(0, 0, 0, 0));

    CCLog("init!");
    /*
    if (result) {
        auto dir = CCDirector::sharedDirector();


        dir->getTouchDispatcher()->incrementForcePrio(2);
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);
        this->setKeyboardEnabled(true);





        auto layer = CCLayer::create();
        layer->setContentSize(dir->getWinSize());

        screenWidth = dir->getWinSize().width;
        this->addChild(layer);
        auto d = dir->getTouchDispatcher();
        d->addTargetedDelegate(this, -500, true);

        LOGD("dadas");




        CCArray * rows = CCArray::create();
        rows->retain();




        auto pop = GJDropDownLayer::create("Test!");

        pop->showLayer(true);
        this->addChild(pop);







    }
     */
    return result;
}

void GDPSSettings::keyBackClicked()
{

    this->close();
}

void GDPSSettings::close()
{
    this->removeFromParentAndCleanup(true);
}

GDPSSettings *GDPSSettings::create()
{
    auto v12 = new GDPSSettings();
    if (v12)
    {
        if (v12->init())
        {
            v12->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(v12);
        }
    }
    return v12;
}

bool GDPSSettings::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{

    return true;
}

void GDPSSettings::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
}

void GDPSSettings::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *)
{
}
