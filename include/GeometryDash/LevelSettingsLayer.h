#pragma once

#include "FLAlertLayer.h"
#include "LevelEditorLayer.h"
#include "LevelSettingsObject.h"

class LevelSettingsLayer : public FLAlertLayer 
{
public:
    LevelSettingsLayer();
    virtual ~LevelSettingsLayer();

    static LevelSettingsLayer* create( LevelSettingsObject* a1, LevelEditorLayer* a2 );
	
	void onSelectMode(CCObject*);
	void onMiniMode(CCObject*);
	
	void onMini(CCObject* sender) {
	CCLog("onmini!!!");
	
	auto settings = MBO(LevelSettingsObject*, this, 0x230);
	auto mini = MBO(bool, settings, 0x110);
	MBO(bool, settings, 0x110) = !mini;
	
}
	virtual bool init(void);
};