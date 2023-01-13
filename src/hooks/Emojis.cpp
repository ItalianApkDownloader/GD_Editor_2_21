//#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"
#include "patch.h"
#include "GDPSHelper.h"
#include "GDPSManager.h"
#include "FunctionHelper.h"
#include "gd.h"
#include "../definitions.h"
#include "ShareCommentLayer.h"
#include <cocos2dx/extensions/CCScale9Sprite.h>

constexpr int kNumEmoji {15};
constexpr int kColumns {5};
constexpr int kEmojiSpacing {30};
constexpr int kVerticalSpacing {30};

int modifyString(std::string& s, std::string& s1,
				std::string& s2)
{
	int times = 0;
	std::string ans = "";
	for (int i = 0; i < s.length(); i++) {
		int k = 0;
		if (s[i] == s1[k]
			&& i + s1.length()
				<= s.length()) {
			int j;
			for (j = i; j < i + s1.length(); j++) {

				if (s[j] != s1[k]) {
					break;
				}
				else {
					k = k + 1;
				}
			}
			if (j == i + s1.length()) {
				times++;
				ans.append(s2);
				i = j - 1;
			}
			else {
				ans.push_back(s[i]);
			}
		}
		else {
			ans.push_back(s[i]);
		}
	}
	s = ans;
	return times;
	//fmtlog("ans: {}", ans);
}

		
const char* getEmoji(int n, bool text) {
	
	static const char* chars[] = {
		"♈", "♉", "♊", "♋", "♌", "♍", "♎", "♏", "♐", "♑", "♒", "♓", "♿", "⛎", "✅"
	};

	static const char* emojiText[] = {
		"trollskullirl", "gato_surprise", "sphere_extravagant", "gato_que", "TheRock", "cube_ok", "skull", "quepedo", "trollface", "GD_dislike", "sphere_dead", "GD_like", "gato_thumbsUp", "XD", "sphere_issue"
	};

	if(n < 0 || n > 14) return "";

	return text ? emojiText[n] : chars[n];
}

void ShareCommentLayer::onEmoji(CCObject* sender) {

	auto input = MBO(CCTextInputNode*, this, 0x204);
	auto str = MBO(std::string, this, 0x208);
	str = fmt::format("{}:{}:", str, getEmoji(sender->CCObject::getTag(), true));
	input->setString(str);
	auto layer = (CCLayer*)this->getChildByTag(1938);
	auto label = (CCLabelBMFont*)layer->getChildByTag(1945);
	if(!label) return;
	
	//this->updatePreviewLabel(str);
}

class TextArea : public cocos2d::CCSprite {

public:
	static TextArea* create(std::string,char const*,float,float,cocos2d::CCPoint,float,bool);
	void setString(std::string);
};

void ShareCommentLayer::updatePreviewLabel(std::string previewStr) {
	
	auto layer = (CCLayer*)this->getChildByTag(1938); if(!layer) return;
	auto label = (CCLabelBMFont*)layer->getChildByTag(1945); if(!label) return;
	

	for(int i = 0; i < 15; i++) {
		std::string s1 = fmt::format(":{}:", getEmoji(i, true));
		std::string s2 = getEmoji(i, false);
	//	fmtlog("preview: {}, s1: {}, s2: {}", previewStr, s1, s2);
		modifyString(previewStr, s1, s2);
	}
	
	label->CCLabelBMFont::setString(previewStr.c_str());

}

FUNCTIONHOOK(void, ShareCommentLayer_updateDescLabel, ShareCommentLayer* self, const char* str) {
	ShareCommentLayer_updateDescLabelO(self, str);
	self->updatePreviewLabel(str);
}




FUNCTIONHOOK(void, ShareCommentLayer_onShare, ShareCommentLayer* self, CCObject* sender) {
	
	auto layer = (CCLayer*)self->getChildByTag(1938); if(!layer) return;
	auto label = (CCLabelBMFont*)layer->getChildByTag(1945); if(!label) return;
	auto str = label->CCLabelBMFont::getString();
	
	
	MBO(std::string, self, 0x208) = str;
	//fmtlog("{}", str);
	ShareCommentLayer_onShareO(self, sender);
}




void ShareCommentLayer::onEmojiPicker(CCObject*) {
	
	auto layer = CCLayer::create();
	layer->setTag(1938);
	
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint mainPos {winSize.width / 2 + 190, winSize.height / 2 - 50};
	
	auto bg = extension::CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
	bg->setPosition(mainPos);
	bg->setZOrder(-1);
	bg->setContentSize(CCSize(170, 120));
	layer->addChild(bg);
	
	auto menu = CCMenu::create();
	menu->setPosition(mainPos);
	layer->addChild(menu);
	

	CCPoint previewPos = {mainPos.x - 240, mainPos.y};
	auto str = MBO(std::string, this, 0x208);
	
	auto label = CCLabelBMFont::create("AAAAAAAAAAAAAAAAAAAA", "chatFont.fnt");
	label->setPosition(previewPos.x, previewPos.y + 30);
	label->setTag(1945);
	label->CCLabelBMFont::limitLabelWidth(10, 0.8f, 0.5f);
	label->setLineBreakWithoutSpace(false);
	layer->addChild(label);
	


	
	auto previewbg = extension::CCScale9Sprite::create("GJ_square01.png", {0, 0, 80, 80});
	previewbg->setPosition(previewPos);
	previewbg->setZOrder(-1);
	previewbg->setContentSize(CCSize(295, 120));
	layer->addChild(previewbg);
	
	
	CCPoint startPos {-30, 50};
	for (int i = 0; i < kNumEmoji; ++i) {
		if (i % kColumns == 0) {
			startPos.y -= kVerticalSpacing;
			startPos.x = -68;
		} else {
			startPos.x += kEmojiSpacing;
		}
		
		auto ebg = CCSprite::create("square02b_001.png");
		ebg->setPosition(startPos);
		ebg->setScale(0.3f);
		ebg->CCSprite::setOpacity(100);
		ebg->CCSprite::setColor({0, 0, 0});
		ebg->setContentSize(CCSize(20, 20));
		menu->addChild(ebg);
		
		auto char_sprite = CCLabelBMFont::create(getEmoji(i, false), "chatFont.fnt");
		auto btn = CCMenuItemSpriteExtra::create(char_sprite, nullptr, this, menu_selector(ShareCommentLayer::onEmoji));
		btn->CCObject::setTag(i);
		menu->addChild(btn);
		btn->setPosition(ebg->getPositionX() + 10, ebg->getPositionY() + 8.5);
	}


	this->addChild(layer);
}


FUNCTIONHOOK(bool, ShareCommentLayer_init, ShareCommentLayer* self, std::string a2, int a3, int a4, int a5) {
	if(!ShareCommentLayer_initO(self, a2, a3, a4, a5)) return false;
	
	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = self->buttonMenu();
	
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	
	auto copyBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("btn_chatHistory_001.png"), nullptr, self, menu_selector(ShareCommentLayer::onEmojiPicker));
	copyBtn->setPosition({170, 35});
	menu->addChild(copyBtn);
	//GDPSHelper::createLabels(layer);
	return true;
}

inline bool containsEmoji(const std::string& str) {
	for(int i = 0; i < 15; i++)
	{
		if(str.find(getEmoji(i, false), 0) != std::string::npos)
			return true;
	}
	return false;
}
FUNCTIONHOOK(void*, GJCommentCell_loadFromComment, CCLayer* self, void* comment) {
	auto ret = GJCommentCell_loadFromCommentO(self, comment);
	CCLog("loadFromComment");
	auto commentStr = MBO(std::string, comment, 0x104);
	if(!containsEmoji(commentStr)) return ret;

	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(1);
	int c = layer->getChildrenCount();
	CCLog("emoji found, enter loop");
	for(int i = 0; i < c; i++)
	{
		if(auto label = dynamic_cast<CCLabelBMFont*>(layer->getChildren()->objectAtIndex(i)))
		{
			//GDPSHelper::createLabels(label);
			//CCLog("label");
			if(label->CCLabelBMFont::getString() == commentStr)
			{
				
				std::string str = label->CCLabelBMFont::getString();
				//fmtlog("done! comment: {}", str);
				//find all emojis and store their position
				std::vector<size_t> positions;
				for(int j = 0; j < 15; j++)
				{
					const char* emoji = getEmoji(j, false);
					size_t pos = str.find(emoji, 0);
					while(pos != string::npos)
					{
						fmtlog("found at: {}", pos);
						positions.push_back(pos);
						pos = str.find(emoji,pos+2);
					}
				}
				if(positions.empty()) continue;
				
				std::sort(positions.begin(), positions.end());
				for(int j = 0; j < positions.size(); j++)
				{
					auto epos = positions[j];
					if(epos != 0)
						epos -= 2 * j;
				//	fmtlog("emojipos: {}", epos);
					auto letter = (CCSprite*)label->getChildren()->objectAtIndex(epos);
					letter->CCSprite::setColor({0, 0, 0}); //nothing changes if i dont do this lmao
					letter->CCSprite::setColor({255, 255, 255});
				}
				
				break;
			}
		}
	}
	
	return ret;
}
/*
FUNCTIONHOOK(float, getDeviceFrameRate, void) {
	return 999999.0f;
}
*/
void Emojis::ApplyHooks() 
{
	//HOOK2("_ZN15PlatformToolbox20getDeviceRefreshRateEv", getDeviceFrameRate);
	HOOK2("_ZN11CommentCell15loadFromCommentEP9GJComment", GJCommentCell_loadFromComment);
	HOOK2("_ZN17ShareCommentLayer7onShareEPN7cocos2d8CCObjectE", ShareCommentLayer_onShare);
	HOOK2("_ZN17ShareCommentLayer14updateDescTextEPKc", ShareCommentLayer_updateDescLabel);
	HOOK("_ZN17ShareCommentLayer4initESsi11CommentTypei", ShareCommentLayer_initH, ShareCommentLayer_initO);
}