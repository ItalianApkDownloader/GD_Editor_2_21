#include <bits/stdc++.h>
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



const char* charForInt(int n) {
	switch(n) {
		case 0: return "👍"; break;
		case 1: return "💀"; break;
		case 2: return "😀"; break;
		
		default: return "💀"; break;
	}
}
void ShareCommentLayer::onEmoji(CCObject* sender) {
	//getString() crashes if input is empty
	CCLog("hello");
	auto input = MBO(CCTextInputNode*, this, 0x204);
	auto pastStr = input->getString();

	input->CCTextInputNode::setString(fmt::format("{}{}", pastStr, charForInt(sender->CCObject::getTag())));
}

void ShareCommentLayer::onEmojiPicker(CCObject*) {
	
	auto layer = CCLayer::create();

	
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
	
	CCPoint startPos {-30, 50};
	for(int i = 0 ; i < 15; i++) {
		
		if(i % 5 == 0) {
			startPos.y -= 30;
			startPos.x = -68;
		}
		else
			startPos.x += 30;
		
		auto ebg = CCSprite::create("square02b_001.png");
		ebg->setPosition(startPos);
		ebg->setScale(.3);
		ebg->CCSprite::setOpacity(100);
		ebg->CCSprite::setColor({0, 0, 0});
		ebg->setContentSize(CCSize(20, 20));
		menu->addChild(ebg);
		
		auto charSprite = CCLabelBMFont::create(charForInt(i),  "chatFont.fnt");
		auto btn = CCMenuItemSpriteExtra::create(charSprite, nullptr, this, menu_selector(ShareCommentLayer::onEmoji));
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
	
	auto copyBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("bgIcon_01_001.png"), nullptr, self, menu_selector(ShareCommentLayer::onEmojiPicker));
	copyBtn->setPosition({-50, -20});
	menu->addChild(copyBtn);
	//GDPSHelper::createLabels(layer);
	return true;
}


void Emojis::ApplyHooks() 
{
	HOOK("_ZN17ShareCommentLayer4initESsi11CommentTypei", ShareCommentLayer_initH, ShareCommentLayer_initO);
}