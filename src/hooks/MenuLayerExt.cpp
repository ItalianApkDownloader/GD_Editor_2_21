#include <ColoursPalette.h>
#include <sstream>
#include "../cocos2dx/extensions/network/HttpClient.h"
#include "../cocos2dx/extensions/network/HttpRequest.h"
#include "../cocos2dx/extensions/network/HttpResponse.h"
#include "cocos2d.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "patch.h"
#include "KittyMemory.h"
#include "../Encryption/Encryption.h"
#include "../ZLIB/zlib.h"
#include "../GDPSManager.h"
#include "../obfuscate.h"
#include "../layers/advancedOptionsLayer.h"
#include "../layers/CreditsLayer.h"
//#include "../layers/ToolsLayer.h"
#include "../../include/hooks/MenuLayerExt.h"
#include "hooking.h"
#include "FunctionHelper.h"

template <class T>
extern void *getPointer(T value);

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

	std::vector<char> *buffer = response->getResponseData();
	GameToolbox *gameToolbox = new GameToolbox();
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
	CCLog("aaa");
	string v = "";
	for (int i = 0; i < version.length(); i++)
	{
		v = v + version[i];
		if (i < version.length() - 1)
			v = v + ".";
	}
	CCLog("bbb");

	string description = "Version: " + v + "\nRelease Date: " + date + "\nSize: " + weight + " MB" + "\n\n\Changelog:\n" + changelog + "\n\nWould you like to download the new update?";
	CCLog("ccc");

	alert = FLAlertLayer::create(nullptr, "New update!", description.c_str(), "NO", nullptr, 400, true, 300);
	CCLog("ddd");

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

void MenuLayerExt::onJoinDiscord(CCObject *sender)
{

auto app = cocos2d::CCApplication::sharedApplication();
	auto url = ("http://game.gdpseditor.com/server/game/discord.php");

	app->openURL(url);
}

void MenuLayerExt::onProgressChanged(CCObject *sender)
{
	auto value = reinterpret_cast<SliderThumb *>(sender)->getValue();
	CCLog("%f", value);
}

void MenuLayerExt::onBlaze(CCObject *sender)
{

	auto app = cocos2d::CCApplication::sharedApplication();
	auto url = ("https://www.youtube.com/channel/UCcfPtuop90e_JzxPkiZ6Q5Q");

	app->openURL(url);
}

static inline void (*onJoinDiscordO)(MenuLayer* self, CCObject* sender);
void MenuLayerExt::onJoinDiscordH(CCObject* sender){
	cocos2d::CCApplication::sharedApplication()->openURL("https://discord.gdpseditor.com/");
}

static inline void (*updateUserProfileButtonO)(MenuLayer*);
void MenuLayerExt::updateUserProfileButtonH() {
	


 //   this->_playerUsernameLabel()->setVisible(true);
	auto profileBtn = MBO(CCMenuItemSpriteExtra*, this, 0x150);
    profileBtn->setVisible(true);



}


#include <fstream>

void MenuLayerExt::onStackTrace(CCObject* s) {
	
        auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + "crash.txt";

    std::ifstream ifs( path );
    std::string content = "File not found.";

    std::stringstream ss;

    if ( ifs.good() )
    {
        std::string sLine;

        int i = 0;
        while ( i < 19 )
        {
            getline(ifs, sLine);
            ss << sLine << std::endl;
            i++;
        }
    }
    else 
        ss << "File not found.";

    ifs.close( );

    // saber::logging::log( "%s ALL DATA", content.c_str() );
    FLAlertLayer::create( nullptr, "Stack Trace", ss.str(), "Exit", nullptr, 450., true, 300. )->show( );

}


static inline bool (*init_trp)(MenuLayer *self);
bool MenuLayerExt::init_hk()
{

	auto ret = init_trp(this);
	
	#ifdef DEVDEBUG
	FMOD->setBackgroundMusicVolume(0);

	#endif

 /*
 
	auto menu = CCMenu::create();
	menu->setPositionY(menu->getPositionY() + 100);
	auto btn3 = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	auto myButton3 = CCMenuItemSpriteExtra::create(
		btn3,
		btn3,
		this,
	//menu_selector(MenuLayerExt::onStackTrace));
	menu_selector(VideoOptionsLayer::onAdvanced));
	menu->addChild(myButton3);
	this->addChild(menu);
	
 */
		extern bool doRequest;
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

	
	
				

	return ret;
};


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
	
}