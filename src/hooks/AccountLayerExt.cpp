#include <gd.h>
#include "cocos2d.h"
#include "AccountLayerExt.h"
#include "patch.h"
#include "AccountLayer.h"
//#include "../../include/hooks/MenuLayerExt.h"

void AccountLayerExt::accload_hk()
{

	extern bool shouldSendDefaultValue;
	shouldSendDefaultValue = true;

	accload(this);
}
