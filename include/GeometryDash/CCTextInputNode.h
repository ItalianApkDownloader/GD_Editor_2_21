#pragma once

#include "cocos2d.h"

#include "GDAPI_Macros.h"

class CCTextInputNode : public cocos2d::CCLayer {
public:
    CLASS_MEMBER(cocos2d::CCLabelBMFont*, placeholderLabel, 0x174);
	
	void setString(std::string);
};