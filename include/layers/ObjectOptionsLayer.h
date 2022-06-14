#ifndef OBJOPTLAYER_H
#define OBJOPTLAYER_H
#pragma once

#include "gd.h"
#include "GameObject.h"

class SetGroupIDLayer;

class ObjectOptionsLayer : public FLAlertLayer {
public:
    GameObject* m_pGameObj;
    cocos2d::CCArray* m_pGameObjects;
    SetGroupIDLayer* parent;

    static bool isOpened;

public:
    static ObjectOptionsLayer* create(SetGroupIDLayer* parent, GameObject* object, cocos2d::CCArray* objectsInvolved) {
        ObjectOptionsLayer* pRet = new ObjectOptionsLayer();
        if (pRet && pRet->init(parent, object, objectsInvolved)) {
            pRet->autorelease();
            return pRet;
        } else {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }
    bool init(SetGroupIDLayer*, GameObject*, cocos2d::CCArray*);

    void onClose(CCObject*)
    {
        CCDirector::sharedDirector()->getTouchDispatcher()->decrementForcePrio(2);
        this->setTouchEnabled(false);
        this->setKeypadEnabled(false);
        ObjectOptionsLayer::isOpened = false;
        this->removeFromParentAndCleanup(true);
    };
    virtual void keyBackClicked() { this->onClose(nullptr); }

    void createToggleButton(const char*, CCPoint, CCObject*, cocos2d::SEL_MenuHandler, CCMenu*, bool, bool);

    // toggle callbacks
    void onDontFade(CCObject*);
    void onDontEnter(CCObject*);
    void onHideEffects(CCObject*);
    void onGroupParent(CCObject*);
    void onHighDetail(CCObject*);

    void onUntouchable(CCObject*);
    void onPassable(CCObject*);
    void onHide(CCObject*);
    void onNonStick(CCObject*);
    void onIceBlock(CCObject*);
};

#endif