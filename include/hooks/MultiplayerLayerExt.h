#pragma once

#include <sstream>
#include "cocos2d.h"
#include "hooking.h"
#include "../FunctionHelper.h"
#include "../GDPSHelper.h"
#include "networking/NetworkingManager.h"

class MultiplayerLayerExt : public MultiplayerLayer 
{
public:
	void onUpdate(float dt);

    static inline bool (*init_trp)(MultiplayerLayer * self);
    bool init_hk();

	static void ApplyHooks();
};