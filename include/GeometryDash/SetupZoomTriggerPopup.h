#pragma once
#include "GDAPI_Macros.h"
#include <cocos2dx/CCLayer.h>
#include <cocos2dx/CCNode.h>

#include "Slider.h"
#include "CCTextInputNode.h"
#include "FLAlertLayer.h"

class SetupZoomTriggerPopup : public FLAlertLayer
{
public:

	PAD(0x1A4)
	CCMenu* menu; //
	PAD(0x1B8)
	CCLayer* layer; //
	PAD(0x1E8)
	CCTextInputNode* zoomtimeInput_; //
	PAD(0x220)
	CCMenuItemToggle* touchToggle_; //
	CCMenuItemToggle* spawnToggle_; //





	CLASS_MEMBER(CCMenu*, menu, 0x1A4);
	CLASS_MEMBER(CCTextInputNode*, zoomtimeInput, 0x1E8);
	CLASS_MEMBER(CCLayer*, layer, 0x1B8);
   // CLASS_MEMBER(Slider*, degreesInputSlider, 0x214);
	

public:
    SetupZoomTriggerPopup();
    virtual ~SetupZoomTriggerPopup();
	void onClose(CCObject*);
	void sliderDegreesChanged(CCObject*);
	void sliderChanged(CCObject*);

    bool init( );
};