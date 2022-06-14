#include <gd.h>
#include <ColoursPalette.h>
#include "cocos2d.h"
#include "GJGarageLayerExt.h"
#include "AdvancedIconSelect.h"

bool GJGarageLayerExt::init_hk()
{

    extern bool inSettings;
    inSettings = false;
    auto ret = GJGarageLayerExt::init_trp(this);

    auto winsize = CCDirector::sharedDirector()->getWinSize();
    auto dir = CCDirector::sharedDirector();
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(5));
        // icons_node->setPositionX(winsize.width/2);
        //  icons_node->setPositionY(icons_node->getPositionY() - 20);
        icons_node->setVisible(false);
        // icons_node->setPositionY(icons_node->getPositionY() + 10);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(4));
        icons_node->setPositionX(winsize.width / 2);
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(6));
        icons_node->setPositionY(icons_node->getPositionY() - 10);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(7));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(8));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(9));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(10));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(11));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(12));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(13));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }
    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(14));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }

    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(15));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }

    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(3));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }

    {
        auto icons_node = reinterpret_cast<CCNode *>(this->getChildren()->objectAtIndex(2));
        icons_node->setPositionY(icons_node->getPositionY() - 20);
    }

    {
        auto artBottomL = cocos2d::CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        artBottomL->setPosition(ccp(artBottomL->getScaledContentSize().width / 2, artBottomL->getScaledContentSize().height / 2));
        this->addChild(artBottomL);
    }
    {
        auto artBottomL = cocos2d::CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        artBottomL->setPosition(ccp(-artBottomL->getScaledContentSize().width / 2 + winsize.width, artBottomL->getScaledContentSize().height / 2));
        artBottomL->setFlipX(true);
        this->addChild(artBottomL);
    }
    {
        auto palette_menu = CCMenu::create();
        auto paletteSprite = cocos2d::CCSprite::create("GJ_paletteBtn_001.png");
        auto btn = CCMenuItemSpriteExtra::create(
            paletteSprite,
            paletteSprite,
            this,
            static_cast<cocos2d::SEL_MenuHandler>(&GJGarageLayerExt::onPalette));
        palette_menu->addChild(btn);
        btn->setPositionX(-winsize.width / 2 + 30);

        this->addChild(palette_menu);
    }

    {
        auto icons_menu = reinterpret_cast<CCMenu *>(this->getChildren()->objectAtIndex(6));

        auto swingSprite = cocos2d::CCSprite::createWithSpriteFrameName("gj_swingBtn_off_001.png");
        swingSprite->setScale(.9);
        auto swing_btn = CCMenuItemSpriteExtra::create(
            swingSprite,
            swingSprite,
            this,
            static_cast<cocos2d::SEL_MenuHandler>(&AdvancedIconSelect::onSwing));
        auto array = icons_menu->getChildren();
        auto count = array->count();
        auto trails = reinterpret_cast<CCNode *>(array->objectAtIndex(count - 2));
        // swing_btn->setScale(.89);
        swing_btn->setPosition(trails->getPosition());
        icons_menu->addChild(swing_btn);
        trails->setPositionX(trails->getPositionX() + (35 * 2));
    }

    {
        auto icons_menu = reinterpret_cast<CCMenu *>(this->getChildren()->objectAtIndex(6));
        auto array = icons_menu->getChildren();
        auto count = array->count();
        auto deathEffects = reinterpret_cast<CCNode *>(array->objectAtIndex(count - 2));
        auto jetpackSprite = cocos2d::CCSprite::create("jetpack_off.png");
        jetpackSprite->setScale(.85);
        auto jetpack_btn = CCMenuItemSpriteExtra::create(
            jetpackSprite,
            jetpackSprite,
            this,
            static_cast<cocos2d::SEL_MenuHandler>(&AdvancedIconSelect::onJetpack));
        icons_menu->addChild(jetpack_btn);
        // jetpack_btn->setScale(.89);
        jetpack_btn->setPosition(deathEffects->getPosition());
        deathEffects->setPositionX(deathEffects->getPositionX() + (35 * 2));

        icons_menu->setPositionX(icons_menu->getPositionX() + 30);
    }

    auto icons_menu = reinterpret_cast<CCMenu *>(this->getChildren()->objectAtIndex(6));
    auto array = icons_menu->getChildren();
    auto count = array->count();
    auto deathEffects = reinterpret_cast<CCNode *>(array->objectAtIndex(count - 3));
    deathEffects->setPositionY(100000);
    icons_menu->setPositionX(icons_menu->getPositionX() + 15);
    icons_menu->setPositionY(icons_menu->getPositionY() - 5);

    return ret;
}

void GJGarageLayerExt::onPalette(CCObject *sender)
{
    ColoursPalette::create(this)->show();
}