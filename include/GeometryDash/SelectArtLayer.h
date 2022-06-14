#pragma once

#include <cocos2dx/CCLayer.h>

#include "FLAlertLayer.h"
#include "GDAPI_Macros.h"

enum SelectArtType {
    background,
    ground
};

class SelectArtLayer : public FLAlertLayer {
public:
    CLASS_MEMBER(cocos2d::CCMenu*, bgSelectMenu, 0x1A4);
    CLASS_MEMBER(cocos2d::CCArray*, someArray, 0x1D8);

public:
    void selectArt(cocos2d::CCObject*);
    void onClose(cocos2d::CCObject*);
};