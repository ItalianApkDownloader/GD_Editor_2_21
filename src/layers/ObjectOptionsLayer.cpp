#include "ObjectOptionsLayer.h"

#include <gd.h>
#include <cocos2dx/extensions/CCScale9Sprite.h>
#include <cocos2d.h>

#include "CCMenuItemToggler.h"
#include "InfoAlertButton.h"

#include "SetGroupIDLayer.h"

using namespace cocos2d;

bool ObjectOptionsLayer::isOpened = false;

bool ObjectOptionsLayer::init(SetGroupIDLayer *parent, GameObject *object, CCArray *objectsInvolved)
{
    if (!this->initWithColor({0, 0, 0, 105}))
        return false;

    if (!ObjectOptionsLayer::isOpened)
    {
        if (object != nullptr)
            this->m_pGameObj = object;
        else
            this->m_pGameObjects = objectsInvolved;

        this->parent = parent;

        auto dir = CCDirector::sharedDirector();
        auto winSize = dir->getWinSize();

        auto layer = CCLayer::create();
        this->m_pLayer = layer;

        this->addChild(layer);

        auto menu = CCMenu::create();
        menu->setPosition(0, 0);

        layer->addChild(menu, 3);

        // bg
        auto bg = extension::CCScale9Sprite::create("GJ_square02.png", {0, 0, 80, 80});
        bg->setContentSize(CCSize(390, 310));
        bg->setPosition(winSize.width / 2, winSize.height / 2);

        layer->addChild(bg, 1);

        // title label
        auto titleLabel = CCLabelBMFont::create("Object Options", "goldFont.fnt");
        titleLabel->setScale(.7);
        titleLabel->setPosition(winSize.width / 2, winSize.height / 2 + 138);
        layer->addChild(titleLabel, 2);

        // ok button
        auto okBtnSpr = ButtonSprite::create("OK", 42, 0, .8, true, "goldFont.fnt", "GJ_button_01.png", 30);
        auto okBtn = CCMenuItemSpriteExtra::create(okBtnSpr, okBtnSpr, this, menu_selector(ObjectOptionsLayer::onClose));
        okBtn->setPosition(winSize.width / 2, winSize.height / 2 - 132);

        menu->addChild(okBtn);

        // info button
        auto infoBtn = InfoAlertButton::create("Object Options", "I literally have no clue what goes here", 1);
        infoBtn->setPosition(winSize.width / 2 - 175, winSize.height / 2 + 135);

        menu->addChild(infoBtn);

        // toggles
        auto togglesMenu = CCMenu::create();

        // - left
        createToggleButton("Dont\nFade", CCPoint(-130, 90), this, menu_selector(ObjectOptionsLayer::onDontFade), togglesMenu, parent->_dontFadeToggled(), true);           // Dont Fade
        createToggleButton("Dont\nEnter", CCPoint(-130, 50), this, menu_selector(ObjectOptionsLayer::onDontEnter), togglesMenu, parent->_dontEnterToggled(), true);        // Dont Enter
        createToggleButton("No\nEffects", CCPoint(-130, 10), this, menu_selector(ObjectOptionsLayer::onHideEffects), togglesMenu, parent->_hideEffectsToggled(), true);    // No Effects
        createToggleButton("Group\nParent", CCPoint(-130, -30), this, menu_selector(ObjectOptionsLayer::onGroupParent), togglesMenu, parent->_groupParentToggled(), true); // Group Parent
        createToggleButton("High\nDetail", CCPoint(-130, -70), this, menu_selector(ObjectOptionsLayer::onHighDetail), togglesMenu, parent->_highDetailToggled(), true);    // High Detail

        // - right
        createToggleButton("Untouchable", CCPoint(50, 90), this, menu_selector(ObjectOptionsLayer::onUntouchable), togglesMenu, parent->_untouchableToggled(), true); // Untouchable
        createToggleButton("Passable", CCPoint(50, 50), this, menu_selector(ObjectOptionsLayer::onPassable), togglesMenu, parent->_passableToggled(), true);          // Passable
        createToggleButton("Hide", CCPoint(50, 10), this, menu_selector(ObjectOptionsLayer::onHide), togglesMenu, parent->_hideToggled(), true);                      // Hide
        createToggleButton("NonStick", CCPoint(50, -30), this, menu_selector(ObjectOptionsLayer::onNonStick), togglesMenu, parent->_nonStickToggled(), true);         // NonStick
        createToggleButton("IceBlock", CCPoint(50, -70), this, menu_selector(ObjectOptionsLayer::onIceBlock), togglesMenu, parent->_iceBlockToggled(), true);         // IceBlock

        this->m_pLayer->addChild(togglesMenu, 5);

        // fix touch
        dir->getTouchDispatcher()->incrementForcePrio(2);
        dir->getTouchDispatcher()->addTargetedDelegate(this, -500, true);
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        ObjectOptionsLayer::isOpened = true;
    }

    return true;
}

void ObjectOptionsLayer::onDontFade(CCObject *pSender)
{
    this->parent->_dontFadeToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_dontFadeToggled() = this->parent->_dontFadeToggled();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_dontFadeToggled() = this->parent->_dontFadeToggled();

            i++;
        }
    }
}

void ObjectOptionsLayer::onDontEnter(CCObject *pSender)
{
    this->parent->_dontEnterToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_dontEnterToggled() = this->parent->_dontEnterToggled();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_dontEnterToggled() = this->parent->_dontEnterToggled();

            i++;
        }
    }
}

void ObjectOptionsLayer::onHideEffects(CCObject *pSender)
{
    this->parent->_hideEffectsToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_hideEffectsToggled() = this->parent->_hideEffectsToggled();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_hideEffectsToggled() = this->parent->_hideEffectsToggled();

            i++;
        }
    }
}

void ObjectOptionsLayer::onGroupParent(CCObject *pSender)
{
    this->parent->_groupParentToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_groupParentToggled() = this->parent->_groupParentToggled();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_groupParentToggled() = this->parent->_groupParentToggled();

            i++;
        }
    }
}

void ObjectOptionsLayer::onHighDetail(CCObject *pSender)
{
    this->parent->_highDetailToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_highDetailToggled() = this->parent->_highDetailToggled();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_highDetailToggled() = this->parent->_highDetailToggled();

            i++;
        }
    }
}

void ObjectOptionsLayer::onUntouchable(CCObject *pSender)
{
    this->parent->_untouchableToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_untouchableToggled() = this->parent->_untouchableToggled();

        this->m_pGameObj->saveActiveColors();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_untouchableToggled() = this->parent->_untouchableToggled();

            object->saveActiveColors();

            i++;
        }
    }
}

void ObjectOptionsLayer::onPassable(CCObject *pSender)
{
    this->parent->_passableToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_passableToggled() = this->parent->_passableToggled();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_passableToggled() = this->parent->_passableToggled();

            i++;
        }
    }
}

void ObjectOptionsLayer::onHide(CCObject *pSender)
{
    this->parent->_hideToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_hideToggled() = this->parent->_hideToggled();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_hideToggled() = this->parent->_hideToggled();

            i++;
        }
    }
}

void ObjectOptionsLayer::onNonStick(CCObject *pSender)
{
    this->parent->_nonStickToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_nonStickToggled() = this->parent->_nonStickToggled();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_nonStickToggled() = this->parent->_nonStickToggled();

            i++;
        }
    }
}

void ObjectOptionsLayer::onIceBlock(CCObject *pSender)
{
    this->parent->_iceBlockToggled() = reinterpret_cast<CCMenuItemToggler *>(pSender)->_isToggled();

    // single object
    if (this->m_pGameObj != nullptr)
    {
        this->m_pGameObj->_iceBlockToggled() = this->parent->_iceBlockToggled();
    }
    // multiple objects
    else
    {
        int i = 0;
        while (i < this->m_pGameObjects->count())
        {
            auto object = (GameObject *)this->m_pGameObjects->objectAtIndex(i);
            object->_iceBlockToggled() = this->parent->_iceBlockToggled();

            i++;
        }
    }
}

void ObjectOptionsLayer::createToggleButton(const char *text, CCPoint position, CCObject *baseclass, cocos2d::SEL_MenuHandler callback, CCMenu *menu, bool toggled, bool enabled)
{
    auto onSpr = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto offSpr = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

    onSpr->setScale(.7);
    offSpr->setScale(.7);

    auto toggle = CCMenuItemToggler::create(
        onSpr,
        offSpr,
        this,
        callback);
    toggle->setSizeMult(1.5);
    toggle->toggle(!toggled);

    auto label = CCLabelBMFont::create(text, "bigFont.fnt");
    label->limitLabelWidth(80, 0.35, 0);
    label->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentLeft);

    toggle->setPosition(position);
    label->setPosition(toggle->getPositionX() + 17.5 + label->getScaledContentSize().width / 2, toggle->getPositionY());

    menu->addChild(toggle);
    menu->addChild(label);
}