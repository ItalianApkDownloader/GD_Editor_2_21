#include "MultiplayerLayerExt.h"

std::string getPassword()
{
	return "";
}

#include <thread>
#include <unistd.h>

static inline bool (*init_trp)(MultiplayerLayer *self);
bool MultiplayerLayerExt::init_hk()
{
	fmtlog("mp init");

	GJAccountManager* am = GJAccountManager::sharedState();
	NetworkingManager::peer = static_cast<unsigned long>(MBO(int, am, 0x10C));
	
	std::thread t([] {
		NetworkingManager::Connect(2123387681, getPassword);
		while(true) {
			NetworkingManager::Update();
		}
	});
	t.detach();
	
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