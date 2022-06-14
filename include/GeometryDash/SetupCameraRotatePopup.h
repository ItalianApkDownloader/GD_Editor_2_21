#pragma once
#include "GDAPI_Macros.h"
#include <cocos2dx/CCLayer.h>
#include <cocos2dx/CCNode.h>

#include "Slider.h"
#include "FLAlertLayer.h"
#include "CCTextInputNode.h"

class SetupCameraRotatePopup : public FLAlertLayer
{
public:
	CLASS_MEMBER(cocos2d::CCLayer*, m_pLayer, 0x1B8);
	CLASS_MEMBER(CCTextInputNode*, degreesInput, 0x208);
    CLASS_MEMBER(Slider*, degreesInputSlider, 0x214);

public:
    SetupCameraRotatePopup();
    virtual ~SetupCameraRotatePopup();
	void onClose(CCObject*);
	void sliderDegreesChanged(CCObject*);
	void sliderChanged(CCObject*);

    bool init( );
};