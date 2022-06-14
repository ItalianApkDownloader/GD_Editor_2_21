#include "GeometryDash.h"
#include "cocos2d.h"

USING_NS_CC;

/* Main menu for user-created levels */
class GD_DLL SimplePlayer : public CCSprite {
public:
	~SimplePlayer();
	void updateColors();
	static SimplePlayer *create(int);
	
	void setSecondColor(cocos2d::_ccColor3B const&);
	
	
};