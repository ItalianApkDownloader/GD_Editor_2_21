#pragma once

#include <cocos2dx/CCLayer.h>

#include "FLAlertLayer.h"
#include "GDAPI_Macros.h"

enum SelectArtType {
    background,
    ground,
    mg,
    smartTemplate
};

class SelectArtLayer : public FLAlertLayer {
public:
    CLASS_MEMBER(cocos2d::CCMenu*, bgSelectMenu, 0x1B4);
    CLASS_MEMBER(cocos2d::CCArray*, someArray, 0x1EC);
	CLASS_MEMBER(int, type, 0x1FC)

public:
    void selectArt(cocos2d::CCObject*);
    void onClose(cocos2d::CCObject*);
};