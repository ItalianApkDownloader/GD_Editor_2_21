#pragma once

#include "GDAPI_Macros.h"
#include "../layers/ObjectOptionsLayer.h"

class CCTextInputNode;

class SetGroupIDLayer : public cocos2d::CCLayerColor {
public:
    CLASS_MEMBER(cocos2d::CCLayer*, m_pLayer, 0x1B8);
    CLASS_MEMBER(GameObject*, object, 0x1DC);
    CLASS_MEMBER(cocos2d::CCArray*, objectsInvolved, 0x1E0);

    CLASS_MEMBER(bool, dontFadeToggled, 0x21C);
    CLASS_MEMBER(bool, dontEnterToggled, 0x21D);
    CLASS_MEMBER(bool, hideEffectsToggled, 0x21E);
    CLASS_MEMBER(bool, groupParentToggled, 0x22C);
    CLASS_MEMBER(bool, highDetailToggled, 0x21B);

    CLASS_MEMBER(bool, untouchableToggled, 0x220);
    CLASS_MEMBER(bool, passableToggled, 0x221);
    CLASS_MEMBER(bool, hideToggled, 0x222);
    CLASS_MEMBER(bool, nonStickToggled, 0x223);
    CLASS_MEMBER(bool, iceBlockToggled, 0x224);

    CLASS_MEMBER(CCTextInputNode*, orderInput, 0x1FC);

public:
    // custom func
    void onExtra(cocos2d::CCObject*) {
        auto layer = ObjectOptionsLayer::create(this, this->_object(), this->_objectsInvolved());
        layer->show();
        layer->m_pLayer->stopAllActions();
        layer->m_pLayer->setScale(1);
        layer->setColor(ccc3(0, 0, 0));
        layer->setOpacity(105);
    }
};