#pragma once

#include "LevelEditorLayer.h"

class LevelEditorLayer;

class DrawGridLayer : public cocos2d::CCLayer
{
public:
    DrawGridLayer();
    virtual ~DrawGridLayer();

    static DrawGridLayer* create( CCNode* a1, LevelEditorLayer* a2 );

    void updateTimeMarkers( );
	CCPoint posForTime(float);
	int timeForPos(cocos2d::CCPoint, int, int, bool, bool, bool);
	void updateMusicGuideTime(float);
};
