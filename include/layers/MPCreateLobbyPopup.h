//
// Created by Marco on 03/11/2021.
//
#pragma once
#ifndef GDPSEDITOR_MPCreateLobbyPopup_H
#define GDPSEDITOR_MPCreateLobbyPopup_H

#include "cocos2d.h"
#include "gd.h"




class MPCreateLobbyPopup : public FLAlertLayer {
    
    MultiplayerLayer* MPL;
  
    static inline bool popupOpen = false;
public:
    static MPCreateLobbyPopup* create(MultiplayerLayer * MPLayer)
    {
        MPCreateLobbyPopup* pRet = new MPCreateLobbyPopup();
        if (pRet && pRet->init(MPLayer)) {
            pRet->autorelease();
            return pRet;
        } else {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }
    bool init(MultiplayerLayer * MPLayer);

    void onClose(cocos2d::CCObject*)
    {
        MPCreateLobbyPopup::popupOpen = false;
        this->setKeyboardEnabled(false);
        this->removeFromParentAndCleanup(true);
    };


    virtual void keyBackClicked() { this->onClose(nullptr); }


};

#endif //GDPSEDITOR_MPCreateLobbyPopup_H
