
#include <gd.h>
#include "cocos2d.h"
#include "MultiplayerLayerExt.h"

bool MultiplayerLayerExt::init_hk(GJGameLevel *)
{

        auto director = CCDirector::sharedDirector();
        auto winSize = director->getWinSize();

        auto bg = CCSprite::create("GJ_gradientBG.png");

        bg->setPosition(CCPoint(0, winSize.height / 2));
        bg->setScaleX(winSize.width);
        bg->setScaleY(winSize.height / bg->getContentSize().height);

        bg->setColor(ccc3(0, 0x66, 0xFF));

        this->addChild(bg, -2);

        auto misc = CCMenu::create();
        this->addChild(misc);

        auto m_pButtonMenu = CCMenu::create();
        this->addChild(m_pButtonMenu);

        auto backBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
            CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
            this,
            menu_selector(MultiplayerLayer::onBack));
        backBtn->setPosition((-winSize.width / 2) + 25.0f, (winSize.height / 2) - 25.0f);
        misc->addChild(backBtn);

        auto list = GJListLayer::create(NULL, "Versus", ccc4(255, 0, 0, 0), 356, 220);

        list->setPosition(ccp(
            winSize.width / 2 - list->getContentSize().width / 2,
            winSize.height / 2 - list->getContentSize().height / 2));

        auto txt = CCLabelBMFont::create("Nothing to see here...", "bigFont.fnt");
        txt->setPosition(ccp(
            winSize.width / 2,
            winSize.height / 2));
        txt->setScale(0.5);
        this->addChild(txt);

        this->addChild(list);

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
}
