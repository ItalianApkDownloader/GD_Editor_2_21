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
*/
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

	if(editor->_editorUI()) editor->_editorUI()->updateGroupIDLabel();

	removeFromParentAndCleanup(true);
}

void EditorPauseLayerExt::ApplyHooks() {
	/*
	HOOK_STATIC("_ZN16EditorPauseLayer17onUnlockAllLayersEPN7cocos2d8CCObjectE", 
	EditorPauseLayerExt::UnlockAllLayersH, EditorPauseLayerExt::UnlockAllLayersO);
	*/
	HOOK_STATIC("_ZN16EditorPauseLayer8onResumeEPN7cocos2d8CCObjectE", 
	EditorPauseLayerExt::onResumeH, EditorPauseLayerExt::onResumeO);

}