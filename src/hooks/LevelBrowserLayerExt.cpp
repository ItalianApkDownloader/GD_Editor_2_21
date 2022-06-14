#include <gd.h>
#include "LevelBrowserLayerExt.h"
#include "patch.h"
#include <gd.h>
#include "cocos2d.h"
#include "LevelBrowserLayer.h"
#include "GJSearchObject.h"
//#include "../../include/hooks/MenuLayerExt.h"

bool LevelBrowserLayerExt::init_hk(GJSearchObject *a2)
{

	extern bool legendaryChanged;
	if (legendaryChanged)
	{
		this->runAction(CCCallFuncO::create(this, callfuncO_selector(LevelBrowserLayer::onRefresh), this));
		legendaryChanged = false;
	}

	return init_trp(this, a2);
}
