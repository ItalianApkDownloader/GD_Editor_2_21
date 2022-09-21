#include <dlfcn.h>
#include "blaze.h"
#include "cocos2d.h"
#include "EditLevelLayerExt.h"
#include "patch.h"
#include "hooking.h"
#include "MapSelectLayer.h"


void EditLevelLayerExt::onClick(CCObject* sender){
	
	if(!this->startedEditor) {
		
		
		#ifdef DEVDEBUG
			goto skipDisclaimer;
		#endif
		
		
		if(!GM->getGameVariable("100099")) {
			FLAlertLayer::create(nullptr, "DISCLAIMER", "This version of the editor is\n<cr>unstable and contains bugs and crashes that can't be fixed</c>.\nYour game will crash or freeze, <cg>save your game and levels often</c>.\n<co>If you don't want an unstable editor wait for official 2.2 and don't complain about it</c>.", "OK", nullptr, 500, false, 300)->show();
			GM->setGameVariable("100099", true);
		}
		else {
		
			skipDisclaimer:
			this->closeTextInputs();
			*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(GM) + 0x1BC) = 3;
			this->verifyLevelName();
			
			
			auto dir = cocos2d::CCDirector::sharedDirector( );
			auto editorLayer = LevelEditorLayer::create( this->_gameLevel(), true);
			//auto mapLayer = MapSelectLayer::scene();
			auto sc = CCScene::create();
			sc->addChild(editorLayer);
			
			auto scene = cocos2d::CCTransitionFade::create(0.5, sc);
			
			dir->replaceScene( scene );
			this->startedEditor = true;
		}
		
	}
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
	
	auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	sprite->setScale(.7);
	auto optionsBtn = CCMenuItemSpriteExtra::create(sprite, nullptr, this, menu_selector(MenuLayer::onOptions));
	auto bottomMenu2 = CCMenu::createWithItem(optionsBtn);
	reinterpret_cast<CCSprite *>(bottomMenu2)->setPosition({CCLEFT + 80, CCTOP - 25});
	this->addChild(bottomMenu2);
	
	auto menu3 = CCMenu::create();
	menu3->setPosition(CCRIGHT - 90, CCTOP - 30);
	auto btn3 = CCSprite::createWithSpriteFrameName("garageRope_001.png");
	auto myButton3 = CCMenuItemSpriteExtra::create(
		btn3,
		btn3,
		this,
	menu_selector(LevelInfoLayer::onGarage));

	myButton3->useAnimationType(MenuAnimationType::two);
	myButton3->setSizeMult(1.2);
	menu3->addChild(myButton3);
	this->addChild(menu3);
	
	
	this->startedEditor = false;


	return ret;
	
}


void EditLevelLayerExt::ApplyHooks() {
	
	HOOK_STATIC("_ZN14EditLevelLayer4initEP11GJGameLevel", 
	EditLevelLayerExt::initH, EditLevelLayerExt::initO);
	
}		