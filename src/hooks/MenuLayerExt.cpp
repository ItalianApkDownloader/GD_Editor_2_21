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
#include "../layers/ToolsLayer.h"
#include "../../include/hooks/MenuLayerExt.h"

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
	CCLog("onHttpRequestCompleted - Response code: %s", response->getResponseHeader());

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
		MenuLayerExt::showUpdateAlert(GDPS->itos(ver), weight, date, changelog);
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
	for (int i = 0; i < 4; i++)
	{
		v = v + version[i];
		if (i < 3)
			v = v + ".";
	}

	string description = "Version: " + v + "\nRelease Date: " + date + "\nSize: " + weight + " MB" + "\n\n\Changelog:\n" + changelog + "\n\nWould you like to download the new update?";

	alert = FLAlertLayer::create(nullptr, "New update!", description.c_str(), "NO", nullptr, 400, true, 300);

	auto menu = alert->m_pButtonMenu;
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

void onPopupTest(CCObject *sender)
{
	// crash :v
	advancedOptionsLayer::create()->show();
}

static inline bool (*init_trp)(MenuLayer *self);
bool MenuLayerExt::init_hk()
{

	auto ret = init_trp(this);

	extern bool addBadges;
	addBadges = true;

	auto director = CCDirector::sharedDirector();
	auto dir = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();
	auto gm = GameManager::sharedState();

	/*//auto gradient = CCLayerGradient::create(ccc4(255, 0, 0, 0), ccc4(255, 0, 0, 0));
	auto gradient = CCLayerGradient::create(ccc4(255, 255, 0, 255), ccc4(0, 0, 255, 255));
	gradient->setPosition(CCRIGHT / 2, CCTOP / 2);
	//gradient->initWithColor(ccc4(59, 255, 240, 1), ccc4(241, 0, 255, 1));
	this->addChild(gradient,10000000000);
	gradient->init();
	*/

	/*
			auto btn = ButtonSprite::create("Support Blaze!",80,10,10,1);
			auto myButton = CCMenuItemSpriteExtra::create(
					btn,
					btn,
					this,
					menu_selector(MenuLayerExt::onBlaze)
			);
			auto bottomMenu =  CCMenu::create();
			reinterpret_cast<CCSprite*>(bottomMenu)->setPositionY(90);
	*/

	auto menu = CCMenu::create();
	auto createMenu = CCMenu::create();

	if (GM->getGameVariable("100007"))
	{
		auto createSpr = CCSprite::createWithSpriteFrameName("GJ_createBtn_001.png");
		createSpr->setScale(.7);
		auto createBtn = CCMenuItemSpriteExtra::create(
			createSpr,
			createSpr,
			this,
			menu_selector(CreatorLayer::onMyLevels)
			// menu_selector(OptionsLayer::onVideo)
		);
		createMenu->addChild(createBtn);
		reinterpret_cast<CCSprite *>(createMenu)->setPosition({CCRIGHT - 50, CCBOTTOM + 45});
		this->addChild(createMenu);
	}
	auto btn = CCSprite::createWithSpriteFrameName("toolsBtn_001.png");
	btn->setScale(1.2);
	auto myButton = CCMenuItemSpriteExtra::create(
		btn,
		btn,
		this,
		menu_selector(ToolsLayer::create));
	menu->addChild(myButton);

	auto btn2 = CCSprite::createWithSpriteFrameName("GJ_achBtn_001.png");
	auto myButton2 = CCMenuItemSpriteExtra::create(
		btn2,
		btn2,
		this,
		menu_selector(MenuLayer::onAchievements));
	menu->addChild(myButton2);

	auto btn3 = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	auto myButton3 = CCMenuItemSpriteExtra::create(
		btn3,
		btn3,
		this,
		menu_selector(MenuLayer::onOptions));
	menu->addChild(myButton3);

	auto btn4 = CCSprite::createWithSpriteFrameName("GJ_statsBtn_001.png");
	auto myButton4 = CCMenuItemSpriteExtra::create(
		btn4,
		btn4,
		this,
		menu_selector(MenuLayer::onStats));
	menu->addChild(myButton4);

	auto btn5 = CCSprite::createWithSpriteFrameName("communityCreditsBtn_001.png");
	btn5->setScale(1.4);
	auto myButton5 = CCMenuItemSpriteExtra::create(
		btn5,
		btn5,
		this,
		menu_selector(CreditsLayer::create));
	menu->addChild(myButton5);

	// menu->setPositionY(CCBOTTOM + 45);
	menu->setPosition(0, 0);
	GameToolbox::alignItemsHorisontally(menu->getChildren(), 5, {CCMIDX, CCBOTTOM + 45}, false);

	this->addChild(menu, 100);

	// auto glview = cocos2d::CCEGLView::sharedOpenGLView();
	// glview->setDesignResolutionSize(winSize.width + 1000, winSize.height + 100, kResolutionNoBorder);
	// messing with this is funny

	/*
		auto children = reinterpret_cast<CCMenu*>(this->getChildren());

			int count = children->getChildrenCount();
			for(int i = 0; i < count; i++) {

				auto label = CCLabelBMFont::create(GDPS->itos(i).c_str(), "bigFont.fnt");

				auto node = reinterpret_cast<CCNode*>(children->getChildren()->objectAtIndex(i));

				label->setPosition(node->getPosition());
				this->addChild(label, 1000);

			}
			*/

	extern CCLabelBMFont *timerLabel;
	/*
		//other stuff
		timerLabel = CCLabelBMFont::create("0", "chatFont.fnt");
		timerLabel->setScale(2);
		timerLabel->setPosition(CCMIDX - 30, CCMIDY);
		this->addChild(timerLabel);

		this->schedule(schedule_selector(MenuLayerExt::onUpdate), 0.01);
	*/

	auto m_nPlayerJetpack = gm->getIntGameVariable("7001");
	gm->m_nPlayerSwing = gm->getIntGameVariable("6969");

	if (m_nPlayerJetpack == 0)
	{
		gm->setIntGameVariable("7001", 1);
	}
	if (gm->m_nPlayerSwing == 0)
	{
		gm->m_nPlayerSwing = 1;
	}
	GDPS->setPlayerSwing(gm->m_nPlayerSwing);
	GDPS->setPlayerJetpack(m_nPlayerJetpack);

	auto numReq = gm->getGameVariable("11000");

	if (numReq)
	{

		/*
			 cocos2d::extension::CCHttpRequest* request = new (std::nothrow) cocos2d::extension::CCHttpRequest();
				request->setUrl(AY_OBFUSCATE("http://game.gdpseditor.com/server/game/version2.php"));
				request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);


				request->setResponseCallback(this,callfuncND_selector(MenuLayerExt::onRequestCompleted));
				request->setTag("Post test2");
				*((int *)request + 8) = 1;
				//*((int *)request + 16) = 0;
				cocos2d::extension::CCHttpClient::getInstance()->send(request);
				request->release();

				gm->setGameVariable("11000", false);
				*/
	}
	else
	{

		auto particles = gm->getIntGameVariable("11001");

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
			this->addChild(CCParticleSnow::create());
			break;
		case 11:
			this->addChild(CCParticleRain::create());
		case 12:
			this->addChild(CCParticleSnow::create());
			this->addChild(CCParticleSmoke::create());
		}
	}

	return ret;
};

/*
server adresses

http://ps.fhgdps.com/GDPSEditor22

0x7A8513
0x7A8941
0x7A8C21
0x7A8CB0
0x7A8D5C
0x7A8DE7
0x7A8DE7
0x7A8DE7
0x7A8EAA
0x7A8EE5
0x7A8F53
0x7A8FCC
0x7A9054
0x7A90BB
0x7A9107
0x7A914B
0x7A918A
0x7A91D3
0x7A922C
0x7A92A4
0x7A9353
0x7A93A3
0x7A93FA
0x7A94A0
0x7B7A1D
0x7B8191
0x7B8202
0x7B8278
0x7B8446
0x7B8A5D
0x7B8A99
*/