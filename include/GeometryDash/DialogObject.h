#pragma once

#include "CCNode.h"
#include "FLAlertLayer.h"

class DialogObject : public cocos2d::CCObject
{
public:

public:
     DialogObject();
    virtual ~DialogObject();

	static DialogObject*  create(std::string title, std::string text, int type, float text_scale, bool is_unskippable, _ccColor3B color);
	static bool init(std::string title, std::string text, int type, float text_scale, bool is_unskippable, _ccColor3B color);

};