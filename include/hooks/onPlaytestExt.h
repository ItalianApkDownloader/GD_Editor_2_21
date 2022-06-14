#pragma once
#include "LevelEditorLayer.h"
class  onPlaytestExt : public LevelEditorLayer {

public: 

FLAlertLayer* fl;
public:

    static inline void (*playtest)(LevelEditorLayer*, int a2, int a3, int a4);
    void playtest_hk(int a2, int a3, int a4); 
	
    void onEnablePlaytest() { 
	GM->setGameVariable("100001", true); 
	fl->keyBackClicked();	
	}

};