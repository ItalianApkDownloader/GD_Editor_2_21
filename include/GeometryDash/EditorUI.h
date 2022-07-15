#pragma once

#include "LevelEditorLayer.h"
#include "CreateMenuItem.h"

class LevelEditorLayer;

class EditorUI : public cocos2d::CCLayer
{
public:
    uint8_t _pad0[ 0x1b8 ];
    LevelEditorLayer* levelEditor_; // 0x2D4

public:

CLASS_MEMBER(LevelEditorLayer*, levelEditor, 0x2F8);
CLASS_MEMBER(CCMenu*, objectMenu, 0x298);

public:
    EditorUI();
    virtual ~EditorUI();

    static EditorUI* create( LevelEditorLayer* editorLayer );
    CreateMenuItem* getCreateBtn( int objectID, int a1 );

    void updateSlider( );

    void onPause(CCObject* sender);
    void onResume(CCObject* sender);
    void onStopPlaytest(CCObject* sender);
    void updateGroupIDLabel( );
    void toggleStickyControls( bool toggle );
    void updateZoom(float);
    CCArray* getSelectedObjects();
	int editorLayerForArray(CCArray*, bool);
	void onGroupUp(CCObject*);
	void onGroupDown(CCObject*);
	void onLockLayer(CCObject*);
	void onGoToBaseLayer(CCObject*);	
	void replaceGroupID(GameObject* go, int old, int newgroup);
};