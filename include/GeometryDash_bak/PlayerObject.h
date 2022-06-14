#ifndef __PLAYEROBJECT_H__
#define __PLAYEROBJECT_H__

#include "GeometryDash.h"
#include "cocos2d.h"
#include "GameObject.h"

USING_NS_CC;




enum PlayerButton {
	kPlayerButtonNone,
	kPlayerButtonJump
};

class PlayerObject : public GameObject {
public:

	unsigned char _pad1[0x120]; // padding for CCSprite + 0x10 (0x1e4)
	int field304; // 0x304
	uint8_t _pad308[ 0x3f0 ]; // 0x308
	bool field6f8; // 0x6f8
	uint8_t _pad6f9[ 0x68b ]; // 0x3f9
	int platformerMode; // 0xA84

public:
	PlayerObject();

	static PlayerObject* create( int playerFrame, int shipFrame, cocos2d::CCLayer* a3 );

	void updatePlayerShipFrame( int a1 );

	void updateGlowColor( );
	void setSecondColor( const cocos2d::ccColor3B& color );

	void pushButton(PlayerButton btn);
	void releaseButton(PlayerButton btn);
	void switchedDirTo(PlayerButton btn);

	// virtual void setVisible( bool visible ); // 0xA8

	virtual void placeholder1();
	virtual void placeholder2();

	void setColor( const cocos2d::ccColor3B& color ); // 0x1D4
};

#endif //__PLAYEROBJECT_H__
