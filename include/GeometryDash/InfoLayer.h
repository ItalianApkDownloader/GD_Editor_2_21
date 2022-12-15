#pragma once

#include "CCLayer.h"
#include "LevelFeatureLayer.h"
#include "GeometryDash/CCMenuItemSpriteExtra.h"
#include "GJGameLevel.h"
#include "LoadingLayer.h"
#include "FLAlertLayer.h"
#include "GJUserScore.h"

class InfoLayer : public FLAlertLayer
{
public:

	PAD(472)

public:

	static InfoLayer* create(GJGameLevel*, GJUserScore*);
	void show();
	void loadPage(int, bool);
	
};
