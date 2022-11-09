#pragma once

#include <gd.h>
#include "GDAPI_Macros.h"

class PlayLayer : public GJBaseGameLayer // GJBaseGameLayer
{
public:
    unsigned char _pad2[0x284]; // 0x3e4
    GJGameLevel* level; // 0x668
	
public:

	    CLASS_MEMBER(bool, practiceMode, 0x675);
		CLASS_MEMBER(GJGameLevel*, level, 0x13C);
		CLASS_MEMBER(LevelSettingsObject*, lvlSettingsObject, 0x33C);
		//CLASS_MEMBER(UILayer*, UILayer, 0x2CA0); the include errors are scary
		
		

public:
    PlayLayer();
    virtual ~PlayLayer();

    static PlayLayer* create( GJGameLevel* level, bool a2, bool a3 );

    void toggleInfoLabel( );
    void stopRecording( );
    void removeAllObjects();
    void updateVisibility(float);
	void onQuit();

    void destroyPlayer(PlayerObject* player, GameObject* object);
};