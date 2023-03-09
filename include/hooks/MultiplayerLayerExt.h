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
	CCTextInputNode* _create_input_name;
	CCTextInputNode* _create_input_pass;

	CCTextInputNode* _join_input_id;
	CCTextInputNode* _join_input_pass;
	
public:
	void onUpdate(float dt);
	static inline bool (*init_trp)(MultiplayerLayer * self);
	bool init_hk();
	static void ApplyHooks();
	
	void onCreate(CCObject*);
	void onJoin(CCObject*);
};