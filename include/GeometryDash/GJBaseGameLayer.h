#pragma once

#include "obb2d.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "LevelSettingsObject.h"

#include "CCSpriteBatchNode.h"
#include "CCLayer.h"
#include "CCScene.h"
#include "TriggerEffectDelegate.h"
#include "GameObject.h"
#include "GJEffectManager.h"
#include "particle_nodes/CCParticleSystemQuad.h"

class PlayerObject;
class DrawGridLayer;

// size 0x3F8
class GJBaseGameLayer : public cocos2d::CCLayer, TriggerEffectDelegate
{
public:
    CCArray* unk_00_01;    //0x76 - 304
    CCArray* unk_01_01;    //0x77 - 308
    GJGameLevel* _level;    //0x78 - 312
    bool unk_01;
    bool unk_02;
	bool ignoreDamage_; // 318
	bool unk_06;
	PAD(152);
    cocos2d::CCSpriteBatchNode* batchNode_;
	PAD(308);
	cocos2d::CCSpriteBatchNode* batchNodePlayer_;  //probably idk
	PAD(28);
	PlayerObject* player1;               //204
	PlayerObject* player2;				 //205
    LevelSettingsObject* levelSettings_; //206
    PAD(4);
    CCArray* unk_arr_209;
    CCArray* sections;
    CCArray* unk_arr_211;
	PAD(196);
	CCArray* arr_260;   //260
	CCArray* arr_261;   //260
	PAD(60);
	CCArray* arr_01;    //277
	CCArray* arr_02;    //278
	CCArray* arr_03;    //279
	PAD(16)
    cocos2d::CCLayer * gameLayer_;
	PAD(10001);

    CLASS_MEMBER(vector<int>,something,2778*4);
	CLASS_MEMBER(GJEffectManager*, effectManager, 0x158);

	
public:
    CLASS_MEMBER(PlayerObject*, player1, 0x334);
    CLASS_MEMBER(PlayerObject*, player2, 0x338);

    CLASS_MEMBER(GJGameLevel*, gameLevel, 0x13C);

    CLASS_MEMBER(CCSpriteBatchNode*, batchNode, 0x200);

    CLASS_MEMBER(LevelSettingsObject*, levelSettings, 0x33C);

    CLASS_MEMBER(CCLayer*, gameLayer, 0x48C);

public:
    GJBaseGameLayer();
    virtual ~GJBaseGameLayer();

    void releaseButton( int a1, bool b2 );
    void pushButton( int a1, bool b2 );

    void setupLayers( void );
    void createTextLayers( void );
    void enableHighCapacityMode( void );
    void resetGroupCounters( bool reset );
    void createPlayer();

    void createGroundLayer( int,int);
	void createBackground(int );
    void createMiddleground(int );

    void updateGroundWidth(bool);
    void sortStickyGroups( );
    GameObject* parentForZLayer( int a1, bool a2, int a3 );

    void tweenValue( float from, float to, int tag, std::string property_key, float duration, int action, float f_rate );

    void resetLevelVariables();

    void queueButton(int,bool,bool);
    void processAreaVisualActions();

    virtual bool init( void );

    virtual void updateTweenAction( float, const char* );
    virtual void objectsCollided( int, int );
    virtual cocos2d::CCObject* createMoveCommand( cocos2d::CCPoint, int, float, int, float, bool, bool, bool, bool, float, float, int );
    virtual void updateColor( cocos2d::_ccColor3B, float, int, bool, float, int, int, bool, int, cocos2d::CCObject* );
    virtual void flipGravity( PlayerObject*, bool, bool );
    virtual void calculateColorValues( cocos2d::CCObject*, cocos2d::CCObject*, int, float, ColorActionSprite*, GJEffectManager* );
    virtual void toggleGroupTriggered( int, bool, int );
    virtual void spawnGroup( int, int );
    virtual void activateEndTrigger( int, bool, bool );
    virtual void activatePlayerControlTrigger( void );
    virtual void addToSection( GameObject* );
    virtual void addToGroup( GameObject*, int, bool );
    virtual void removeFromGroup( GameObject*, int );
    virtual void toggleGroundVisibility( bool );
    virtual void updateZoom( int, float, int, float );
    virtual void updateScreenRotation( int, float, int, float );
    virtual void updateCameraOffsetX( float, float, int, float );
    virtual void updateCameraOffsetY( float, float, int, float );
    virtual bool playerReversedDirection( GameObject* );
    virtual void reverseDirection( GameObject* );
    virtual void updateTimeWarp( float );
    virtual void playGravityEffect( bool );
    virtual cocos2d::CCParticleSystemQuad* createCustomParticle( std::string );
    virtual cocos2d::CCParticleSystemQuad* claimCustomParticle( std::string, int, int );
    virtual void unclaimCustomParticle( std::string, cocos2d::CCParticleSystemQuad* );
}; 