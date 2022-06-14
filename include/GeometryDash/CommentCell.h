#pragma once

#include <cocos2dx/CCLayer.h>

#include "GDAPI_Macros.h"
#include "TableViewCell.h"
class GJComment : public cocos2d::CCObject {
public:
    CLASS_MEMBER(int, modBadge, 0x124);
	CLASS_MEMBER(CCLabelBMFont*, percentage, 0x20C);
};

// not fucking sure if this is a CCLayer but im not even gonna bother it doesnt matter
class CommentCell : public TableViewCell {
public:
    CLASS_MEMBER(cocos2d::CCMenu*, menu, 0x16C);
};