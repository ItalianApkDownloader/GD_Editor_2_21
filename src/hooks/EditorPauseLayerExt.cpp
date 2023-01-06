#include <gd.h>
#include "cocos2d.h"
#include "EditorPauseLayerExt.h"
#include "hooking.h"
#include "GDPSHelper.h"


static inline void (*onResumeO)(EditorPauseLayer* self,CCObject* a1);
void EditorPauseLayerExt::onResumeH(CCObject* a1){
	auto gm = GameManager::sharedState();

	auto editor = MEMBERBYOFFSET(LevelEditorLayer*, this, 0x1C8);

	MBO(bool, editor, 0x142) = gm->getGameVariable("0009");
	MBO(bool, editor, 0x2BC0) = gm->getGameVariable("0001");
	MBO(bool, editor, 0x2BC1) = gm->getGameVariable("0044");
	MBO(bool, editor, 0x2BC2) = gm->getGameVariable("0045");
	MBO(bool, editor, 0x2BC3) = gm->getGameVariable("0038");
	MBO(bool, editor, 0x2BC5) = gm->getGameVariable("0043");
	MBO(bool, editor, 0x2BC6) = gm->getGameVariable("0037");
	MBO(bool, editor, 0x2BC7) = gm->getGameVariable("0058");
	MBO(bool, editor, 0x2BC8) = gm->getGameVariable("0013");
	MBO(bool, editor, 0x2C91) = gm->getGameVariable("0036");
	MBO(bool, editor, 0x2BC9) = gm->getGameVariable("0078");
	MBO(bool, editor, 0x2BCA) = gm->getGameVariable("0120");
	MBO(bool, editor, 0x2BC4) = gm->getGameVariable("0079");
	MBO(bool, editor, 0x2BCB) = gm->getGameVariable("0103");
	MBO(bool, editor, 0x2BCC) = gm->getGameVariable("0104");

	if(editor->_editorUI()) {
		editor->_editorUI()->updateGroupIDLabel();
		editor->_editorUI()->toggleStickyControls(GM->getGameVariable("0097"));
	}

	if(editor->_background()) editor->_background()->setVisible(!MEMBERBYOFFSET(bool, editor, 0x2BC9));
	if(editor->_middleground()) editor->_middleground()->setVisible(MEMBERBYOFFSET(bool, editor, 0x2BCA));

	if(editor->_gridLayer()) {
		auto parent = editor->_gridLayer()->getParent();


		int zOrder = -9999;

		if(GM->getGameVariable("0039")) zOrder = 9999;

		parent->reorderChild(editor->_gridLayer(), zOrder);
	}
	
	removeFromParentAndCleanup(true);
}


	
static inline void (*customSetupO)(EditorPauseLayer* self);
void EditorPauseLayerExt::customSetupH() {
		
	customSetupO(this);
		
	auto menu = CCMenu::create();
	menu->setPosition(0, 0);
		
	GDPSHelper::createToggleButton(
	"Disable Shaders",
	{CCMIDX - 200, CCBOTTOM + 24},
	.55, 8,
	this,
	menu_selector(EditorPauseLayerExt::onDisableShaders),
	menu,
	GM->getGameVariable("69234"),
	true);
	
	this->addChild(menu);
	
	//GDPSHelper::createLabels(this);
	
}

static inline bool (*initO)(EditorPauseLayer* self, void* editor);
bool EditorPauseLayerExt::initH(void* editor) {
	
	if(!initO(this, editor)) return false;
	
	for(int i = 15; i < 18; i++) {
		auto label = (CCLabelBMFont*)this->getChildren()->objectAtIndex(i);
		label->setPositionX(label->getPositionX() + 125);
	}
	auto menu = (CCMenu*)this->getChildren()->objectAtIndex(1);
	
	CCArray* arr = CCArray::create();
	arr->addObject(menu->getChildren()->objectAtIndex(0));
	arr->addObject(menu->getChildren()->objectAtIndex(1));
	//arr->addObject(menu->getChildren()->objectAtIndex());
	
	

	auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	sprite->setScale(0.75);
	auto optionsBtn = CCMenuItemSpriteExtra::create(sprite, nullptr, this, menu_selector(MenuLayer::onOptions));
	menu->addChild(optionsBtn);
	arr->addObject(optionsBtn);
	GameToolbox::alignItemsHorisontally(arr, 30, {0, 0}, false);


	return true;
}
	
void EditorPauseLayerExt::onDisableShaders(CCObject* sender) {
	
	GM->toggleGameVariable("69234");
}
	
	
	
void EditorPauseLayerExt::ApplyHooks() {


	HOOK_STATIC("_ZN16EditorPauseLayer8onResumeEPN7cocos2d8CCObjectE", 
	EditorPauseLayerExt::onResumeH, EditorPauseLayerExt::onResumeO);
	
	HOOK_STATIC("_ZN16EditorPauseLayer11customSetupEv", 
	EditorPauseLayerExt::customSetupH, EditorPauseLayerExt::customSetupO);
	
	HOOK_STATIC("_ZN16EditorPauseLayer4initEP16LevelEditorLayer", 
	EditorPauseLayerExt::initH, EditorPauseLayerExt::initO);
	

}