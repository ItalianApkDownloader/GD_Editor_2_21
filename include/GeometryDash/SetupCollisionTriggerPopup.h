#pragma once
#include "GDAPI_Macros.h"
#include <cocos2dx/CCLayer.h>
#include <cocos2dx/CCNode.h>

#include "Slider.h"
#include "CCTextInputNode.h"
#include "FLAlertLayer.h"

class SetupCollisionTriggerPopup : public FLAlertLayer
{
public:

public:
    SetupCollisionTriggerPopup();
    virtual ~SetupCollisionTriggerPopup();

    bool init( );
};