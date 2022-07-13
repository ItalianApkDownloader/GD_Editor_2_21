#include <dlfcn.h>
#include "blaze.h"
#include "cocos2d.h"
#include "EditLevelLayerExt.h"
#include "patch.h"
#include "hooking.h"


void EditLevelLayerExt::onClick(CCObject* sender){
	/*
		if(!GM->getGameVariable("100099")) {
			
			FLAlertLayer::create(nullthis, "DISCLAIMER", "This version of the editor is\n<cr>unstable and contains bugs and crashes that can't be fixed</c>.\nYour game will crash or freeze, <cg>save your game and levels often</c>.\n<co>If you don't want an unstable editor wait for official 2.2 and don't complain about it</c>.", "OK", nullthis, 500, false, 300)->show();
			GM->setGameVariable("100099", true);
		}
		else {
			*/
	this->closeTextInputs();
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(GameManager::sharedState()) + 0x1BC) = 3;
	this->verifyLevelName();
	

	auto dir = cocos2d::CCDirector::sharedDirector( );
	auto layer = LevelEditorLayer::create( this->_gameLevel(), false);
	auto sc = CCScene::create();
	sc->addChild(layer);

	auto scene = cocos2d::CCTransitionFade::create(0.5, sc);

	dir->replaceScene( scene );
	//	}
	

}

static inline bool (*initO)(EditLevelLayer*, GJGameLevel* level);
bool EditLevelLayerExt::initH(GJGameLevel* level) {
	
	auto ret = initO(this, level);

	auto menu = MEMBERBYOFFSET(CCMenu*, this, 0x14C);

	auto editBtn = (CCMenuItemSpriteExtra *)menu->getChildren()->objectAtIndex(0);

	auto editBtnCustom = CCMenuItemSpriteExtra::create(
	CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png"), 
	nullptr, 
	this,
	menu_selector(EditLevelLayerExt::onClick));
	
	menu->addChild(editBtnCustom);
	editBtnCustom->setPosition(editBtn->getPosition());
	editBtn->removeFromParentAndCleanup(true);

	return ret;
	
}


void EditLevelLayerExt::ApplyHooks() {
	
	HOOK_STATIC("_ZN14EditLevelLayer4initEP11GJGameLevel", 
	EditLevelLayerExt::initH, EditLevelLayerExt::initO);
	
}