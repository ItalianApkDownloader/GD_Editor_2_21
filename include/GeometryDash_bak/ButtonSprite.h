#ifndef __BUTTONSPRITE_H__
#define __BUTTONSPRITE_H__

#include "GeometryDash.h"
#include <gd.h>

using namespace cocos2d;

enum ButtonSpriteType {
	kButtonSpriteImage,
	kButtonSpriteText
};

/* Used for creating button sprites */
class GD_DLL ButtonSprite : public CCSprite {
public:
	static ButtonSprite *create(const char *text);
	static ButtonSprite *create(char const*,int,int,float,bool);
	static ButtonSprite *create(char const*,float);
	void setString(const char*);
};

#endif //__BUTTONSPRITE_H__
