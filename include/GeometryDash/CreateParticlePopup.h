#pragma once

#include "FLAlertLayer.h"
#include "GameObject.h"

#include "GDAPI_Macros.h"

class ParticleGameObject : public GameObject 
{
public:
    ParticleGameObject();
    virtual ~ParticleGameObject();

    static ParticleGameObject* create(  );
};

class CreateParticlePopup : public FLAlertLayer
{
public:
    CLASS_MEMBER(cocos2d::CCArray*, textAndStuff, 0x270);

public:
    CreateParticlePopup();
    virtual ~CreateParticlePopup();

    static CreateParticlePopup* create( ParticleGameObject* a1, cocos2d::CCArray* a2 );

    cocos2d::CCDictionary* getPageInputNodes(int page);
    cocos2d::CCDictionary* getPageSliders(int page);
};
