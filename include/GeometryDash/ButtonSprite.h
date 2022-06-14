#pragma once

#include "CCSprite.h"

class ButtonSprite : public cocos2d::CCSprite 
{
public:
    

public:
    ButtonSprite();
    virtual ~ButtonSprite();

    static ButtonSprite* create( cocos2d::CCSprite* sprite );
    static ButtonSprite* create( const char* sprite );
    static ButtonSprite* create( const char* sprite, float size );
    static ButtonSprite* create( char const* label, int type, int a1, float size, bool a2 );
    //ButtonSprite::create(char const*,int,int,float,bool,char const*,char const*,float)
    static ButtonSprite* create( char const* label , int a2, int a3, float fontSize, bool dontMatchSize, char const* font, char const* btnTexture, float height );
};