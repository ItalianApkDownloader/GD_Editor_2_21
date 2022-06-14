#ifndef __EDITORUI_H__
#define __EDITORUI_H__

#include "LevelEditorLayer.h"


class EditorUI : public cocos2d::CCLayer
{
public:
	uint8_t _pad0[ 0x1b8 ];
	LevelEditorLayer* levelEditor_; // 0x2D4

public:
	EditorUI();
	virtual ~EditorUI();

	static EditorUI* create( LevelEditorLayer* editorLayer );
	void* getCreateBtn( int objectID, int a1 );

	void updateSlider( );

	void updateGroupIDLabel( );
	void toggleStickyControls( bool toggle );
};

#endif