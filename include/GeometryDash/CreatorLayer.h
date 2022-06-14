#pragma once
#include "cocos2d.h"
#include "CCSprite.h"
#include "WorldSelectLayer.h"
#include "GameManager.h"
#include <gd.h>
#include "GDAPI_Macros.h"
#include "GJListLayer.h"
#include "patch.h"
#include "GDPSManager.h"

class CreatorLayer : public cocos2d::CCLayer 
{
public:
    CLASS_MEMBER(cocos2d::CCSprite*, questBtnSpr, 0x134);

public:
    CreatorLayer();
    virtual ~CreatorLayer();
	static CCScene* scene();
	
	void onMyLevels(CCObject*);
    void onSavedLevels(CCObject*);
	void onFameLevels(CCObject*);
    void onLeaderboards(CCObject*);
    void onChallenge(CCObject*);
    void onMultiplayer(CCObject*);
    void onDailyLevel(CCObject*);
    void onWeeklyLevel(CCObject*);
    void onGauntlets(CCObject*);
    void onFeaturedLevels(CCObject*);
    void onMapPacks(CCObject*);
    void onOnlineLevels(CCObject*);
    void onOnlyFullVersion(CCObject*);

    void checkQuestsStatus();
};

class WorldSelectLayerExt : public WorldSelectLayer
{
public:
    void onGo(cocos2d::CCObject*) {
        auto dir = CCDirector::sharedDirector();
        dir->pushScene(cocos2d::CCTransitionFade::create(0.5, CreatorLayer::scene()));
    }
};