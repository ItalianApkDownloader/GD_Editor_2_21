#pragma once

#include "CCSprite.h"
#include "GameManager.h"

class GJRobotSprite;
class GJSpiderSprite;

class __attribute__( (aligned( 4 )) ) SimplePlayer : public cocos2d::CCSprite
{
public:
	cocos2d::CCSprite* player_;				// 0x1F4
	cocos2d::CCSprite* playerOutline_;
	cocos2d::CCSprite* playerOutline2_;
	cocos2d::CCSprite* playerGlow_;
	cocos2d::CCSprite* playerOutline3_;
	GJRobotSprite* robot_; 
	GJSpiderSprite* spider_;
	int special_;
	bool useGlow_;

public:
	SimplePlayer( void );
	void updatePlayerFrame(int,IconType);
	virtual ~SimplePlayer( void );

	static SimplePlayer* create( int icon );
	void setColor(cocos2d::_ccColor3B const&);
	void setSecondColor(cocos2d::_ccColor3B const&);
	void updateColors();

	inline void setPlayerGlow(bool n_glow)
	{
		auto glow_ptr = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x214);
		*glow_ptr = n_glow;
	};

    inline bool getPlayerGlow()
    {
        auto glow_ptr = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x214);
        return *glow_ptr;
    };
};