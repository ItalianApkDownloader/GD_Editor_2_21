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
};