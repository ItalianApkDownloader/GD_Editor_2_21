#include <gd.h>
#include "cocos2d.h"
#include "EditorPauseLayerExt.h"
#include "hooking.h"
/*

static inline void (*UnlockAllLayersO)(EditorPauseLayer* self, CCObject* a2);
void EditorPauseLayerExt::UnlockAllLayersH(CCObject* a2) {

	auto editor = this->_editorLayer();
	auto ui = editor->editorUI_;

	for(int i = 0; i < 1000; i++) {
		CCLog("class member : %d", editor->_currentLayer());
		
		editor->_currentLayer()++;

		if(editor->isLayerLocked(editor->_currentLayer()))
		editor->toggleLockActiveLayer();
	}
	
	CCLog("finish loop");
	editor->_currentLayer() = 0xFFFFFFFF;
	CCLog("reseting");
	
}

static inline void (*onResumeO)(EditorPauseLayer* self,CCObject* a1);
void EditorPauseLayerExt::onResumeH(CCObject* a1){
	
	auto p = this;
	auto gm = GameManager::sharedState();

	auto editor = MEMBERBYOFFSET(LevelEditorLayer*, p, 0x1C4);

	MEMBERBYOFFSET(bool, editor, 0x13E) = gm->getGameVariable("0009");
	MEMBERBYOFFSET(bool, editor, 0x2B8C) = gm->getGameVariable("0001");
	//MEMBERBYOFFSET(bool, editor, 0x2B8D) = gm->getGameVariable("0044"); // orbs and shit
	MEMBERBYOFFSET(bool, editor, 0x2B8E) = gm->getGameVariable("0045");
	MEMBERBYOFFSET(bool, editor, 0x2B8F) = gm->getGameVariable("0038");
	MEMBERBYOFFSET(bool, editor, 0x2B91) = gm->getGameVariable("0043");
	MEMBERBYOFFSET(bool, editor, 0x2B92) = gm->getGameVariable("0037");
	MEMBERBYOFFSET(bool, editor, 0x2B93) = gm->getGameVariable("0058");
	MEMBERBYOFFSET(bool, editor, 0x2B94) = gm->getGameVariable("0013");
	MEMBERBYOFFSET(bool, editor, 0x2C5D) = gm->getGameVariable("0036");
	MEMBERBYOFFSET(bool, editor, 0x2B95) = gm->getGameVariable("0078");
	MEMBERBYOFFSET(bool, editor, 0x2B96) = gm->getGameVariable("0120");
	MEMBERBYOFFSET(bool, editor, 0x2B90) = gm->getGameVariable("0079");
	MEMBERBYOFFSET(bool, editor, 0x2B97) = true; //layer locking
	MEMBERBYOFFSET(bool, editor, 0x2B98) = gm->getGameVariable("0104");

	if(editor->editorUI_) editor->editorUI_->updateGroupIDLabel();

	/**((bool *)p + 770) = gm->getGameVariable("0009");
	*((bool *)p + 11600) = gm->getGameVariable("0001");
	//*((bool *)p + 11601) = gm->getGameVariable( "0044");
	*((bool *)p + 11602) = gm->getGameVariable("0045");
	*((bool *)p + 11603) = gm->getGameVariable("0038");
	*((bool *)p + 11605) = gm->getGameVariable("0043");
	*((bool *)p + 11606) = gm->getGameVariable("0037");
	*((bool *)p + 11607) = gm->getGameVariable("0058");
	*((bool *)p + 11608) = gm->getGameVariable("0013");
	*((bool *)p + 11809) = gm->getGameVariable("0036");
	*((bool *)p + 11609) = gm->getGameVariable("0078");
	*((bool *)p + 11610) = gm->getGameVariable("0120");
	*((bool *)p + 11604) = gm->getGameVariable("0079");
	*((bool *)p + 11611) = gm->getGameVariable("0103");
	*((bool *)p + 11612) = gm->getGameVariable("0104");

	p->removeFromParentAndCleanup(true);
}



	*/

void EditorPauseLayerExt::ApplyHooks() {
	/*
	HOOK_STATIC("_ZN16EditorPauseLayer17onUnlockAllLayersEPN7cocos2d8CCObjectE", 
	EditorPauseLayerExt::UnlockAllLayersH, EditorPauseLayerExt::UnlockAllLayersO);
	
	HOOK_STATIC("_ZN16EditorPauseLayer8onResumeEPN7cocos2d8CCObjectE", 
	EditorPauseLayerExt::onResumeH, EditorPauseLayerExt::onResumeO);*/

}