#pragma once
#include <cocos2d.h>
#include <gd.h>

class ShareCommentLayer : public cocos2d::CCLayerColor
{
public:

public:
	
	inline CCMenu* buttonMenu() {return MBO(CCMenu*, this, 0x1B4); }
	
	inline int someInt1() { return MBO(int, this, 0x1F8); }
	inline int someInt2() { return MBO(int, this, 0x1FC); }
	
	inline int commentType() { return MBO(int, this, 0x200); }
	inline CCTextInputNode* inputNode() { return MBO(CCTextInputNode*, this, 0x204); }
	inline CCLabelBMFont* maxAllowedLabel() {return MBO(CCLabelBMFont*, this, 0x210); }
	
	void onEmojiPicker(CCObject*);
	void onEmoji(CCObject*);
	void updateDescText(const char*);
	void updatePreviewLabel(std::string);
};