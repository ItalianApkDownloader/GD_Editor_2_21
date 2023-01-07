#include <sstream>
#include "cocos2d.h"
#include "../cocos2dx/extensions/network/HttpClient.h"
#include "../cocos2dx/extensions/network/HttpRequest.h"
#include "../cocos2dx/extensions/network/HttpResponse.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "patch.h"
#include "../GDPSManager.h"
#include "../obfuscate.h"

#include "../Geometrydash/InfoLayer.h"
#include "../../include/hooks/MenuLayerExt.h"
#include "hooking.h"
#include "../FunctionHelper.h"
#include "../GDPSHelper.h"
#include "SearchButton.h"



void MenuLayerExt::onRequestCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response)
{
	CCLog("Completed!");

	if (!response)
	{
		CCLog("onHttpRequestCompleted - No Response");
		return;
	}

	CCLog("onHttpRequestCompleted - Response code: %lu", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLog("onHttpRequestCompleted - Response failed");
		CCLog("onHttpRequestCompleted - Error buffer: %s", response->getErrorBuffer());
		return;
	}

	//std::vector<char> *buffer = response->getResponseData();
	//GameToolbox *gameToolbox = new GameToolbox();
	auto resp = GameToolbox::getResponse(response);
	auto gm = GameManager::sharedState();

	CCLog("%s", resp.c_str());

	rapidjson::Document parsedFromString;

	parsedFromString.Parse<0>(resp.c_str());

	CCLog("%d", parsedFromString["version"].GetInt());

	int ver = parsedFromString["version"].GetInt();
	int particles = parsedFromString["particles"].GetInt();
	string weight = parsedFromString["weight"].GetString();
	string date = parsedFromString["date"].GetString();
	string changelog = parsedFromString["changelog"].GetString();

	gm->setIntGameVariable("11001", particles);

	if (ver > version2)
	{
		// FLAlertLayer::create(nullptr, "New update!", "New update!\n<cg>Download</c> the new update in the discord server\n Join using the <co>join discord!</c> button",  "OK", nullptr, 400, false, 300)->show();
		MenuLayerExt::showUpdateAlert(FunctionHelper::itos(ver), weight, date, changelog);
	}

	// I dont think I will use this but it can be funny to troll or something XDD

	switch (particles)
	{

	case 1:
		this->addChild(CCParticleFire::create());
		break;
	case 2:
		this->addChild(CCParticleFireworks::create());
		break;
	case 3:
		this->addChild(CCParticleSun::create());
		break;
	case 4:
		this->addChild(CCParticleGalaxy::create());
		break;
	case 5:
		this->addChild(CCParticleFlower::create());
		break;
	case 6:
		this->addChild(CCParticleMeteor::create());
	case 7:
		this->addChild(CCParticleSpiral::create());
		break;
	case 8:
		this->addChild(CCParticleExplosion::create());
		break;
	case 9:
		this->addChild(CCParticleSmoke::create());
		break;
	case 10:
		CCLog("should have added snow?!");
		this->addChild(CCParticleSnow::create());
		break;
	case 11:
		this->addChild(CCParticleRain::create());
	case 12:
		this->addChild(CCParticleSnow::create());
		this->addChild(CCParticleSmoke::create());
	}

	GM->setGameVariable("11000", false);
}

void MenuLayerExt::showUpdateAlert(string version, string weight, string date, string changelog)
{
	string v = "";
	for (int i = 0; i < version.length(); i++)
	{
		v = v + version[i];
		if (i < version.length() - 1)
			v = v + ".";
	}

	string description = "Version: " + v + "\nRelease Date: " + date + "\nSize: " + weight + " MB" + "\n\n\Changelog:\n" + changelog + "\n\nWould you like to download the new update?";

	alert = FLAlertLayer::create(nullptr, "New update!", description.c_str(), "NO", nullptr, 400, true, 300);

	auto menu = alert->_btnMenu();
	auto okBtn = reinterpret_cast<CCNode *>(menu->getChildren()->objectAtIndex(0));

	auto sprite = ButtonSprite::create("Yes", 50, 10, 10, 5);
	auto btn = CCMenuItemSpriteExtra::create(sprite, sprite, this, menu_selector(MenuLayerExt::onDownload));
	menu->addChild(btn);

	GameToolbox::alignItemsHorisontally(menu->getChildren(), 15, okBtn->getPosition(), false);

	alert->show();
}

void MenuLayerExt::onDownload(CCObject *sender)
{

	auto app = cocos2d::CCApplication::sharedApplication();
	auto url = ("http://game.gdpseditor.com/server/game/download.php");

	app->openURL(url);
}



void MenuLayerExt::onTools(CCObject* sender) {
	cocos2d::CCApplication::sharedApplication()->openURL("http://gdpseditor.com/tools");
}
void MenuLayerExt::onBlaze(CCObject *sender)
{
	cocos2d::CCApplication::sharedApplication()->openURL("https://www.youtube.com/channel/UCcfPtuop90e_JzxPkiZ6Q5Q");
}

static inline void (*onJoinDiscordO)(MenuLayer* self, CCObject* sender);
void MenuLayerExt::onJoinDiscordH(CCObject* sender){
	cocos2d::CCApplication::sharedApplication()->openURL("https://discord.gdpseditor.com/");
}

static inline void (*updateUserProfileButtonO)(MenuLayer*);
void MenuLayerExt::updateUserProfileButtonH() {
	auto profileBtn = MBO(CCMenuItemSpriteExtra*, this, 0x150);
	profileBtn->setVisible(true);
}


#include <fstream>

void MenuLayerExt::onStackTrace(CCObject* s) {
	
	auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + "crash.txt";
	std::ifstream ifs( path );
	std::stringstream ss;
	if (ifs.good())
	{
		system("mkdir sdcard/GDPSEditor");
		system("cp -F crash.txt GDPSEditor");
	}
}


static inline bool (*init_trp)(MenuLayer *self);
bool MenuLayerExt::init_hk()
{
	CCLog("Menu Init!");
	if(!init_trp(this)) return false;
	
	//GDPSHelper::createLabels(this);
	
	int j = 0;
	
	for(int i = 0; i < this->getChildrenCount(); i++)
	{
		if(auto btnmenu = dynamic_cast<CCMenu*>(this->getChildren()->objectAtIndex(i)))
		{
			if(j == 1)
			{
				auto spr = CCSprite::createWithSpriteFrameName(GDPS->showNewNewsIndicator ? "GJ_newBtn_001.png" : "GJ_changeSongBtn_001.png");
				spr->setScale(GDPS->showNewNewsIndicator ? 1 : 1.35);
				auto freebtn = (CCNode*)btnmenu->getChildren()->lastObject();
				btnmenu->removeChild(freebtn, false);
				auto newsbtn = CCMenuItemSpriteExtra::create(spr, spr, this, menu_selector(MenuLayerExt::onNews));
				
				if(GDPS->showNewNewsIndicator)
				{
					GDPS->showNewNewsIndicator = false;
					auto v136 = cocos2d::CCScaleTo::create(0.5, 1.2);
					auto v157 = cocos2d::CCEaseInOut::create(v136, 2);
					auto v139 = cocos2d::CCScaleTo::create(0.5, 1.1);
					auto v141 = cocos2d::CCEaseInOut::create(v139, 2);
					auto v142 = cocos2d::CCSequence::create(v157, v141, nullptr);
					auto v144 = cocos2d::CCRepeatForever::create(v142);
					newsbtn->runAction(v144);
				}
				
				btnmenu->addChild(newsbtn, 2);
				btnmenu->getChildren()->exchangeObjectAtIndex(4, 3);
				btnmenu->getChildren()->exchangeObjectAtIndex(3, 2);
				
				btnmenu->alignItemsHorizontallyWithPadding(5);
				btnmenu->addChild(freebtn);
			}
			else if(j == 2)
			{
				auto robBtn = (CCNode*)btnmenu->getChildren()->objectAtIndex(0);
				auto spr = SearchButton::create("GJ_longBtn04_001.png", "Tools", 0.7, nullptr);
				spr->setScale(.7);
				//auto btn = MenuItemSpriteExtra::createWithNode(spr, [](CCNode* sender) { cocos2d::CCApplication::sharedApplication()->openURL("http://gdpseditor.com/tools");});
				auto btn = CCMenuItemSpriteExtra::create(spr, nullptr, this, menu_selector(MenuLayerExt::onTools));
				btn->setPosition(robBtn->getPosition());
				btnmenu->addChild(btn);
				robBtn->setPositionY(20000);
				break;
			}
			j++;
		}
	}

	extern bool first;
	if (first)
	{
		if (GM->getGameVariable("100003"))
			this->runAction(CCCallFuncO::create(this, callfuncO_selector(CreatorLayer::onMyLevels), this));
		first = false;
	}
	
	extern bool doRequest;
		
	
	#ifdef DEVDEBUG
	doRequest = false;
	FMOD->setBackgroundMusicVolume(0);
	#endif
	

	
	if(doRequest) {
	
		doRequest = false;
		CCLog("try request");
		
		 cocos2d::extension::CCHttpRequest* request = new (std::nothrow) cocos2d::extension::CCHttpRequest();
		request->setUrl("http://game.gdpseditor.com/server/game/version2.php");
		
		request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);

		request->setResponseCallback(this,callfuncND_selector(MenuLayerExt::onRequestCompleted));
		request->setTag("Post test2");
		cocos2d::extension::CCHttpClient::getInstance()->send(request);
		request->release();
	}

/* would work with fixed headers
    auto* sss = cocos2d::CCSprite::create("dialogIcon_017.png");
	auto bbb = MenuItemSpriteExtra::createWithNode(sss, [](CCNode* sender) {std::cout << "button clicked" << std::endl; });

	auto gg = CCMenu::create();
	gg->addChild(bbb);
	gg->setPosition(120, 130);
	this->addChild(gg);
*/	
	return true;
};


void LoadingLayer::onNewsRequestCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response)
{
	CCLog("Completed!");

	if (!response)
	{
		CCLog("onHttpRequestCompleted - No Response");
		return;
	}

	CCLog("onHttpRequestCompleted - Response code: %lu", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLog("onHttpRequestCompleted - Response failed");
		CCLog("onHttpRequestCompleted - Error buffer: %s", response->getErrorBuffer());
		return;
	}

	//std::vector<char> *buffer = response->getResponseData();
	//GameToolbox *gameToolbox = new GameToolbox();
	auto resp = GameToolbox::getResponse(response);
	CCLog("newscount: %d, response: %s", GDPS->newsCount, resp.c_str());
	
	int nCount = atoi(resp.c_str());
	GDPS->showNewNewsIndicator = nCount > GDPS->newsCount;
	GDPS->newsCount = nCount;
	
	
}

void LoadingLayer::makeNewsRequest() {
	
	cocos2d::extension::CCHttpRequest* request = new (std::nothrow) cocos2d::extension::CCHttpRequest();
	request->setUrl("http://game.gdpseditor.com/server/game/getNewsCount.php");
		
	request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);

	request->setResponseCallback(this,callfuncND_selector(LoadingLayer::onNewsRequestCompleted));
	request->setTag("Post test2");
	cocos2d::extension::CCHttpClient::getInstance()->send(request);
	request->release();
}
void MenuLayerExt::onNews(CCObject* sender) {
	
	
	
	auto level = GJGameLevel::create();
	level->setLevelID(455895);
	
	//config of comments, save user choice -> set custom -> restore user choice when exiting layer
	
	bool g1 = GM->ggv("0089");
	bool g2 = GM->ggv("0088");
	bool g3 = GM->ggv("0069");
	
	GDPS->g1 = g1;
	GDPS->g2 = g2;
	GDPS->g3 = g3;
	GDPS->isNews = true;

	GM->sgv("0089", true); //sort by time
	GM->sgv("0088", false); //make comments big 
	GM->sgv("0069", false); //hide description
	
	auto il = InfoLayer::create(level, nullptr);
	il->loadPage(0, false);
	
	auto layer = (CCLayer*)il->getChildren()->objectAtIndex(0);
	
	for(int i = 3; i <= 4; i++) {
		auto someLabel = (CCLabelBMFont*)layer->getChildren()->objectAtIndex(i);
		someLabel->setVisible(false);
	}
	
	auto m1 = (CCMenu*)layer->getChildren()->objectAtIndex(1);
	auto creatorbtn = (CCMenuItemSpriteExtra*)m1->getChildren()->objectAtIndex(0);
	auto creatorLabel = (CCLabelBMFont*)creatorbtn->getChildren()->objectAtIndex(0);
	creatorLabel->CCLabelBMFont::setString("Announcements");
	creatorLabel->CCLabelBMFont::setColor({255, 255, 255});
	creatorLabel->CCLabelBMFont::setScale(creatorLabel->getScale() * 1.5);
	
	auto btn = (CCMenuItemSpriteExtra*)m1->getChildren()->objectAtIndex(1);
	btn->setPositionY(99999);
	
	btn = (CCMenuItemSpriteExtra*)m1->getChildren()->objectAtIndex(2);
	btn->setPositionY(99999);

	btn = (CCMenuItemSpriteExtra*)m1->getChildren()->objectAtIndex(5);
	btn->setPositionY(99999);

	auto m2 = (CCMenu*)layer->getChildren()->objectAtIndex(4);
	auto m3 = (CCMenu*)layer->getChildren()->objectAtIndex(6);
	auto m4 = (CCMenu*)layer->getChildren()->objectAtIndex(8);
	m4->setPositionY(99999);
	
//	GDPSHelper::createLabels(m1);
	
	il->InfoLayer::show();
}

FUNCTIONHOOK(void, InfoLayer_onClose, InfoLayer* self, CCObject* sender) {
		
	
	if(GDPS->isNews) {
		GDPS->isNews = false;
		CCLog("close is news");
		GM->sgv("0089", GDPS->g1);
		GM->sgv("0088", GDPS->g2);
		GM->sgv("0069", GDPS->g3);
	}

	InfoLayer_onCloseO(self, sender);
}

FUNCTIONHOOK(InfoLayer*, InfoLayer_create, GJGameLevel* level, void* a) {
	
	bool g1 = GM->ggv("0089");
	bool g2 = GM->ggv("0088");
	bool g3 = GM->ggv("0069");
	
	CCLog("89: %d | 88: %d | 69: %d", g1, g2, g3);
	return InfoLayer_createO(level, a);
}

void MenuLayerExt::onUpdateShaderFile(CCObject* sender) {
	
		cocos2d::extension::CCHttpRequest* request = new (std::nothrow) cocos2d::extension::CCHttpRequest();
		request->setUrl("http://game.gdpseditor.com/server/game/shaders/uberShader.fsh");
			
		request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpPost);


		request->setResponseCallback(this,callfuncND_selector(MenuLayerExt::onShaderRequestCompleted));
		request->setTag("Post test2");
		cocos2d::extension::CCHttpClient::getInstance()->send(request);
		request->release();
		
		
		
				
}

void MenuLayerExt::onShaderRequestCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response) {
	
		CCLog("Completed!");

	if (!response)
	{
		CCLog("onHttpRequestCompleted - No Response");
		return;
	}

	CCLog("onHttpRequestCompleted - Response code: %lu", response->getResponseCode());

	if (!response->isSucceed())
	{
		CCLog("onHttpRequestCompleted - Response failed");
		CCLog("onHttpRequestCompleted - Error buffer: %s", response->getErrorBuffer());
		return;
	}

std::vector<char> *buffer = response->getResponseData();
//	GameToolbox *gameToolbox = new GameToolbox();
	auto resp = GameToolbox::getResponse(response);
	auto gm = GameManager::sharedState();

	CCLog("%s", resp.c_str());
	
	
	
	   auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + "uberShader.fsh";
		std::ofstream outfile;
		outfile.open(path);
		outfile << resp;
		outfile.close();
	
	
}


void MenuLayerExt::onTest(CCObject *sender)
{

	//LayerTest::create();
}


void MenuLayerExt::ApplyHooks() {
	
	
	HOOK_STATIC("_ZN9MenuLayer9onDiscordEPN7cocos2d8CCObjectE", 
	MenuLayerExt::onJoinDiscordH, MenuLayerExt::onJoinDiscordO);
	
	HOOK_STATIC("_ZN9MenuLayer23updateUserProfileButtonEv", 
	MenuLayerExt::updateUserProfileButtonH, MenuLayerExt::updateUserProfileButtonO);
	
	HOOK_STATIC("_ZN9MenuLayer7showTOSEv", 
	MenuLayerExt::MenuLayer_showTOSH, MenuLayerExt::MenuLayer_showTOSO);
	
	HOOK_STATIC("_ZN9MenuLayer4initEv",
	MenuLayerExt::init_hk, MenuLayerExt::init_trp);
	
	HOOK("_ZN9InfoLayer7onCloseEPN7cocos2d8CCObjectE", InfoLayer_onCloseH, InfoLayer_onCloseO);
	HOOK("_ZN9InfoLayer6createEP11GJGameLevelP11GJUserScore", InfoLayer_createH, InfoLayer_createO);
	
}