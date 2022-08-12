
#include <gd.h>
#include "cocos2d.h"
#include "MultiplayerLayerExt.h"
#include "MultiplayerLayer.h"
#include "hooking.h"
#include "MPCreateLobbyPopup.h"

bool MultiplayerLayerExt::init_hk(GJGameLevel *)
{

        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();

        auto bg = CCSprite::create("GJ_gradientBG.png");

        bg->setPosition(CCPoint(0, winSize.height / 2));
        bg->setScaleX(winSize.width);
        bg->setScaleY(winSize.height / bg->getContentSize().height);

        bg->CCSprite::setColor(ccc3(0, 0x66, 0xFF));

        this->addChild(bg, -2);

        auto misc = CCMenu::create();
        this->addChild(misc);

        auto btnMenu = CCMenu::create();
        this->addChild(btnMenu);

        auto arrow = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");

        auto backBtn = CCMenuItemSpriteExtra::create(
            arrow,
            arrow,
            this,
            menu_selector(MultiplayerLayer::onBack));
        backBtn->setPosition((-winSize.width / 2) + 25.0f, (winSize.height / 2) - 25.0f);
        misc->addChild(backBtn);
        
        auto createBtn = CCMenuItemSpriteExtra::create(
            arrow,
            arrow,
            this,
            menu_selector(MultiplayerLayerExt::onCreateLobby));
        createBtn->setPositionY(+150);
        btnMenu->addChild(createBtn);
        

        
        auto joinBtn = CCMenuItemSpriteExtra::create(
            arrow,
            arrow,
            this,
            menu_selector(MultiplayerLayerExt::onJoinLobby));
        createBtn->setPositionY(-50);
        btnMenu->addChild(joinBtn);

        auto list = GJListLayer::create(NULL, "Versus", ccc4(255, 0, 0, 0), 356, 220);

        list->setPosition(ccp(
            winSize.width / 2 - list->getContentSize().width / 2,
            winSize.height / 2 - list->getContentSize().height / 2));


        this->addChild(list);

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
}

    void MultiplayerLayerExt::onCreateLobby(CCObject* sender) {
          //  MPCreateLobbyPopup::create(this)->show();

}
    void MultiplayerLayerExt::onJoinLobby(CCObject* sender) {
        
}



void MultiplayerLayerExt::ApplyHooks() {
    
    
    HOOK_STATIC("_ZN16MultiplayerLayer4initEv", 
MultiplayerLayerExt::init_hk, MultiplayerLayerExt::init_trp);

}
