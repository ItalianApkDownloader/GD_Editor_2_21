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
/*
	GJAccountManager* am = GJAccountManager::sharedState();
	NetworkingManager::peer = static_cast<unsigned long>(MBO(int, am, 0x10C));
	
	std::thread t([] {
		NetworkingManager::Connect(2123387681, getPassword);
		while(true) {
			NetworkingManager::Update();
		}
	});
	t.detach();
	*/
	
	auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
	auto visSize = director->getVisibleSize();
	auto menu = CCMenu::create();
	menu->setPosition(0, 0);
	
	auto bg = CCSprite::create("GJ_gradientBG.png");
	bg->setPosition(CCPoint(0, winSize.height / 2));
	bg->setScaleX(winSize.width);
	bg->setScaleY(winSize.height / bg->getContentSize().height);
	bg->CCSprite::setColor(ccc3(0, 0x66, 0xFF));
	this->addChild(bg, -2);

	float ww = visSize.width;
	float hh = visSize.height;
	
	constexpr float input_scale = 0.6f;
	constexpr float max_width = 250.0f;
	
	float align_left = ww / 2 - 150; //why not aligned :skull:
	float align_right = ww / 2 + 120;
	
	//create - left
	_create_input_name = CCTextInputNode::create(100, 50.0f, "Lobby Name", "Thonburi", 5, "bigFont.fnt");	
	_create_input_name->setPosition(align_left, hh - 100);
	_create_input_name->setMaxLabelWidth(max_width);
	_create_input_name->setScale(input_scale);
	this->addChild(_create_input_name);
	
	_create_input_pass = CCTextInputNode::create(100, 50.0f, "Lobby Password", "Thonburi", 5, "bigFont.fnt");
	_create_input_pass->setString("1");
	_create_input_pass->setPosition(align_left, hh - 150);
	_create_input_pass->setScale(input_scale);
	_create_input_pass->setMaxLabelWidth(max_width);
	this->addChild(_create_input_pass);
	
	auto sprite = ButtonSprite::create("Create", 5);
	auto btn = CCMenuItemSpriteExtra::create(sprite, nullptr, this, menu_selector(MultiplayerLayerExt::onCreate));
	btn->setPosition(align_left + 15, hh - 180);
	menu->addChild(btn);
	
	//join - right
	_join_input_id = CCTextInputNode::create(100, 50.0f, "Lobby ID", "Thonburi", 5, "bigFont.fnt");	
	_join_input_id->setPosition({align_right, hh - 100});
	_join_input_id->setScale(input_scale);
	_join_input_id->setMaxLabelWidth(max_width);
	this->addChild(_join_input_id);
	
	_join_input_pass = CCTextInputNode::create(100, 50.0f, "Lobby Password", "Thonburi", 5, "bigFont.fnt");	
	_join_input_pass->setPosition(align_right, hh - 150);
	_join_input_pass->setScale(input_scale);
	_join_input_pass->setMaxLabelWidth(max_width);
	this->addChild(_join_input_pass);
	
	sprite = ButtonSprite::create("Join", 5);
	btn = CCMenuItemSpriteExtra::create(sprite, nullptr, this, menu_selector(MultiplayerLayerExt::onJoin));
	btn->setPosition(align_right + 15, hh - 180);
	menu->addChild(btn);
	
	this->addChild(menu);

	return true;
}

void MultiplayerLayerExt::onCreate(CCObject* sender)
{
	//WHY DOES IT CRASH IF EMPTY??????????????????????????
	//std::string lobbyName = _create_input_name->CCTextInputNode::getString();
	//std::string lobbyPass = _create_input_pass->CCTextInputNode::getString();
	std::string emptyStr = "";
	fmtlog("empty str: {}", emptyStr); //its not fmt

	//fmtlog("on create| name: {}, pass: {}", lobbyName, lobbyPass);
}

void MultiplayerLayerExt::onJoin(CCObject* sender)
{
	//WHY DOES IT CRASH IF EMPTY??????????????????????????
	//std::string lobbyID = _join_input_id->getString();
	//std::string lobbyPass = _join_input_pass->getString();
	
	//fmtlog("on join| name: {}, pass: {}", lobbyID, lobbyPass);
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