#pragma once

#include "CCLayer.h"
#include "CCMenu.h"
#include "CCNode.h"

#include "ButtonSprite.h"
#include "FLAlertLayerProtocol.h"

class FLAlertLayer : public cocos2d::CCLayerColor{
public:
    cocos2d::CCMenu* m_pButtonMenu;
    int m_nControlConnected; //?
    cocos2d::CCObject* m_pTarget;
    bool m_bReverseKeyBack;
    cocos2d::ccColor3B m_cColor;
    PAD(4);
    cocos2d::CCLayer* m_pLayer;
    int m_nZOrder2;
    bool m_bNoElasticity;
    cocos2d::ccColor3B m_cColor2; //?
    ButtonSprite* m_pButton1;
    ButtonSprite* m_pButton2;
    void* m_pScrollingLayer;
    int m_nJoystickConnected;
    bool m_bBorder; //?
    bool m_bNoAction; //?

public:
    FLAlertLayer();
    ~FLAlertLayer();

    static FLAlertLayer* create( FLAlertLayerProtocol* protocol, const char* title, std::string caption, const char* button_title, const char* button2_title );
    static FLAlertLayer* create( FLAlertLayerProtocol* protocol, const char* title, std::string caption, const char* b1_title, const char* b2_title, float x_size, bool add_border, float y_size );

    virtual void onEnter(void);
    virtual bool ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
    virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
    virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
    virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
    virtual void registerWithTouchDispatcher(void);
    virtual void keyBackClicked(void);
    virtual void keyDown(cocos2d::enumKeyCodes);
    virtual void show(void);
};
