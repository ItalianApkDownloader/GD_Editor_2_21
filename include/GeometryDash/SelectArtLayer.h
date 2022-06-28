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
    CLASS_MEMBER(cocos2d::CCMenu*, bgSelectMenu, 0x1B0);
    CLASS_MEMBER(cocos2d::CCArray*, someArray, 0x1E8);
	CLASS_MEMBER(int, type, 0x1F8)

public:
    void selectArt(cocos2d::CCObject*);
    void onClose(cocos2d::CCObject*);
};