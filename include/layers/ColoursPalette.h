//
// Created by Marco on 03/11/2021.
//
#pragma once
#ifndef GDPSEDITOR_COLOURSPALETTE_H
#define GDPSEDITOR_COLOURSPALETTE_H

#include "cocos2d.h"
#include "gd.h"




class ColoursPalette : public FLAlertLayer {
    SimplePlayer * cubeIcon;
    SimplePlayer * shipIcon;
    SimplePlayer * ballIcon;
    SimplePlayer * birdIcon;
    SimplePlayer * dartIcon;
    SimplePlayer * robotIcon;
    SimplePlayer * spiderIcon;
    SimplePlayer * swingIcon;
    CCMenuItemSpriteExtra * clr1Btn;
    CCMenuItemSpriteExtra * clr2Btn;
    GJGarageLayer * garageLayer;
    int colorType = 0;
    bool glow = false;
    CCSprite * color1Selector = nullptr;
    CCSprite * color2Selector = nullptr;


    static inline bool popupOpen = false;
public:
    static ColoursPalette* create(GJGarageLayer * garageLayer)
    {
        ColoursPalette* pRet = new ColoursPalette();
        if (pRet && pRet->init(garageLayer)) {
            pRet->autorelease();
            return pRet;
        } else {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }
    bool init(GJGarageLayer * garageLayer);

    void onClose(cocos2d::CCObject*)
    {
        this->garageLayer->updatePlayerColors();
        ColoursPalette::popupOpen = false;
        this->setKeyboardEnabled(false);
        this->removeFromParentAndCleanup(true);
    };

    void addColorButton(CCMenu * father,float posX,float posY,int id);
    void onColorClick(CCObject * sender);
    void onColor1Click(CCObject * sender);
    void onColor2Click(CCObject * sender);
    void onGlow(CCObject * sender);
    virtual void keyBackClicked() { this->onClose(nullptr); }


    static void do_hooks(void * lib);
};

#endif //GDPSEDITOR_COLOURSPALETTE_H
