#include "MultiplayerLayerExt.h"

std::string getPassword()
{
	return "";
}

static inline bool (*init_trp)(MultiplayerLayer *self);
bool MultiplayerLayerExt::init_hk()
{
    this->schedule(schedule_selector(MultiplayerLayerExt::onUpdate), 0.01);

	NetworkingManager::Connect(3760762687, getPassword);

    return true;
}

void MultiplayerLayerExt::onUpdate(float dt)
{
    NetworkingManager::Update();
}

void MultiplayerLayerExt::ApplyHooks() 
{
	HOOK_STATIC("_ZN16MultiplayerLayer4initEv",
	    MultiplayerLayerExt::init_hk, MultiplayerLayerExt::init_trp);
	
}