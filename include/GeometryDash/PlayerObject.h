#pragma once

#include "CCSprite.h"
#include "GJBaseGameLayer.h"

enum PlayerButton 
{
    Jump = 1,
    Left = 2,
    Right = 3
};

enum PlayerCollisionDirection
{
    dirTop = 0,
    dirBottom = 1,
    dirLeft = 2,
    dirRight = 3
};

class PlayerObject : public GameObject  // GameObject->CCSpritePlus
{	
public:
    CLASS_MEMBER(bool, isPlatformer, 0xB70);
    CLASS_MEMBER(bool, isGameplayRotated, 0x75B);
    CLASS_MEMBER(bool, isGravityFlipped, 0x757);
    CLASS_MEMBER(bool, inPlayLayer, 0x60C);
    CLASS_MEMBER(bool, isRollMode, 0x753);
    CLASS_MEMBER(bool, isDartMode, 0x754);
    CLASS_MEMBER(double, xVelocityPlatformer, 0xB00);
    CLASS_MEMBER(double, yVelocity, 0x740);
    CLASS_MEMBER(bool, isDashing, 0x770);
    CLASS_MEMBER(float, playerScale, 0x774);

public:
    PlayerObject();
    virtual ~PlayerObject();

    static PlayerObject* create( int playerFrame, int shipFrame, GJBaseGameLayer*,cocos2d::CCLayer* a3,bool );

    void updatePlayerShipFrame( int a1 );

    void updateGlowColor();
    void setSecondColor( const cocos2d::ccColor3B& color );

    void pushButton(PlayerButton btn);
    void releaseButton(PlayerButton btn);
    void switchedDirTo(PlayerButton btn);
    int getActiveMode();

    void hitGround(bool);
    void hitGroundNoJump(bool);
    void updateCollide(PlayerCollisionDirection, int);
    void didHitHead();
    void stopDashing();

    void updateCollideLeft(float, int);
    void updateCollideRight(float, int);
    void updateCollideBottom(float, int);
    void updateCollideTop(float, int);

    void exitPlatformerAnimateJump();

    void setYVelocity(double, int);

    bool isInNormalMode();
    bool isFlying();
    bool isSafeFlip(float);
    void checkSnapJumpToObject(GameObject* obj);
    
    int flipMod();

    // virtual void setVisible( bool visible ); // 0xA8
    void logValues();

    virtual void placeholder1();
    virtual void placeholder2();

    virtual void setColor( const cocos2d::ccColor3B& color ); // 0x1D4
};