//
// Created by Marco on 03/11/2021.
//

#include <cocos2dx_bak/extensions/GUI/CCControlExtension/CCScale9Sprite.h>
#include <hooking.h>
#include <dlfcn.h>
#include <Logger.h>
#include "MPCreateLobbyPopup.h"
#include "GameManager.h"
#include "gd.h"


bool MPCreateLobbyPopup::init(MultiplayerLayer *MPLayer)
{

    if (this->initWithColor({0x00, 0x00, 0x00, 0x3B}))
    {
        auto dir = CCDirector::sharedDirector();
        MPL = MPLayer;
        

        dir->getTouchDispatcher()->incrementForcePrio(2);
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);
        this->setKeyboardEnabled(true);
        auto layer = CCLayer::create();
        layer->setContentSize(dir->getWinSize());
        this->m_pLayer = layer;
        this->addChild(this->m_pLayer);
        auto d = dir->getTouchDispatcher();
        d->addTargetedDelegate(this, -500, true);

        auto win_size = dir->getWinSize();
        auto background = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png");
        background->setContentSize(cocos2d::CCSize(450.0f, 300.0f));
        background->setPosition(win_size.width / 2, win_size.height / 2);
        this->m_pLayer->addChild(background);
/*
        auto col2 = ButtonSprite::create("Col2", 30, 10, 10, 1, "bigFont.fnt", "GJ_button_04.png", 20);
        auto color_menu = CCMenu::create();
        {
            auto col1 = ButtonSprite::create("Col1", 30, 10, 10, 1, "bigFont.fnt", "GJ_button_01.png", 20);
            this->clr1Btn = CCMenuItemSpriteExtra::create(
                col1,
                nullptr,
                this,
                static_cast<cocos2d::SEL_MenuHandler>(&MPCreateLobbyPopup::onColor1Click));
*/

    return true;
    }
return false;
}