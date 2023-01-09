#pragma once

#include "cocos2d.h"

#include "GDAPI_Macros.h"

class CCTextInputNode : public cocos2d::CCLayer {
public:
    CLASS_MEMBER(cocos2d::CCLabelBMFont*, placeholderLabel, 0x174);
	
	void setString(std::string);
	std::string getString();
	void updateLabel(std::string);
	static CCTextInputNode* create(float, float, const char*, const char*, int, const char*);
	void setMaxLabelWidth(float);
};