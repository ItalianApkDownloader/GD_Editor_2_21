#pragma once

#include "GJSearchObject.h"

class LevelBrowserLayer : public CCLayer
{
public:

	void onRefresh(CCObject*);
	static CCScene* scene(GJSearchObject*);
};