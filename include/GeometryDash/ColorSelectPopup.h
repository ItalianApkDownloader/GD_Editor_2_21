#pragma once
#include "GDAPI_Macros.h"
#include <cocos2dx/CCLayer.h>
#include <cocos2dx/CCNode.h>

#include "Slider.h"
#include "CCTextInputNode.h"
#include "FLAlertLayer.h"

class ColorSelectPopup : public FLAlertLayer
{
public:



	CLASS_MEMBER(CCMenu*, menu, 0x1A4);
	

public:
    ColorSelectPopup();
    virtual ~ColorSelectPopup();
	void onClose(CCObject*);
	void sliderDegreesChanged(CCObject*);
	void sliderChanged(CCObject*);

    bool init( );
};