#include "cocos2d.h"
#include <jni.h>
#include <dlfcn.h>
#include "blaze.h"
#include "Logger.h"
#include "patch.h"
#include <cstdlib>
#include "hooks/MenuLayerExt.h"
#include "layers/ToolsLayer.h"
#include "hooks/MoreSearchLayerExt.h"
#include "hooks/EditLevelLayerExt.h"
#include "hooks/LevelEditorLayerExt.h"
#include "hooks/MultiplayerLayerExt.h"
#include "hooks/EditorUIExt.h"
#include "hooks/PauseLayerExt.h"
#include "hooks/PlayLayerExt.h"
#include "hooks/MoreOptionsLayerExt.h"
#include "hooks/GJGarageLayerExt.h"
#include "hooks/AccountRegisterLayerExt.h"
#include "hooks/LevelBrowserLayerExt.h"
#include "hooks/AccountLayerExt.h"
#include "hooks/CreatorLayerExt.h"
#include "hooks/onPlaytestExt.h"
#include "GDPSManager.h"
#include "layers/GDPSSettings.h"
#include <gd.h>
#include <hooking.h>
#include <CCFileUtils.h>
#include "../Encryption/Encryption.h"
#include "../ZLIB/zlib.h"
#include <string>
#include <iostream>
#include <cocos2dx/extensions/CCScale9Sprite.h>
#include "LevelBrowserLayer.h"
#include "GJSearchObject.h"
#include "SetGroupIDLayer.h"
#include "DialogLayer.h"
#include "DialogObject.h"
#include "GJGameLevel.h"
#include "GJUserScore.h"
#include "CommentCell.h"
#include "obfuscate.h"

using namespace std;
#define contains(x, y) strstr(x, y) != NULL

#define null NULL
#define targetLibName ("libcocos2dcpp.so")

patch *pauseBtn = new patch();
patch *groupIDLayerPatches = new patch();
bool inEditor;
bool userDataChanged;
bool legendaryChanged;
bool shouldSendDefaultValue;
bool addBadges;
bool isSpider;
bool isSpider2;
bool inSettings;
bool inParticle;
CCLabelBMFont *CI;
CCLabelBMFont *CO;
CCLabelBMFont *timerLabel;
float timer = 0;
int CSPcount;

template <class T>
void *getPointer(T value)
{
	auto val = reinterpret_cast<void *&>(value);
	return val;
}

template <typename T>
std::string itos(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}
// https://stackoverflow.com/a/13636164

void *getPointerFromSymbol(void *handle, const char *symbol)
{
	return reinterpret_cast<void *>(dlsym(handle, symbol));
}

inline long mid_num(const std::string &s)
{
	return std::strtol(&s[s.find('_') + 1], nullptr, 10);
}

std::vector<std::string> SplitIntoVector(const std::string &s, const char *c)
{
	std::vector<std::string> newV;
	std::string::size_type i = 0;
	std::string::size_type j = s.find(c);

	while (j != std::string::npos)
	{
		newV.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(c, j);

		if (j == std::string::npos)
			newV.push_back(s.substr(i, s.length()));
	}

	if (newV.size() == 0)
		newV.push_back(s);

	return newV;
}
// https://github.com/TheNachoBIT/TheRetroFunkProject/blob/79e13c00e1aedefbd5a3d52a1c63a9b09874dc11/Geometria/String/StringAPI.cpp#L34
// Nachobit god

std::string string_from_vector(const std::vector<std::string> &pieces)
{
	return std::accumulate(pieces.begin(), pieces.end(), std::string(""));
}
// https://stackoverflow.com/a/15347198

int prevRelease = -1;
void (*release_trp)(PlayerObject *self, int button);
void release_hk(PlayerObject *self, int button)
{
	if (prevRelease == 5)
	{
		prevRelease = button;
		return;
	}

	prevRelease = button;
	release_trp(self, button);
}

char const *(*loading_trp)(LoadingLayer *);
char const *loading_hook(LoadingLayer *layer)
{

	const char *server = AY_OBFUSCATE("http://game.gdpseditor.com/server");
	// const char* server = AY_OBFUSCATE("http://game.gdpseditor.com/servel");

	// const char* server = AY_OBFUSCATE("http://gmdpseditor.7m.pl/database"); // testing on old server

	const char *server_b64 = AY_OBFUSCATE("aHR0cDovL2dhbWUuZ2Rwc2VkaXRvci5jb20vc2VydmVy");
	// const char* server_b64 = AY_OBFUSCATE("aHR0cDovL2dhbWUuZ2Rwc2VkaXRvci5jb20vc2VydmVs");
	// const char* server_b64 = AY_OBFUSCATE("aHR0cDovL2dtZHBzZWRpdG9yLjdtLnBsL2RhdGFiYXNl"); // testing on old server

	GDPS->changeServers(server, server_b64);

	auto gdpsmanager = GDPSManager::sharedState();
	auto gm = GameManager::sharedState();

	gm->setGameVariable("11000", true); // shit in menu layer for the request
	gm->setGameVariable("0023", false); // smooth fix
	gm->setGameVariable("0074", true);	// show restart button

	extern bool shouldSendDefaultValue;
	shouldSendDefaultValue = true;

	gdpsmanager->playTest = GameManager::sharedState()->getGameVariable("100001");
	gdpsmanager->pauseBtn = GameManager::sharedState()->getGameVariable("100005");

	if (!gdpsmanager->playTest)
	{
	}

	if (gdpsmanager->pauseBtn)
	{
		pauseBtn->Modify();
	}

	/* 	 	auto crashfix = gm->getGameVariable("100004");

		if(!crashfix) {
					patch *tmp = new patch();
				tmp->addPatch("libcocos2dcpp.so", 0x44D2AE, "00 bf 00 bf"); // editor fix, breaks levels

				tmp->Modify();
		}else{
							patch *tmp = new patch();

						tmp->addPatch("libcocos2dcpp.so", 0x7A617F, "47 4a 5f 63 72 65 61 74 65 42 74 6e 5f 30 30 32"); // editor fix, breaks levels
				tmp->Modify();




			}
	 */
	return loading_trp(layer);
}

void (*dict1)(CCDictionary *, CCObject *, int);
void dict_hk1(cocos2d::CCDictionary *d, CCObject *obj, int key)
{

	// LOGD("%s", obj, key);

	switch (key)
	{

	case 0x7DF: // add if statement for string check
		return dict1(d, CCString::create("edit_eRotateBtn_001.png"), key);
		break;

	case 0x778:
		return dict1(d, CCString::create("edit_eAdvRandomBtn_001.png"), key);
		break;

	case 0x7E0:
		return dict1(d, CCString::create("edit_eGuideBtn_001.png"), key);
		break;

	case 0xA8D:
		return dict1(d, CCString::create("edit_ePauseMoverBtn_001.png"), key);
		break;

	case 0xA8E:
		return dict1(d, CCString::create("edit_eResumeComBtn_001.png"), key);
		break;

	case 0x80E:
		return dict1(d, CCString::create("edit_eEdgeBtn_001.png"), key);
		break;

	default:

		dict1(d, obj, key);
		break;
	}
}

GameObject *create_hk(int key)
{
	auto tb = ObjectToolbox::sharedState()->intKeyToFrame(key);

	// LOGD("GAMEOBJECT: %s", tb);
	if (strstr(tb, "pixelart") != NULL && strstr(tb, "b_001") == NULL)
	{
		auto pixelKey = mid_num(tb);
		// LOGD("ART: %i", pixelKey);

		return old4(pixelKey > 140 ? 1 : key);
	}

	if (!strcmp(tb, "pixelb_03_01_001.png"))
		return old4(1);

	if (!strcmp(tb, "pixelb_03_02_001.png"))
		return old4(1);

	if (strstr(tb, "gdh") != NULL
		//  || strstr( tb, "secretCoin" ) != NULL
		|| strstr(tb, "fireball") != NULL || strstr(tb, "fire_b") != NULL || strstr(tb, "gj22_anim") != NULL || strstr(tb, "pixel") != NULL || strstr(tb, "gjHand2") != NULL)
		return old4(1);

	auto o = old4(key);

	return o;
}

bool isGauntlet = false;
CCSpriteFrame *(*old5)(CCSpriteFrameCache *, const char *) = nullptr;
CCSpriteFrame *sprite_hk(CCSpriteFrameCache *ptr, const char *s)
{

	if (!strcmp(s, "GJ_fullBtn_001.png"))
		return old5(ptr, "GJ_creatorBtn_001.png");

	if (!strcmp(s, "GJ_freeLevelsBtn_001.png"))
		return old5(ptr, "GJ_moreGamesBtn_001.png");

	if (!strcmp(s, "GJ_stuffTxt_001.png") ||
		!strcmp(s, "GJ_twitterTxt_001.png") ||
		!strcmp(s, "GJ_youtubeTxt_001.png") ||
		!strcmp(s, "GJ_twitchTxt_001.png") ||
		!strcmp(s, "GJ_freeStuffBtn_001.png"))
		return old5(ptr, "transparent.png");

	if (!strcmp(s, "GJ_epicCoin2_001.png"))
	{

		if (GM->getIntGameVariable("52342") >= 3)
		{

			return old5(ptr, "GJ_epicCoin3_001.png");
		}
		else
		{
			return old5(ptr, s);
		}
	}

	extern bool inSettings;

	if (inSettings)
	{

		if (!strcmp(s, "gj_spiderBtn_off_001.png"))
		{
			extern bool isSpider;

			if (!isSpider)
			{
				isSpider = true;
				return old5(ptr, "gj_spiderBtn_off_001.png");
			}
			{
				isSpider = false;
				return old5(ptr, "gj_swingBtn_off_001.png");
			}
		}

		if (!strcmp(s, "gj_spiderBtn_on_001.png"))
		{
			extern bool isSpider2;

			if (!isSpider2)
			{
				isSpider2 = true;
				return old5(ptr, "gj_spiderBtn_on_001.png");
			}
			{
				isSpider2 = false;
				return old5(ptr, "gj_swingBtn_on_001.png");
			}
		}
	}

	return old5(ptr, s);
}

void (*save_trp)(void *);
void save_hook(void *self)
{
	GDPSManager::sharedState()->save();
	return save_trp(self);
}

void OptionsLayer::onGDPSSettings(CCObject *sender)
{
	GDPSManager::sharedState()->isGDPSSettings = true;
	MoreOptionsLayer::create()->show();
}

void (*ol_customSetup_trp)(OptionsLayer *layer);
void OptionsLayer_customSetup_hk(OptionsLayer *layer)
{
	ol_customSetup_trp(layer);
	CCMenu *m = *((CCMenu **)layer + 116);
	CCMenuItemSpriteExtra *btn = dynamic_cast<CCMenuItemSpriteExtra *>(m->getChildren()->objectAtIndex(2));
	btn->setPositionX(-80);

	auto gdpsSetting = ButtonSprite::create("GDPS Settings", 125, 10, 10, 1);
	auto myButton = CCMenuItemSpriteExtra::create(
		gdpsSetting,
		gdpsSetting,
		layer,
		menu_selector(OptionsLayer::onGDPSSettings));
	auto button = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
	auto myButton1 = CCMenuItemSpriteExtra::create(
		button,
		button,
		layer,
		menu_selector(VideoOptionsLayer::onAdvanced));
	myButton1->setPosition(-140, -55);

	myButton->setPositionX(80);
	m->addChild(myButton);
	m->addChild(myButton1);
}
extern int restartCounter;

int (*onToggleTrampoline)(void *pthis, const char *val);
void hook_onToggle(void *pthis, const char *val)
{
	int v = atoi(val);

	onToggleTrampoline(pthis, val);

	if (v == 0074)
	{

		restartCounter = restartCounter++;
	}

	if (v > 100000)
	{
		if (v == 100001)
		{
			GDPS->playTest = !GDPS->playTest;
			if (!GDPS->playTest)
			{
			}
			else
			{
			}
		}

		if (v == 100002)
		{
			GDPSManager::sharedState()->oldTextures = !GDPSManager::sharedState()->oldTextures;
			GM->reloadAll(false, false, true);
			GM->setGameVariable("100002", GDPSManager::sharedState()->oldTextures);
		}

		/* 		if (v == 100003)
				{
					//GM->reloadAll(true, true, true);
					GM->setGameVariable("100003", GDPSManager::sharedState()->cleanMenu);
				}
		 */
		if (v == 100004)
		{
			GM->reloadAll(false, false, true);
		}

		if (v == 100005)
		{
			GDPS->pauseBtn = !GDPS->pauseBtn;
			if (GDPS->pauseBtn)
			{
				pauseBtn->Modify();
			}
			else
			{
				pauseBtn->Restore();
			}
		}
	}
}

void (*CCFileUtils_addSearchPath_trp)(CCFileUtils *self, const char *searchpath);
void CCFileUtils_addSearchPath_hk(CCFileUtils *self, const char *searchpath)
{
	CCLog("HELLO!!!");
	CCFileUtils_addSearchPath_trp(self, searchpath);
	if (GDPSManager::sharedState()->oldTextures)
	{
		auto fileUtils = CCFileUtils::sharedFileUtils();
		fileUtils->removeAllPaths();
		CCFileUtils_addSearchPath_trp(self, "assets/oldtp/");
		CCFileUtils_addSearchPath_trp(self, "assets/");
		CCFileUtils_addSearchPath_trp(self, searchpath);
	}
}

void (*dataLoaded_trp)(GameManager *self, DS_Dictionary *a1);
void GameManager_dataLoaded_hk(GameManager *self, DS_Dictionary *a1)
{
	dataLoaded_trp(self, a1);

	self->setGameVariable("100002", GDPSManager::sharedState()->oldTextures);
	if (GDPSManager::sharedState()->oldTextures)
	{
		CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
		auto vect = fileUtils->getSearchPaths();
		fileUtils->removeAllPaths();
		fileUtils->addSearchPath("assets/");
		dataLoaded_trp(self, a1);
		fileUtils->removeAllPaths();
		fileUtils->addSearchPath("assets/oldtp/");
		fileUtils->addSearchPath("assets/");
	}
}

void *(*t_trp)(void *a1, const char *func);
void *t(void *a1, const char *func)
{
	LOGD("%s", func);
	return t_trp(a1, func);
}

cocos2d::_ccColor3B (*PlayerObject_updateGlowColortrp)(int value);
cocos2d::_ccColor3B PlayerObject_updateGlowColorhook(int value)
{
	return ccc3(255, 0, 0);
}

cocos2d::_ccColor3B (*GameManager_colorForIdx_trp)(GameManager *pthis, int value);
cocos2d::_ccColor3B GameManager_colorForIdx_hook(GameManager *pthis, int value)
{
	switch (value)
	{
	case 0:
		return ccc3(125, 255, 0);
	case 1:
		return ccc3(0, 255, 0);
	case 2:
		return ccc3(0, 255, 125);
	case 3:
		return ccc3(0, 255, 255);
	case 4:
		return ccc3(0, 125, 255);
	case 5:
		return ccc3(0, 0, 255);
	case 6:
		return ccc3(125, 0, 255);
	case 7:
		return ccc3(255, 0, 255);
	case 8:
		return ccc3(255, 0, 125);
	case 9:
		return ccc3(255, 0, 0);
	case 10:
		return ccc3(255, 125, 0);
	case 11:
		return ccc3(255, 255, 0);
	case 12:
		return ccc3(255, 255, 255);
	case 13:
		return ccc3(185, 0, 255);
	case 14:
		return ccc3(255, 185, 0);
	case 15:
		return ccc3(0, 0, 0);
	case 16:
		return ccc3(0, 200, 255);
	case 17:
		return ccc3(175, 175, 175);
	case 18:
		return ccc3(90, 90, 90);
	case 19:
		return ccc3(255, 125, 125);
	case 20:
		return ccc3(0, 175, 75);
	case 21:
		return ccc3(0, 125, 125);
	case 22:
		return ccc3(0, 75, 175);
	case 23:
		return ccc3(75, 0, 175);
	case 24:
		return ccc3(125, 0, 125);
	case 25:
		return ccc3(175, 0, 75);
	case 26:
		return ccc3(175, 75, 0);
	case 27:
		return ccc3(125, 125, 0);
	case 28:
		return ccc3(75, 175, 0);
	case 29:
		return ccc3(255, 75, 0);
	case 30:
		return ccc3(150, 50, 0);

	case 31:
		return ccc3(150, 100, 0);
	case 32:
		return ccc3(100, 150, 0);
	case 33:
		return ccc3(0, 150, 100);
	case 34:
		return ccc3(0, 100, 150);
	case 35:
		return ccc3(100, 0, 150);
	case 36:
		return ccc3(150, 0, 100);
	case 37:
		return ccc3(150, 0, 0);
	case 38:
		return ccc3(0, 150, 0);
	case 39:
		return ccc3(0, 0, 150);
	case 40:
		return ccc3(125, 255, 175);
	case 41:
		return ccc3(125, 125, 255);

	case 42:
		return ccc3(255, 215, 215);
	case 43:
		return ccc3(115, 0, 0);
	case 44:
		return ccc3(95, 0, 0);
	case 45:
		return ccc3(60, 0, 0);
	case 46:
		return ccc3(130, 80, 80);
	case 47:
		return ccc3(130, 50, 50);
	case 48:
		return ccc3(85, 40, 40);
	case 49:
		return ccc3(190, 255, 225);
	case 50:
		return ccc3(140, 255, 225);
	case 51:
		return ccc3(0, 255, 200);
	case 52:
		return ccc3(55, 160, 125);
	case 53:
		return ccc3(50, 110, 95);
	case 54:
		return ccc3(40, 85, 70);
	case 55:
		return ccc3(0, 100, 100);
	case 56:
		return ccc3(0, 70, 70);

	case 57:
		return ccc3(255, 185, 110);
	case 58:
		return ccc3(255, 160, 60);
	case 59:
		return ccc3(160, 100, 60);
	case 60:
		return ccc3(115, 70, 50);

	case 61:
		return ccc3(125, 75, 55);
	case 62:
		return ccc3(80, 50, 30);
	case 63:
		return ccc3(100, 50, 30);
	case 64:
		return ccc3(75, 30, 0);
	case 65:
		return ccc3(150, 255, 255);
	case 103:
		return ccc3(0, 0, 115);
	case 104:
		return ccc3(0, 10, 85);
	case 66:
		return ccc3(0, 70, 110);
	case 67:
		return ccc3(0, 55, 80);
	case 68:
		return ccc3(0, 40, 60);
	case 69:
		return ccc3(115, 190, 255);
	case 70:
		return ccc3(115, 190, 255);
	case 71:
		return ccc3(75, 126, 180);
	case 72:
		return ccc3(50, 80, 115);
	case 73:
		return ccc3(255, 255, 74);
	case 74:
		return ccc3(255, 255, 130);
	case 75:
		return ccc3(255, 255, 130);
	case 76:
		return ccc3(255, 225, 155);
	case 77:
		return ccc3(80, 50, 10);
	case 78:
		return ccc3(200, 165, 110);
	case 79:
		return ccc3(170, 125, 80);
	case 80:
		return ccc3(110, 80, 55);

	case 81:
		return ccc3(190, 180, 255);
	case 82:
		return ccc3(190, 125, 255);
	case 83:
		return ccc3(60, 0, 150);
	case 84:
		return ccc3(60, 0, 100);
	case 85:
		return ccc3(80, 80, 155);
	case 86:
		return ccc3(110, 70, 180);
	case 87:
		return ccc3(90, 50, 125);
	case 88:
		return ccc3(70, 40, 100);
	case 89:
		return ccc3(200, 255, 170);
	case 90:
		return ccc3(210, 255, 100);
	case 91:
		return ccc3(210, 255, 50);
	case 92:
		return ccc3(0, 100, 0);
	case 93:
		return ccc3(0, 60, 0);
	case 94:
		return ccc3(255, 175, 255);
	case 95:
		return ccc3(75, 0, 50);
	case 96:
		return ccc3(255, 125, 255);
	case 97:
		return ccc3(175, 85, 180);
	case 98:
		return ccc3(135, 60, 135);
	case 99:
		return ccc3(100, 50, 100);
	case 105:
		return ccc3(110, 0, 60);
	case 100:
		return ccc3(225, 225, 225);
	case 101:
		return ccc3(125, 125, 125);
	case 102:
		return ccc3(65, 65, 65);

	default:
		return ccc3(255, 255, 255);
	}
}

bool (*levelsettings)(LevelSettingsLayer *, LevelSettingsObject *, LevelEditorLayer *);
bool levelsettings_hk(LevelSettingsLayer *ptr, LevelSettingsObject *a2, LevelEditorLayer *a3)
{
	inSettings = true;

	auto ret = levelsettings(ptr, a2, a3);

	extern bool inEditor;
	inEditor = false;

	auto winsize = cocos2d::CCDirector::sharedDirector()->getWinSize();

	auto dpad = CCSprite::createWithSpriteFrameName("Dpad_Btn.png");
	dpad->setPosition({winsize.width / 2 - 90, winsize.height - 290});
	dpad->setScale(.5);
	ptr->addChild(dpad);

	auto menu = CCMenu::create();

	ptr->addChild(menu);

	return ret;
};

bool (*lvlsettings_close)(LevelSettingsLayer *, CCObject *);
bool lvlsettings_close_hk(LevelSettingsLayer *ptr, CCObject *a2)
{

	auto ret = lvlsettings_close(ptr, a2);

	extern bool inEditor;
	inEditor = true;

	return ret;
};

bool (*GaragLayer_init_trp)(CCLayer *pthis);
bool GaragLayer_init_hook(CCLayer *pthis)
{
	auto res = GaragLayer_init_trp(pthis);

	auto winsize = CCDirector::sharedDirector()->getWinSize();
	{
		auto icons_node = reinterpret_cast<CCNode *>(pthis->getChildren()->objectAtIndex(6));
		icons_node->setPositionX(winsize.width / 2);
		icons_node->setPositionY(icons_node->getPositionY() + 10);
	}

	{
		auto icons_node = reinterpret_cast<CCNode *>(pthis->getChildren()->objectAtIndex(5));
		icons_node->setPositionX(winsize.width / 2);
		icons_node->setPositionY(icons_node->getPositionY() - 15);
	}

	{
		auto icons_node = reinterpret_cast<CCNode *>(pthis->getChildren()->objectAtIndex(7));
		icons_node->setPositionY(icons_node->getPositionY() - 20);
	}

	{
		auto icons_node = reinterpret_cast<CCNode *>(pthis->getChildren()->objectAtIndex(4));
		icons_node->setPositionY(icons_node->getPositionY() - 20);
	}

	{
		auto artBottomL = cocos2d::CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		artBottomL->setPosition(ccp(artBottomL->getScaledContentSize().width / 2, artBottomL->getScaledContentSize().height / 2));
		pthis->addChild(artBottomL);
	}

	{

		auto artBottomL = cocos2d::CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		artBottomL->setPosition(ccp(-artBottomL->getScaledContentSize().width / 2 + winsize.width, artBottomL->getScaledContentSize().height / 2));
		artBottomL->setFlipX(true);
		pthis->addChild(artBottomL);
	}

	{
		auto palette_menu = CCMenu::create();
		auto paletteSprite = cocos2d::CCSprite::create("GJ_paletteBtn_001.png");
		auto btn = CCMenuItemSpriteExtra::create(
			paletteSprite,
			paletteSprite,
			pthis,
			nullptr // static_cast<cocos2d::SEL_MenuHandler>(&ColoursPalette::onColorClick)
		);
		palette_menu->addChild(btn);
		btn->setPositionX(-winsize.width / 2 + 30);
		pthis->addChild(palette_menu);
	}

	// GJ_sideArt_001.png

	return res;
}

// fix profile btn
void (*MenuLayer_updateUserProfileButtonO)(MenuLayer *);
void MenuLayer_updateUserProfileButtonH(MenuLayer *self)
{
	auto AM = GJAccountManager::sharedState();

	self->_profileBtn()->setVisible(AM->isLoggedIn());
	self->_playerUsernameLabel()->setVisible(true);
	self->_playerUsernameLabel()->setString(AM->_accountUserName().c_str());

	if (AM->isLoggedIn())
	{
		self->_profileBtn()->setVisible(true);
		self->_playerUsernameLabel()->limitLabelWidth(70, 0.7, 0);
	}
}

void (*ProfilePage_loadPageFromUserInfoO)(ProfilePage *, GJUserScore *);
void ProfilePage_loadPageFromUserInfoH(ProfilePage *self, GJUserScore *userData)
{
	ProfilePage_loadPageFromUserInfoO(self, userData);

	// self->_userName()->setPositionX(self->_userName()->getPositionX() + 20);
	/*
	badge lists
	1 = mod
	2 = elder
	3 = booster
	4 = verified
	5 = youtuber
	6 = map pack creator (no badge)
	7 = dev
	8 = admin
	9 = owner

		//std::vector<std::string> vector = SplitIntoVector(modBadge, "0");
//std::string sVector = string_from_vector(vector);
	*/

	unsigned long long int modBadgeLevel = userData->modBadge_;

	auto texture1 = CCString::createWithFormat("modbadge = %llu", modBadgeLevel)->getCString();
	/*
		auto label33 = CCLabelBMFont::create("O", "chatFont.fnt");
		label33->setPosition(226, 283);
		label33->setScale(1);
		//label33->setColor(ccc3(00,00,00));
		label33->setVisible(false);
		self->m_pLayer->addChild(label33,60);
		//test labels

		auto label44 = CCLabelBMFont::create("O", "chatFont.fnt");
		label44->setPosition(CCMIDX - 60, 273);
		label44->setScale(1);
		label44->setVisible(false);
		//label44->setColor(ccc3(00,00,00));
		self->m_pLayer->addChild(label44,60);
		*/

	CCSprite *original; // original modbadge sprite

	extern bool addBadges; // refresh bug
	auto userName = self->_userName();

	// if mod badge is more than 2 and if the function is not being called by the refresh button
	if (modBadgeLevel > 2 && addBadges)
	{

		addBadges = false;

		// original mod badge finding
		for (int i = 0; i < self->_someArray()->count(); i++)
		{
			auto thing = (CCSprite *)self->_someArray()->objectAtIndex(i);

			if (thing->getChildrenCount() == 0 && CCMIDX - 110 < thing->getPositionX() && thing->getPositionY() > 283)
			{

				thing->setVisible(false);
				original = thing;

				break;
			}
		}
		// social button finding (not working idk)
		for (int i = 0; i < self->_someArray()->count(); i++)
		{
			auto thing = (CCMenuItemSpriteExtra *)self->_someArray()->objectAtIndex(i);

			if (thing->getPositionX() > 470 && thing->getPositionY() > 270)
			{

				thing->setPositionX(thing->getPositionX() + 100);

				break;
			}
		}
		// uhh I will try to explain
		std::string modBadge = itos(modBadgeLevel); // int modBadge -> string modBadge
		int length = modBadge.length();				// length of modBadge (with 0's)

		// for each 0 (separator) in the string, add one to the real length (the total of textures)
		// the real length is 1 and not 0 because there is one 0 missing.
		// for example in 50150206, there are 3 0's but 4 textures get created (5,15,2,6)
		int real_length = 1;
		for (int i = 0; i < length; i++)
		{
			if (modBadge.substr(i, 1) == "0")
				real_length++;
		}

		int array[real_length]; // create the array with real length
		// int* array = new int[real_length]; //create the array with real length

		int k = 0; // this is the array iterator

		std::string b = ""; // this is the string to concatenate badges

		for (int i = 0; i < length; i++)
		{ // loop through all the characters again

			std::string ch = modBadge.substr(i, 1); // the char in modBadge

			if (ch == "0")
			{ // if char is 0 (the modbadge int finished)

				b = ""; // empty the string to concatenate (to be filled with the next char that is not 0)
				k++;	// add 1 to the array iterator, store next values to the next array spot
			}
			else
			{ // if the badge is not 0

				b = b + ch;
				// concatenate b with the current char
				//(if ch is the first char after a 0 then b will contain just ch because it got cleaned by the if == 0 before
				// if ch is the second char of the modBadge they will concatenate

				char *s = new char[b.length() + 1];

				strcpy(s, b.c_str()); // copy b
				array[k] = atoi(s);	  // add b to the correct array spot, k++ only happens when char is 0 (new mod badge beggins)
			}
		}
		auto texture = 10 > array[3] ? CCString::createWithFormat("modBadge_%02d_001.png", array[3])->getCString() : CCString::createWithFormat("modBadge_%02d_001.png", array[3])->getCString();
		auto ccstring = CCString::createWithFormat("%i", texture)->getCString();
		auto label55 = CCLabelBMFont::create(texture, "chatFont.fnt");
		label55->setPosition(490, 200);
		label55->setScale(0.7);
		label55->setVisible(false);
		self->m_pLayer->addChild(label55, 50);
		auto menu = CCMenu::create();
		menu->setPosition(0, 0);
		SEL_MenuHandler buttonCallback;
		for (int i = 0; i < real_length; i++)
		{

			bool isBadgeNew = array[i] > 2;

			switch (array[i])
			{
			case 1:
				buttonCallback = NULL;
				break;
			case 2:
				buttonCallback = NULL;
				break;

			case 3:
				buttonCallback = menu_selector(ToolsLayer::onBooster);
				break;

			case 4:
				buttonCallback = menu_selector(ToolsLayer::onVerified);
				break;

			case 5:
				buttonCallback = menu_selector(ToolsLayer::onYoutuber);
				break;

			case 7:
				buttonCallback = menu_selector(ToolsLayer::onDev);
				break;

			case 8:
				buttonCallback = menu_selector(ToolsLayer::onAdmin);
				break;

			case 9:
				buttonCallback = menu_selector(ToolsLayer::onOwner);
				break;

			default:
				buttonCallback = menu_selector(ToolsLayer::onUndefined);
			}

			auto texture = CCString::createWithFormat("modBadge_%02d_001.png", array[i])->getCString();
			auto badge = CCSprite::createWithSpriteFrameName(texture);
			auto btn = CCMenuItemSpriteExtra::create(badge, badge, self->m_pLayer, buttonCallback);
			btn->setPosition(original->getPosition());
			if (isBadgeNew)
			{
				menu->addChild(btn);
			}
			else
			{
				self->m_pLayer->addChild(btn);
			}
			original->setPositionX(original->getPositionX() + 25);
		}

		original->setPositionX(original->getPositionX() - (real_length * 25));

		userName->setPositionX(userName->getPositionX() + ((real_length * 25) - 25));
		self->m_pLayer->addChild(menu, 50);
	}

	// swing
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	int iconIterator = 1;
	for (int i = 0; i < self->_someArray()->count(); i++)
	{
		auto thang = (CCNode *)self->_someArray()->objectAtIndex(i);

		// pos Y = 200
		if (thang->getPositionY() < 201 && thang->getPositionY() > 199)
		{
			thang->setPositionX(winSize.width / 2 + 41 * iconIterator - 184);

			// change swing icon
			if (iconIterator == 8)
			{
				auto icon = (SimplePlayer *)thang;
				thang->setPositionX(thang->getPositionX() + 7);

				icon->updatePlayerFrame(reinterpret_cast<GJUserScoreExt *>(userData)->playerSwing_, IconType::Swing);
			}

			iconIterator++;
		}
	}
}

void (*CommentCell_loadFromCommentO)(CommentCell *, GJComment *);
void CommentCell_loadFromCommentH(CommentCell *self, GJComment *commentData)
{
	CommentCell_loadFromCommentO(self, commentData);

	// self->m_pLayer->addChild(label33,50);
	// test labels

	// testing
	unsigned long long int modBadgeLevel = commentData->_modBadge();

	if (modBadgeLevel > 2)
	{
		auto commentLayer = (CCLayer *)self->getChildren()->objectAtIndex(1);

		auto texture1 = CCString::createWithFormat("%llu", modBadgeLevel)->getCString();

		auto label33 = CCLabelBMFont::create(texture1, "chatFont.fnt");
		// label33->setPosition(50, 20);
		label33->setScale(.65);
		commentLayer->addChild(label33, 5000);
		label33->setVisible(false);

		std::string modBadge = itos(modBadgeLevel);
		int length = modBadge.length();
		int real_length = 1;
		for (int i = 0; i < length; i++)
		{
			if (modBadge.substr(i, 1) == "0")
				real_length++;
		}
		int array[real_length];
		int k = 0;
		std::string b = "";
		for (int i = 0; i < length; i++)
		{
			std::string ch = modBadge.substr(i, 1);
			if (ch == "0")
			{
				b = "";
				k++;
			}
			else
			{
				b = b + ch;
				char *s = new char[b.length() + 1];
				strcpy(s, b.c_str());
				array[k] = atoi(s);
			}
		}
		int pos = 17;
		for (int c = 0; c < commentLayer->getChildrenCount(); c++)
		{
			auto percentage = (CCSprite *)commentLayer->getChildren()->objectAtIndex(c);

			// getting mod badge lolol
			if (percentage->getChildrenCount() != 0 && percentage->getPositionX() > 60 && percentage->getPositionY() > 20 && percentage->getPositionX() < 250 && percentage->getScale() < 1)
			{

				percentage->setPositionX(percentage->getPositionX() + ((real_length * pos) - pos));
				break;
			}
		}

		for (int c = 0; c < commentLayer->getChildrenCount(); c++)
		{
			auto modBadgeSpr = (CCSprite *)commentLayer->getChildren()->objectAtIndex(c);

			if (modBadgeSpr->getChildrenCount() == 0)
			{
				modBadgeSpr->setVisible(false);
				label33->setPosition(modBadgeSpr->getPosition());
				break;
			}
		}

		SEL_MenuHandler buttonCallback;
		auto menu = CCMenu::create();
		menu->setPosition(0, 0);

		for (int i = 0; i < real_length; i++)
		{

			bool isBadgeNew = array[i] > 2;

			switch (array[i])
			{
			case 1:
				buttonCallback = menu_selector(ToolsLayer::onAlert);
				break;
			case 2:
				buttonCallback = menu_selector(ToolsLayer::onAlert);
				break;

			case 3:
				buttonCallback = menu_selector(ToolsLayer::onBooster);
				break;

			case 4:
				buttonCallback = menu_selector(ToolsLayer::onVerified);
				break;

			case 5:
				buttonCallback = menu_selector(ToolsLayer::onYoutuber);
				break;

			case 7:
				buttonCallback = menu_selector(ToolsLayer::onDev);
				break;

			case 8:
				buttonCallback = menu_selector(ToolsLayer::onAdmin);
				break;

			case 9:
				buttonCallback = menu_selector(ToolsLayer::onOwner);
				break;

			default:
				buttonCallback = menu_selector(ToolsLayer::onUndefined);
			}

			auto texture = CCString::createWithFormat("modBadge_%02d_001.png", array[i])->getCString();
			auto badge = CCSprite::createWithSpriteFrameName(texture);
			badge->setScale(.65);
			auto btn = CCMenuItemSpriteExtra::create(badge, badge, commentLayer, buttonCallback);
			btn->setPosition(label33->getPosition());
			if (isBadgeNew)
			{
				menu->addChild(btn);
			}
			else
			{
				commentLayer->addChild(btn);
			}

			label33->setPositionX(label33->getPositionX() + pos);
		}

		commentLayer->addChild(menu);

		label33->setPositionX(label33->getPositionX() + ((real_length * pos) + pos));
		// pcg->setPositionX(pcg->getPositionX() + ((real_length * 25) - 25));
	}
}

bool (*SelectArtLayer_initO)(SelectArtLayer *, SelectArtType);
bool SelectArtLayer_initH(SelectArtLayer *self, SelectArtType type)
{
	if (!SelectArtLayer_initO(self, type))
		return false;

	int maxTextures = 25; // not sure if this is right but even if its too much it won't crash

	if (type == SelectArtType::ground)
		maxTextures = 17;

	auto spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();

	for (int i = 0; i < maxTextures; i++)
	{
		auto bgBtn = (CCMenuItemSpriteExtra *)self->_bgSelectMenu()->getChildren()->objectAtIndex(i);

		if (bgBtn != nullptr)
		{
			const char *frameSprStr;

			if (type == SelectArtType::ground)
				frameSprStr = "gIcon_%02d_001.png";
			else
				frameSprStr = "bgIcon_%02d_001.png";

			auto frameStr = CCString::createWithFormat(frameSprStr, i + 1)->getCString();

			auto frameSpr = spriteCache->spriteFrameByName(frameStr);

			if (frameSpr != nullptr)
			{
				auto bgBtnSpr = (CCSprite *)bgBtn->getChildByTag(1);

				if (bgBtnSpr != nullptr)
					bgBtnSpr->setDisplayFrame(frameSpr);
			}
		}
	}

	// the ok btn is the only btn without a tag apparently
	if (type == SelectArtType::background)
	{
		auto okBtn = (CCMenuItemSpriteExtra *)self->_bgSelectMenu()->getChildren()->objectAtIndex(self->_bgSelectMenu()->getChildrenCount() - 1);

		okBtn->setPositionX(80);
		okBtn->setPositionY(okBtn->getPositionY() + 5);
	}

	return true;
}

bool (*SetGroupIDLayer_initO)(SetGroupIDLayer *, GameObject *, CCArray *);
bool SetGroupIDLayer_initH(SetGroupIDLayer *self, GameObject *object, CCArray *idk)
{
	if (!SetGroupIDLayer_initO(self, object, idk))
		return false;

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	// moving the order input
	auto orderInput = self->_orderInput();
	orderInput->setPosition(winSize.width / 2 - 160, winSize.height / 2 - 130);

	auto orderInputBG = extension::CCScale9Sprite::create("square02_small.png", {0, 0, 40, 40});
	orderInputBG->setPosition(orderInput->getPosition());
	orderInputBG->setOpacity(100);
	orderInputBG->setContentSize(CCSize(50, 30));

	auto label = CCLabelBMFont::create("Reverse\nOrder", "bigFont.fnt");
	label->setPosition(orderInput->getPositionX() + 60, orderInput->getPositionY());
	label->setScale(.4);
	self->addChild(label);

	self->_m_pLayer()->addChild(orderInputBG, -1);

	auto menu = CCMenu::create();
	self->addChild(menu, 14);

	auto extraBtnSpr = ButtonSprite::create("Extra", 20, 0, .5, true, "goldFont.fnt", "GJ_button_04.png", 25);
	auto extraBtn = CCMenuItemSpriteExtra::create(extraBtnSpr, extraBtnSpr, self, menu_selector(SetGroupIDLayer::onExtra));
	extraBtn->setPosition(217.5, 80);

	menu->addChild(extraBtn);

	self->registerWithTouchDispatcher();
	CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
	self->setTouchEnabled(true);

	return true;
}
#include "InfoAlertButton.h"
void *(*SetupCameraRotatePopupO)(SetupCameraRotatePopup *, EffectGameObject *, cocos2d::CCArray *);
void *SetupCameraRotatePopupH(SetupCameraRotatePopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupCameraRotatePopupO(self, object, objects);

	auto dir = CCDirector::sharedDirector();
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	self->_degreesInputSlider()->setPosition(CCPoint(winSize.width / 2, winSize.height / 2 + 70));
	self->_degreesInput()->setPosition(CCPoint(winSize.width / 2 + 45, winSize.height / 2 + 105));

	auto moveInputBG = extension::CCScale9Sprite::create("square02_small.png", {0, 0, 40, 40});
	moveInputBG->setPosition(self->_degreesInput()->getPosition());
	moveInputBG->setOpacity(100);
	moveInputBG->setContentSize(CCSize(50, 30));

	const char *description = "Rotates the camera\n\
- Works by changing the rotation relative to 0 degrees instead of adding/subtracting the previous degrees.";

	auto btn = InfoAlertButton::create("Rotate Camera Trigger", description, 1);
	auto menu = CCMenu::create();
	menu->setPosition(0, 0);
	menu->addChild(btn);
	btn->setPositionX(CCMIDX + 190);
	btn->setPositionY(CCTOP - 25);
	self->m_pLayer->addChild(menu);

	self->_m_pLayer()->addChild(moveInputBG, -1);

	self->registerWithTouchDispatcher();
	CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
	self->setTouchEnabled(true);

	return ret;
}

#include "ColorSelectPopup.h"
#include "CCMenuItemToggler.h"

void *(*ColorSelectPopupO)(ColorSelectPopup *, EffectGameObject *, CCArray *, ColorAction *);
void *ColorSelectPopupH(ColorSelectPopup *self, EffectGameObject *object, CCArray *arr, ColorAction *a3)
{

	auto ret = ColorSelectPopupO(self, object, arr, a3);
	if(inParticle)
	return ret;

	auto layer = self->m_pLayer;
	extern int CSPcount;
	CSPcount = layer->getChildrenCount();
	int count = CSPcount;
	auto menu = self->_menu();
	int count2 = menu->getChildrenCount();

	if (count < 36)
	{

		auto COtoggle = reinterpret_cast<CCNode *>(menu->getChildren()->objectAtIndex(7));
		auto CIleftArrow = reinterpret_cast<CCNode *>(menu->getChildren()->objectAtIndex(8));

		extern CCLabelBMFont *CI;
		extern CCLabelBMFont *CO;
		CI = CCLabelBMFont::create("Channel ID", "goldFont.fnt");
		CI->setScale(.6);
		CI->setPosition(CIleftArrow->GPX() + 53, CIleftArrow->GPY() + 30);
		CI->setVisible(CIleftArrow->isVisible());
		CIleftArrow->getParent()->addChild(CI);

		CO = CCLabelBMFont::create("Copy Opacity", "bigFont.fnt");
		CO->setScale(.37);
		CO->setPosition(COtoggle->GPX() + 62, COtoggle->GPY());
		CO->setVisible(CIleftArrow->isVisible());
		CIleftArrow->getParent()->addChild(CO);
		return ret;
	}

	auto dir = CCDirector::sharedDirector();
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto multitrigger = reinterpret_cast<CCNode *>(menu->getChildren()->objectAtIndex(10));
	auto multitriggerL = reinterpret_cast<CCNode *>(layer->getChildren()->objectAtIndex(16));

	auto CIscaleReference = reinterpret_cast<CCNode *>(layer->getChildren()->objectAtIndex(19));
	auto COtoggle = reinterpret_cast<CCNode *>(menu->getChildren()->objectAtIndex(11));
	auto CIleftArrow = reinterpret_cast<CCNode *>(menu->getChildren()->objectAtIndex(15));
	auto CCtoggle = reinterpret_cast<CCMenuItemToggler *>(menu->getChildren()->objectAtIndex(7));

	extern CCLabelBMFont *CI;
	extern CCLabelBMFont *CO;
	CI = CCLabelBMFont::create("Channel ID", "goldFont.fnt");
	CI->setScale(.6);
	CI->setPosition(CIleftArrow->GPX() + 53, CIleftArrow->GPY() + 55);
	CI->setVisible(CIleftArrow->isVisible());
	CIleftArrow->getParent()->addChild(CI);

	CO = CCLabelBMFont::create("Copy Opacity", "bigFont.fnt");
	CO->setScale(.37);
	CO->setPosition(COtoggle->GPX() + 62, COtoggle->GPY() + 30);
	CO->setVisible(CIleftArrow->isVisible());
	CIleftArrow->getParent()->addChild(CO);

	for (int i = 0; i < count; i++)
	{

		auto node = reinterpret_cast<CCNode *>(layer->getChildren()->objectAtIndex(i));
		switch (i)
		{

		case 15: // spawn label
			node->setPosition(multitriggerL->getPosition());
			multitriggerL->setPositionX(node->GPX() + 80);
			break;

		case 14: // touch label
			node->setPositionX(node->GPX() - 15);
			break;

		case 33:
		case 34:
		case 35:
			node->setPositionX(node->GPX() + 40);
			break;

		case 7: // fade time slider
			node->setPositionX(node->GPX() - 25);
			break;

		case 8:
		case 9:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23: // opacity, slider, color id, etc
			node->setPositionY(node->GPY() + 30);
			break;
		}
	}

	count = menu->getChildrenCount();
	for (int i = 0; i < count; i++)
	{

		auto node = reinterpret_cast<CCNode *>(menu->getChildren()->objectAtIndex(i));

		switch (i)
		{

		case 9: // spawn btn
			node->setPosition(multitrigger->getPosition());
			multitrigger->setPositionX(node->GPX() + 80);
			break;
		case 8: // touch btn
			node->setPositionX(node->GPX() - 15);
			break;
		case 12:
		case 13:
		case 14:
		case 11:
		case 16:
		case 15:
			node->setPositionY(node->GPY() + 30);
			break;
		case 17:
			node->setPositionY(node->GPY() + 15);
		}
	}

	self->registerWithTouchDispatcher();
	CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
	self->setTouchEnabled(true);

	return ret;
}

void (*ToggleCopyColorO)(ColorSelectPopup *, CCObject *);
void ToggleCopyColorH(ColorSelectPopup *self, CCObject *a2)
{

	ToggleCopyColorO(self, a2);

	extern int CSPcount;
	CCMenuItemToggler *CCtoggle;

	if (CSPcount < 36)
	{
		CCtoggle = reinterpret_cast<CCMenuItemToggler *>(self->_menu()->getChildren()->objectAtIndex(6));
	}
	else
	{
		CCtoggle = reinterpret_cast<CCMenuItemToggler *>(self->_menu()->getChildren()->objectAtIndex(7));
	}
	extern CCLabelBMFont *CI;
	extern CCLabelBMFont *CO;

	CO->setVisible(CCtoggle->_isToggled());
	CI->setVisible(CCtoggle->_isToggled());
}

#include "SetupZoomTriggerPopup.h"
void *(*SetupZoomTriggerPopupO)(SetupZoomTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
void *SetupZoomTriggerPopupH(SetupZoomTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupZoomTriggerPopupO(self, object, objects);

	auto dir = CCDirector::sharedDirector();
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto layer = self->m_pLayer;

	auto setup = CCLabelBMFont::create("Setup Zoom", "goldFont.fnt");
	auto node = reinterpret_cast<CCNode *>(layer->getChildren()->objectAtIndex(2));
	setup->setPosition(node->GPX(), node->GPY() + 80);
	setup->setScale(.8);
	layer->addChild(setup);

	auto menu = self->_menu();
	auto array = menu->getChildren();

	const char *description = "Changes the zoom of the camera\n\
0 = default valuen\n\
zoom in = closer to the player\n\
zoom out = further away from the player";

	auto infoBtn = InfoAlertButton::create("Zoom Trigger", description, 1);
	auto originalInfoBtn = reinterpret_cast<CCNode *>(menu->getChildren()->objectAtIndex(1));

	infoBtn->setPosition(originalInfoBtn->GPX(), originalInfoBtn->GPY() + 30);
	originalInfoBtn->setPositionY(100000);
	menu->addChild(infoBtn);

	int count = menu->getChildrenCount();
	for (int i = 0; i < count; i++)
	{

		auto node = reinterpret_cast<CCNode *>(menu->getChildren()->objectAtIndex(i));

		switch (i)
		{

		case 0: // ok button
			node->setPositionY(node->GPY() - 30);
			break;
		case 2:
		case 3:
		case 4: // easing buttons
			node->setPositionX(node->GPX() + 90);
			node->setPositionY(node->GPY() + 20);
			break;
		case 5:
		case 6:
		case 7:
			node->setPositionY(node->GPY() - 30);
			break;
		}
		/*
		auto label = CCLabelBMFont::create(itos(i).c_str(), "chatFont.fnt");
		label->setColor({255,0,0});
		label->setPosition(node->getPosition());
		menu->addChild(label, 1001);
		*/
	}

	count = layer->getChildrenCount();
	for (int i = 0; i < count; i++)
	{

		auto node = reinterpret_cast<CCNode *>(layer->getChildren()->objectAtIndex(i));

		switch (i)
		{

		case 0:
			node->setContentSize(CCSize(370, 290));
			break;
		case 2:
		case 3:
			node->setPositionY(node->GPY() + 50);
			break;
		case 8:
		case 9:
			node->setPositionX(node->GPX() + 90);
			node->setPositionY(node->GPY() + 20);
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			node->setPositionX(node->GPX() + 90);
			node->setPositionY(node->GPY() + 20);
			break;
		case 10:
		case 11:
		case 12:
			node->setPositionY(node->GPY() - 30);
			break;
		}
		/*
		auto label = CCLabelBMFont::create(itos(i).c_str(), "bigFont.fnt");

		label->setScale(.6);
		label->setPosition(node->getPosition());
		layer->addChild(label, 1000);
		*/
	}

	self->registerWithTouchDispatcher();
	CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
	self->setTouchEnabled(true);

	return ret;
}

#include "SetupCollisionTriggerPopup.h"
void *(*SetupCollisionTriggerPopupO)(SetupCollisionTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
void *SetupCollisionTriggerPopupH(SetupCollisionTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupCollisionTriggerPopupO(self, object, objects);

	const char *description = "<cy>detects if 2 Collision Blocks with the selected block ids touch each other.</c>\n\
<cp>P detects if the player is touching a collision block with the selected block id.</c>\n\
<cl>Activate group makes the Collision Trigger behave like a Spawn Trigger once its condition is met.</c>\n\
<cg>\"Trigger on exit\" activates the trigger when 2 collision blocks stop touching each other</c>\n\
<cr>Warning: use as few dynamic collision blocks as possible to avoid massive lag!</c>";

	auto btn = InfoAlertButton::create("Collision Trigger", description, 1);
	auto menu = CCMenu::create();
	menu->setPosition(0, 0);
	menu->addChild(btn);
	auto node = reinterpret_cast<CCNode *>(self->m_pLayer->getChildren()->objectAtIndex(3));
	btn->setPositionX(node->getPositionX() - 60);
	btn->setPositionY(node->getPositionY() + 60);
	self->m_pLayer->addChild(menu);

	self->registerWithTouchDispatcher();
	CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
	self->setTouchEnabled(true);

	return ret;
}

bool (*levelinfoinit)(LevelInfoLayer *, GJGameLevel *, bool);
bool levelinfoinit_hk(LevelInfoLayer *ptr, GJGameLevel *level, bool a3)
{

	GM->setIntGameVariable("52342", level->_epic());
	auto r = levelinfoinit(ptr, level, a3);

	auto dir = cocos2d::CCDirector::sharedDirector();
	if (dir->getScheduler()->_fTimeScale != 1)
	{
		dir->getScheduler()->_fTimeScale = 1;
	}

	auto btn2 = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	auto myButton2 = CCMenuItemSpriteExtra::create(
		btn2,
		nullptr,
		ptr,
		menu_selector(MenuLayer::onOptions));
	btn2->setScale(.7);
	auto bottomMenu2 = CCMenu::create();
	bottomMenu2->addChild(myButton2);
	reinterpret_cast<CCSprite *>(bottomMenu2)->setPosition({CCLEFT + 65, CCTOP - 25});
	ptr->addChild(bottomMenu2);

	/*
		auto string = CCString::createWithFormat("level->featured = %i", level->levelID)->getCString();

		auto label33 = CCLabelBMFont::create(string, "chatFont.fnt");
		label33->setPosition(CCLEFT + 100, CCTOP - 100);
		label33->setScale(2);

	*/
	return r;
}

bool (*setUpLevelInfo)(EditLevelLayer *, GJGameLevel *);
bool setUpLevelInfo_hk(EditLevelLayer *ptr, GJGameLevel *level)
{

	auto ret = setUpLevelInfo(ptr, level);

	extern bool inEditor;
	inEditor = false;

	auto dir = cocos2d::CCDirector::sharedDirector();
	if (dir->getScheduler()->_fTimeScale != 1)
	{
		dir->getScheduler()->_fTimeScale = 1;
	}

	auto menu = ptr->_btnMenu();

	// creator btn

	auto editBtn = (CCMenuItemSpriteExtra *)menu->getChildren()->objectAtIndex(0);

	auto editBtnCustom = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png"), nullptr, ptr, menu_selector(EditLevelLayer::onEditCustom));
	menu->addChild(editBtnCustom);
	editBtnCustom->setPosition(editBtn->getPosition());
	editBtn->removeFromParent();
	editBtn->cleanup();

	auto btn2 = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	auto myButton2 = CCMenuItemSpriteExtra::create(
		btn2,
		nullptr,
		ptr,
		menu_selector(MenuLayer::onOptions));
	btn2->setScale(.7);
	myButton2->setPosition(CCLEFT - 255, editBtn->getPositionY() - 10);
	menu->addChild(myButton2, 1000);
	/*
			auto btn3 = CCSprite::createWithSpriteFrameName("garage_001.png");
			auto myButton3 = CCMenuItemSpriteExtra::create(
					btn3,
					nullptr,
					ptr,
					menu_selector(MenuLayer::onGarage)
			);
			btn3->setScale(.7);
			myButton3->setPosition(editBtn->getPositionX() - 100, editBtn->getPositionY() + 150);
			menu->addChild(myButton3, 1000);
			*/

	return ret;
};

void (*clippingRect)(GLint, GLint, GLsizei, GLsizei);
void clippingRect_hk(GLint x, GLint y, GLsizei width, GLsizei height)
{
	// LOGD( "SIZE: (%i, %i, %i, %i)", x, y, width, height );
	extern bool inEditor;
	if (inEditor)
	{
		clippingRect(x, y, 1000, 1000);
	}
	else
	{
		clippingRect(x, y, width, height);
	}
}

void *(*ui)(EditorUI *);
void *ui_hk(EditorUI *ptr)
{

	extern bool inEditor;
	inEditor = true;

	return ui(ptr);
}

// make GJUserScore store the acc swing
GJUserScore *(*GJUserScore_createO)(CCDictionary *);
GJUserScore *GJUserScore_createH(CCDictionary *userData)
{

	auto userScore = GJUserScore_createO(userData);

	int accSwing = userData->valueForKey("69")->intValue();
	reinterpret_cast<GJUserScoreExt *>(userScore)->playerSwing_ = accSwing;

	return userScore;
}

const char *(*CCString_getCStringO)(CCString *);
const char *CCString_getCStringH(CCString *self)
{
	auto ret = CCString_getCStringO(self);

	if (strstr(ret, "accSpider=") != NULL)
	{

		extern bool shouldSendDefaultValue;
		const char *toAdd;

		if (shouldSendDefaultValue)
		{
			toAdd = CCString::create("&accSwing=1000")->getCString();
		}
		else
		{
			toAdd = CCString::createWithFormat("&accSwing=%i", GM->m_nPlayerSwing)->getCString();
		}

		char *s = new char[strlen(ret) + strlen(toAdd) + 1];
		strcpy(s, ret);
		strcat(s, toAdd);

		ret = s;
	}

	if (strstr(ret, "diff=") != NULL)
	{

		auto glm = GameLevelManager::sharedState();
		auto toAdd = CCString::createWithFormat("&legendary=%i", glm->getBoolForKey("legendary_filter_custom"))->getCString();

		char *s = new char[strlen(ret) + strlen(toAdd) + 1];
		strcpy(s, ret);
		strcat(s, toAdd);

		ret = s;
	}

	if (strstr(ret, "&targetAccountID") != NULL || strstr(ret, "&page") != NULL)
	{

		auto toAdd = CCString::createWithFormat("&gdpsVersion=%i", version2)->getCString();

		char *s = new char[strlen(ret) + strlen(toAdd) + 1];
		strcpy(s, ret);
		strcat(s, toAdd);

		ret = s;
	}

	return ret;
}

bool (*creatorLayer)(CreatorLayer *);
bool creatorLayer_hk(CreatorLayer *ptr)
{
	if (!creatorLayer(ptr))
		return false;

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto menu = CCMenu::create();
	menu->setPosition(0, 0);
	ptr->addChild(menu);

	// creating buttons
	int iterator = 0;

	const char *spriteFrameName;
	SEL_MenuHandler buttonCallback;

	while (iterator < 15)
	{
		switch (iterator)
		{
		case 1:
			spriteFrameName = "GJ_safeBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onOnlyFullVersion);
			break;

		case 2:
			spriteFrameName = "GJ_listsBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onOnlyFullVersion);
			break;

		case 3:
			spriteFrameName = "GJ_mapPacksBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onMapPacks);
			break;

		case 4:
			spriteFrameName = "GJ_searchBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onOnlineLevels);
			break;

		case 5:
			spriteFrameName = "GJ_mapBtn_001.png";
			buttonCallback = menu_selector(ToolsLayer::onTheMap);
			break;

		case 6:
			spriteFrameName = "GJ_dailyBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onDailyLevel);
			break;

		case 7:
			spriteFrameName = "GJ_weeklyBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onWeeklyLevel);
			break;

		case 8:
			spriteFrameName = "GJ_eventBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onOnlyFullVersion);
			break;

		case 9:
			spriteFrameName = "GJ_gauntletsBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onGauntlets);
			break;

		case 10:
			spriteFrameName = "GJ_createBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onMyLevels);
			break;

		case 11:
			spriteFrameName = "GJ_savedBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onSavedLevels);
			break;

		case 12:
			spriteFrameName = "GJ_highscoreBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onLeaderboards);
			break;

		case 13:
			spriteFrameName = "GJ_challengeBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onChallenge);
			break;

		case 14:
			spriteFrameName = "GJ_versusBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onOnlyFullVersion);
			break;

		default:
			spriteFrameName = "GJ_featuredBtn_001.png";
			buttonCallback = menu_selector(CreatorLayer::onFeaturedLevels);
			break;
		};

		auto btnSpr = CCSprite::createWithSpriteFrameName(spriteFrameName);
		btnSpr->setScale(.85);
		auto btn = CCMenuItemSpriteExtra::create(btnSpr, btnSpr, ptr, buttonCallback);

		menu->addChild(btn);

		auto middleScreen = CCPoint(winSize.width / 2, winSize.height / 2);

		auto btnPos = CCPoint((((iterator % 5) - 1) * 100 - 100), ((floorf(iterator / 5) - 1) * 97));

		auto realBtnPos = middleScreen + btnPos; // didnt know this was possible

		btn->setPosition(realBtnPos);

		// quest btn
		if (iterator == 13)
		{
			ptr->_questBtnSpr() = btnSpr;
			ptr->checkQuestsStatus();
		}

		iterator++;
	}

	return true;
}

bool (*world)(WorldSelectLayer *, int);
bool world_hk(WorldSelectLayer *ptr, int a2)
{
	auto ret = world(ptr, a2);

	auto menu = CCMenu::create();
	auto dir = CCDirector::sharedDirector();

	auto back = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	auto backBtn = CCMenuItemSpriteExtra::create(back, back, ptr, menu_selector(WorldSelectLayerExt::onGo));
	menu->addChild(backBtn);
	menu->setPosition({dir->getScreenLeft() + 25, dir->getScreenTop() - 25});
	ptr->addChild(menu);

	return ret;
}

bool (*profile)(ProfilePage *, int, bool);
bool profile_hk(ProfilePage *self, int accountID, bool inMenu)
{

	// auto ret = bruh getting the original first doesnt work

	if (inMenu)
	{
		extern bool addBadges;
		extern bool userDataChanged;
		addBadges = true;
		if (userDataChanged)
		{
			GLM->updateUserScore();
			GLM->resetStoredUserInfo(accountID);
			userDataChanged = false;
		}
	}

	return profile(self, accountID, inMenu);
}

void (*profileRefresh)(ProfilePage *, CCObject *);
void profileRefresh_hk(ProfilePage *self, CCObject *a2)
{

	extern bool addBadges;
	addBadges = true;

	profileRefresh(self, a2);
}

#include "LevelCell.h"
void (*levelcell)(LevelCell *);
void levelcell_hk(LevelCell *self)
{

	GM->setIntGameVariable("52342", self->_level()->_epic());

	levelcell(self);
}

// no
void (*LevelEditorLayer_sortStickyGroupsO)(LevelEditorLayer *);
void LevelEditorLayer_sortStickyGroupsH(LevelEditorLayer *self)
{

	// this still looks weird
	if (!GM->getGameVariable("100006"))
		return LevelEditorLayer_sortStickyGroupsO(self);
}

bool (*lvlbrowser)(LevelBrowserLayer *, GJSearchObject *);
bool lvlbrowser_hk(LevelBrowserLayer *self, GJSearchObject *a2)
{

	extern bool legendaryChanged;
	if (legendaryChanged)
	{
		self->runAction(CCCallFuncO::create(self, callfuncO_selector(LevelBrowserLayer::onRefresh), self));
		legendaryChanged = false;
	}

	return lvlbrowser(self, a2);
}
#include "LevelSearchLayer.h"
void (*clearfilters)(LevelSearchLayer *);
void clearfilters_hk(LevelSearchLayer *self)
{

	GLM->setBoolForKey(0, "legendary_filter_custom");

	clearfilters(self);
}
/*
needs to be patched directly
void (*loginerror)(int, int);
void loginerror_hk(int a1, int a2) {

	patch *tmp = new patch();
	tmp->addPatch("libcocos2dcpp.so", 0x7B8839, "20 20 20 20");
	tmp->addPatch("libcocos2dcpp.so", 0x7B883D, "20 20");
	tmp->addPatch("libcocos2dcpp.so", 0x7B883F, "20 20 20 20 20 20");
	tmp->Modify();


	loginerror(a1, a2);

}
*/

// gotta overwrite the gdkit code cuz it was bad
// not saying like this is any better but it fixes touch triggers
bool (*UILayer_ccTouchBeganO)(UILayer *, CCTouch *, CCEvent *);
bool UILayer_ccTouchBeganH(UILayer *self, CCTouch *touch, CCEvent *event)
{
	auto ret = UILayer_ccTouchBeganO(self, touch, event);

	PlayLayer *layer = GameManager::sharedState()->_playLayer();
	if (layer->_player1()->_platformer())
	{
		auto pos = touch->getLocation();
		if(!GM->getGameVariable("0113"))  {
			if (pos.x > 250.0 || pos.y > 110.0)
			layer->pushButton(1, 0);
			return ret;
		}
		else 
		{
			auto pos2 = touch->getLocationInView();
			if (pos2.x < (CCRIGHT - 250) || pos.y > 110.0)
			layer->pushButton(1, 0);
			return ret;
		}
	}

	return ret;
}

bool (*UILayer_ccTouchEndedO)(UILayer *, CCTouch *, CCEvent *);
bool UILayer_ccTouchEndedH(UILayer *self, CCTouch *touch, CCEvent *event)
{
	auto ret = UILayer_ccTouchEndedO(self, touch, event);

	PlayLayer *layer = GameManager::sharedState()->_playLayer();
	if (layer->_player1()->_platformer())
	{
		layer->releaseButton(1, 0);
	}

	return ret;
}
#include "CreateParticlePopup.h"
// fix the particle popup
bool (*CreateParticlePopup_initO)(CreateParticlePopup *, ParticleGameObject *, CCArray *, std::string);
bool CreateParticlePopup_initH(CreateParticlePopup *self, ParticleGameObject *object, CCArray *idk, std::string data)
{
	inParticle = true;
	auto ret = CreateParticlePopup_initO(self, object, idk, data);

	auto nodes = self->getPageInputNodes(0);
	auto sliders = self->getPageSliders(0);

	int moveY = 96.5;

	// moving text input and sliders
	for (int i = 50; i < 56; i++)
	{
		auto node = (CCNode *)nodes->objectForKey(i);
		node->setPositionY(node->getPositionY() + moveY);

		auto slider = (CCNode *)sliders->objectForKey(i);
		slider->setPositionY(slider->getPositionY() + moveY);
	}

	// moving input bg and text
	for (int i = 0; i < self->_textAndStuff()->count(); i++)
	{
		auto thang = (CCNode *)self->_textAndStuff()->objectAtIndex(i);
		thang->setPositionY(thang->getPositionY() + moveY);
	}

	return ret;
}
#include "MapSelectLayer.h"
bool (*MapSelectLayerO)(MapSelectLayer *, int);
bool MapSelectLayer_hk(MapSelectLayer *self, int a2)
{

	auto ret = MapSelectLayerO(self, a2);
	auto dir = CCDirector::sharedDirector();
	auto old_menu = CCMenu::create();
	auto oldSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	auto old_btn = CCMenuItemSpriteExtra::create(
		oldSprite,
		oldSprite,
		self,
		static_cast<cocos2d::SEL_MenuHandler>(&ToolsLayer::onAlert));
	old_menu->addChild(old_btn, 900);
	old_btn->setPositionX(dir->getScreenRight() - 25);
	old_btn->setPositionY(dir->getScreenTop() - 25);
	old_menu->setPosition({0, 0});
	self->addChild(old_menu, 900);

	return ret;
}

void (*MapOnBack)(CCObject *);
void MapOnBack_hk(CCObject *a1)
{

	auto dir = CCDirector::sharedDirector();
	dir->pushScene(cocos2d::CCTransitionFade::create(0.5, CreatorLayer::scene()));
}

void (*ParticleOnCloseO)(CCObject *);
void ParticleOnCloseH(CCObject *a1)
{
	inParticle = false;
	return ParticleOnCloseO(a1);
}

void (*restoreO)(CCObject *);
void restoreH(CCObject *a1)
{
	FLAlertLayer::create(nullptr, "GDPS", "GDPS Editor 2.2.1.3\nBeta 7", "OK", nullptr, 400, false, 300)->show();
}

bool (*infoButton)(string, string, float);
bool infoButton_hk(string title, string desc, float a3)
{

	auto ret = infoButton(title, desc, a3);

	const char *description = desc.c_str();
	const char *titl = title.c_str();

	// contains(x, y) = strstr(x , y) != NULL
	if (contains(description, "Pickup trigger"))
		return infoButton("Pickup Trigger", "Adds or subtracts a value from a counter.\nOverwrite count sets the value of the counter to that of the pickup trigger.", a3);

	if (contains(titl, "Follow Player command help"))
		return infoButton("Follow Player Y", "makes objects follow the Y position of the player\nOffset controls how much higher or lower the object should follow. \nDelay controls how much time passes until the object goes to the player Y position.\nMax Speed controls how fast the object should follow the players Y position\nSpeed multiplies the Max Speed.", a3);

	if (contains(titl, "Static"))
		return infoButton("Static Camera", "Locks the camera to a specific object.\nOnly X and Y will will lock the camera to only scroll horizontally and vertically respectfully.\nExit Static moves the camera back to the player. This does not require a group.", a3);

	if (contains(titl, "offset"))
		return infoButton("Camera Offset", "Offsets the camera by X or Y.\nThe movement will be based on the original camera position, not the current one. This means offsetting the camera twice won't double the offset", a3);

	if (contains(titl, "Rotate") && !(contains(description, "optim")) && !contains(titl, "Trigger")) // add color
		return infoButton("Camera Guide", "Shows the position of the camera if the player was to spawn on the exact position of the Guide Trigger.\n<cg>The green line shows the entire visible area.</c>\n<cy>The yellow lines show where objects start to fade in and out.</c>", a3);

	if (contains(titl, "Song"))
		return infoButton(title, "<cy>Sets the time of the song in ms.</c>\n<cr>This means 1000ms = 1 second</c>", a3);

	if (contains(description, "ooooo") && contains(description, "uuuuu"))
		return infoButton("Timewarp", "Changes the speed of the game like a speedhack", a3);

	if (contains(description, "function as a") && !(contains(description, "group")))
		return infoButton("Instant Count", "test", a3);

	return ret;
}

bool (*UILayerInitO)(UILayer *);
bool UILayerInitH(UILayer *self)
{
	auto ret = UILayerInitO(self);
			
	
		timerLabel = CCLabelBMFont::create("0", "bigFont.fnt");
		timerLabel->setScale(.5);
		timerLabel->setPosition(CCLEFT + 60, CCTOP - 10);
		self->addChild(timerLabel);
		if(GM->getGameVariable("100008"))
		self->schedule(schedule_selector(PlayLayerExt::onUpdateTimer), 0.01);
		else
		timerLabel->setVisible(false);
	

	return ret;
}

void (*destroyPlayerO)(PlayerObject *, int);
void destroyPlayerH(PlayerObject *self, int a2)
{
	destroyPlayerO(self, a2);
			if(GM->getGameVariable("100008")) {

	PlayLayer *pl = GameManager::sharedState()->_playLayer();

	// i have no idea how this works but it looks cool as fuck
	UILayer *layer = reinterpret_cast<UILayer *>(*((int *)pl + 409));

	layer->unschedule(schedule_selector(PlayLayerExt::onUpdateTimer));
			}
}

bool (*LevelSearchLayerO)(LevelSearchLayer*);
bool LevelSearchLayerH(LevelSearchLayer* self) {
	
	auto ret = LevelSearchLayerO(self);
		auto menu = CCMenu::create();
		menu->setPosition(0,0);
		
	auto dir = CCDirector::sharedDirector();
	auto old_menu = CCMenu::create();
	auto oldSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
	oldSprite->setScale(.8);
	auto old_btn = CCMenuItemSpriteExtra::create(
		oldSprite,
		oldSprite,
		self,
		static_cast<cocos2d::SEL_MenuHandler>(&LevelSearchLayer::onClearString)
	);
	old_menu->addChild(old_btn, 900);
	old_btn->setPositionX(CCMIDX + 200);
	old_btn->setPositionY(dir->getScreenTop() - 30);
	old_menu->setPosition({ 0, 0 });
	self->addChild(old_menu, 900);
	
		    auto node = reinterpret_cast<CCNode*>(self->getChildren()->objectAtIndex(4));
			node->setContentSize(CCSize(410, 40));
			node->setPositionX(node->getPositionX() + 20);
/*
	(testing) this sucks
	
		auto input = CCTextInputNode::create(10, 1000, "A", "B", 1000, "bigFont.fnt");
        input->setAllowedChars("0123456789");
        input->setMaxLabelWidth(10);
        input->setMaxLabelScale(10);
        input->setLabelPlaceholderScale(10);
        input->setPosition(CCLEFT + 50, CCMIDY);
		
		auto aaa = CCMenu::create();
		aaa->setPosition(0,0);
		aaa->addChild(input, 999);
		this->addChild(aaa, 9999);
		*/


	return ret;
}

extern void lib_entry();

void loader()
{

	auto cocos2d = dlopen(targetLibName != "" ? targetLibName : NULL, RTLD_LAZY);
	auto libShira = dlopen("libgdkit.so", RTLD_LAZY);

	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16LevelSearchLayer4initEv"), (void*)LevelSearchLayerH, (void**)&LevelSearchLayerO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN9PlayLayer6resumeEv"), getPointer(&PlayLayerExt::resume_hk), (void **) &PlayLayerExt::resume_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN9PlayLayer18togglePracticeModeEb"), getPointer(&PlayLayerExt::togglePracticeMode_hk), (void **) &PlayLayerExt::togglePracticeMode_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN9PlayLayer13levelCompleteEv"), getPointer(&PlayLayerExt::levelCompleted_hk), (void **) &PlayLayerExt::levelCompleted_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN19CreateParticlePopup7onCloseEPN7cocos2d8CCObjectE"), (void *)ParticleOnCloseH, (void **)&ParticleOnCloseO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN7UILayer4initEv"), (void *)UILayerInitH, (void **)&UILayerInitO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16ColorSelectPopup15onToggleHSVModeEPN7cocos2d8CCObjectE"), (void *)ToggleCopyColorH, (void **)&ToggleCopyColorO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN15InfoAlertButton6createESsSsf"), (void *)infoButton_hk, (void **)&infoButton);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN26SetupCollisionTriggerPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayE"), (void *)getPointer(&SetupCollisionTriggerPopupH), (void **)&SetupCollisionTriggerPopupO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN21SetupZoomTriggerPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayE"), (void *)getPointer(&SetupZoomTriggerPopupH), (void **)&SetupZoomTriggerPopupO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16ColorSelectPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayEP11ColorAction"), (void *)getPointer(&ColorSelectPopupH), (void **)&ColorSelectPopupO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN7UILayer12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE"), (void *)UILayer_ccTouchBeganH, (void **)&UILayer_ccTouchBeganO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN7UILayer12ccTouchEndedEPN7cocos2d7CCTouchEPNS0_7CCEventE"), (void *)UILayer_ccTouchEndedH, (void **)&UILayer_ccTouchEndedO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN14MapSelectLayer6onBackEPN7cocos2d8CCObjectE"), (void *)MapOnBack_hk, (void **)&MapOnBack);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN14MapSelectLayer4initEi"), (void *)MapSelectLayer_hk, (void **)&MapSelectLayerO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN19CreateParticlePopup4initEP18ParticleGameObjectPN7cocos2d7CCArrayESs"), (void *)CreateParticlePopup_initH, (void **)&CreateParticlePopup_initO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11ProfilePage7onCloseEPN7cocos2d8CCObjectE"), (void *)profileRefresh_hk, (void **)&profileRefresh);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16LevelEditorLayer16sortStickyGroupsEv"), (void *)LevelEditorLayer_sortStickyGroupsH, (void **)&LevelEditorLayer_sortStickyGroupsO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16LevelSearchLayer12clearFiltersEv"), (void *)clearfilters_hk, (void **)&clearfilters);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZNK7cocos2d8CCString10getCStringEv"), (void *)CCString_getCStringH, (void **)&CCString_getCStringO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11ProfilePage4initEib"), (void *)profile_hk, (void **)&profile);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN9LevelCell19loadCustomLevelCellEv"), (void *)levelcell_hk, (void **)&levelcell);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN17LevelBrowserLayer4initEP14GJSearchObject"), (void *)lvlbrowser_hk, (void **)&lvlbrowser);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "glScissor"), (void *)clippingRect_hk, (void **)&clippingRect);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16WorldSelectLayer4initEi"), (void *)world_hk, (void **)&world);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN12CreatorLayer4initEv"), (void *)creatorLayer_hk, (void **)&creatorLayer);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11GJUserScore6createEPN7cocos2d12CCDictionaryE"), (void *)GJUserScore_createH, (void **)&GJUserScore_createO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN18LevelSettingsLayer4initEP19LevelSettingsObjectP16LevelEditorLayer"), (void *)levelsettings_hk, (void **)&levelsettings);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN18LevelSettingsLayer7onCloseEPN7cocos2d8CCObjectE"), (void *)lvlsettings_close_hk, (void **)&lvlsettings_close);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN14LevelInfoLayer4initEP11GJGameLevelb"), (void *)levelinfoinit_hk, (void **)&levelinfoinit);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN7cocos2d18CCSpriteFrameCache17spriteFrameByNameEPKc"), (void *)sprite_hk, (void **)&old5);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN14EditLevelLayer4initEP11GJGameLevel"), (void *)setUpLevelInfo_hk, (void **)&setUpLevelInfo);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN15SetGroupIDLayer4initEP10GameObjectPN7cocos2d7CCArrayE"), (void *)getPointer(&SetGroupIDLayer_initH), (void **)&SetGroupIDLayer_initO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN22SetupCameraRotatePopup4initEP16EffectGameObjectPN7cocos2d7CCArrayE"), (void *)getPointer(&SetupCameraRotatePopupH), (void **)&SetupCameraRotatePopupO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN14SelectArtLayer4initE13SelectArtType"), (void *)getPointer(&SelectArtLayer_initH), (void **)&SelectArtLayer_initO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11GameManager11colorForIdxEi"), (void *)getPointer(&GameManager_colorForIdx_hook), (void **)&GameManager_colorForIdx_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN13GJGarageLayer4initEv"), (void *)getPointer(&GJGarageLayerExt::init_hk), (void **)&GJGarageLayerExt::init_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN20AccountRegisterLayer4initEv"), (void *)getPointer(&AccountRegisterLayerExt::init_hk), (void **)&AccountRegisterLayerExt::init_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN12AccountLayer19syncAccountFinishedEv"), (void *)getPointer(&AccountLayerExt::accload_hk), (void **)&AccountLayerExt::accload);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN7cocos2d11CCFileUtils13addSearchPathEPKc"), (void *)CCFileUtils_addSearchPath_hk, (void **)&CCFileUtils_addSearchPath_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11GameManager10dataLoadedEP13DS_Dictionary"), (void *)&GameManager_dataLoaded_hk, (void **)&dataLoaded_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN12LoadingLayer15loadingFinishedEv"), (void *)loading_hook, (void **)&loading_trp);
	HookManager::do_hook((void *)&dict_hk, (void *)dict_hk1, (void **)&dict1);
	HookManager::do_hook((void *)&menu_hk, getPointer(&MenuLayerExt::init_hk), (void **)&MenuLayerExt::init_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN9MenuLayer23updateUserProfileButtonEv"), (void *)MenuLayer_updateUserProfileButtonH, (void **)&MenuLayer_updateUserProfileButtonO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16MultiplayerLayer4initEv"), (void *)getPointer(&MultiplayerLayerExt::init_hk), (void **)&MultiplayerLayerExt::init_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN15MoreSearchLayer4initEv"), (void *)getPointer(&MoreSearchLayerExt::init_hk), (void **)&MoreSearchLayerExt::init_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN10PauseLayer11customSetupEv"), (void *)getPointer(&PauseLayerExt::init_hk), (void **)&PauseLayerExt::init_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN8EditorUIC2Ev"), (void *)ui_hk, (void **)&ui);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN9PlayLayer10resetLevelEv"), getPointer(&PlayLayerExt::resetLevel_hk), (void **)&PlayLayerExt::resetLevel_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN12PlayerObject15playerDestroyedEb"), (void *)destroyPlayerH, (void **)&destroyPlayerO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN9PlayLayer6updateEf"), getPointer(&PlayLayerExt::update_hk), (void **)&PlayLayerExt::update_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16LevelEditorLayer10onPlaytestEv"), getPointer(&onPlaytestExt::playtest_hk), (void **)&onPlaytestExt::playtest);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11AppDelegate11trySaveGameEb"), (void *)save_hook, (void **)&save_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN10GameObject13createWithKeyEi"), (void *)create_hk, (void **)&old4);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN12OptionsLayer11customSetupEv"), (void *)OptionsLayer_customSetup_hk, (void **)&ol_customSetup_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16MoreOptionsLayer9addToggleEPKcS1_S1_"), getPointer(&MoreOptionsLayerExt::addToggle_hk), (void **)&MoreOptionsLayerExt::addToggle_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11GameManager18toggleGameVariableEPKc"), (void *)hook_onToggle, (void **)&onToggleTrampoline);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN12PlayerObject13releaseButtonE12PlayerButton"), (void *)release_hk, (void **)&release_trp, true);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11ProfilePage20loadPageFromUserInfoEP11GJUserScore"), (void *)ProfilePage_loadPageFromUserInfoH, (void **)&ProfilePage_loadPageFromUserInfoO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11CommentCell15loadFromCommentEP9GJComment"), (void *)CommentCell_loadFromCommentH, (void **)&CommentCell_loadFromCommentO);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN12SupportLayer9onRestoreEPN7cocos2d8CCObjectE"), (void *)restoreH, (void **)&restoreO);

	patch *tmp = new patch();

	// EC34
	// patch *shiraPTH = new patch();
	// shiraPTH->addPatch("libgdkit.so", 0xEC34, "F0 B5 03 AF 2D E9");

	// shiraPTH->Modify();
	// HookManager::do_hook(getPointer(&gdmk::get_proc_addr), (void*) t, (void **) &t_trp);
	// lib_entry();
	// LOGD("TTTTT!");

	// 2E9914
	tmp->addPatch("libcocos2dcpp.so", 0x2E9914, "6f f0 01 00 70 47");
	tmp->addPatch("libcocos2dcpp.so", 0x2ECC64, "00 bf");
	// tmp->addPatch("libcocos2dcpp.so", 0x2B1F52, "00 bf 00 bf 00 bf 00 bf");
	// tmp->addPatch("libcocos2dcpp.so", 0x2B1F92, "00 bf");

	// overflow crash spark
	//(you dont know how good it feels to have this commented)
	// tmp->addPatch("libcocos2dcpp.so", 0x44D2AE, "00 bf 00 bf");

	// SetupCameraRotatePopup degrees input bg
	tmp->addPatch("libcocos2dcpp.so", 0x3FAA5E, "00 21");

	// patch refresh
	tmp->addPatch("libcocos2dcpp.so", 0x2388EE, "00 20");
	tmp->addPatch("libcocos2dcpp.so", 0x2388DC, "00 20");

	// playtest try
	tmp->addPatch("libcocos2dcpp.so", 0x288A3E, "00 bf");
	tmp->addPatch("libcocos2dcpp.so", 0x28EB70, "00 bf 00 bf");

	// backgrounds
	tmp->addPatch("libcocos2dcpp.so", 0x28CE5C, "19 23");

	// grounds
	tmp->addPatch("libcocos2dcpp.so", 0x28CE72, "12 23");

	// full -> 2.2
	tmp->addPatch("libcocos2dcpp.so", 0x7A6024, "32 2e 32 20");

	// fix level too short
	tmp->addPatch("libcocos2dcpp.so", 0x241ADA, "00 bf");
	tmp->addPatch("libcocos2dcpp.so", 0x241B04, "00 bf 00 bf");
	tmp->addPatch("libcocos2dcpp.so", 0x241B14, "00 bf 00 bf");

	// 125742

	// 10 stars limit bypass
	tmp->addPatch("libcocos2dcpp.so", 0x2F8E5A, "04 e0");

	// creator layer original buttons
	tmp->addPatch("libcocos2dcpp.so", 0x2B1F92, "00 bf");

	// world layer arrow
	tmp->addPatch("libcocos2dcpp.so", 0x3D2D8E, "00 bf");

	// patch for the swing limit
	tmp->addPatch("libcocos2dcpp.so", 0x2EACB8, "27");

	// probably fix platformer kick
	tmp->addPatch("libcocos2dcpp.so", 0x247EC4, "00 BF 00 BF");
	tmp->addPatch("libcocos2dcpp.so", 0x24168E, "00 BF 00 BF");

	// package for save release
	// tmp->addPatch("libcocos2dcpp.so", 0x785011, "67 65 6f 6d 65 74 72 79 6a 75 6d 70 6c 69 74 65");

	// package for save beta
	tmp->addPatch("libcocos2dcpp.so", 0x785011, "67 65 6f 6d 65 74 72 79 6a 75 6d 70 6c 69 74 61");

	tmp->addPatch("libgdkit.so", 0xDC02, "00 bf");
	tmp->addPatch("libgdkit.so", 0xD284, "00 bf");

	// fps, levl info nop, applied patch to lib directly
	tmp->addPatch("libgdkit.so", 0x00E2C8, "30 30 30 30");
	tmp->addPatch("libgdkit.so", 0x00E2D0, "30 30 30 30");

	//  remove pause change speed	//
	tmp->addPatch("libgdkit.so", 0xE5DA, "00 bf");
	tmp->addPatch("libgdkit.so", 0xE618, "00 bf");
	tmp->addPatch("libgdkit.so", 0xE65C, "00 bf");

	// tmp->addPatch("libcocos2dcpp.so",002EA824,"00 bf");

	tmp->addPatch("libgdkit.so", 0xE7EE, "00 bf");

	// patching the hooks here doesnt work, so these are already applied into the lib itself
	// tmp->addPatch("libgdkit.so", 0xF6CC, "00 bf 00 bf"); //clippingRect_hk
	// tmp->addPatch("libgdkit.so", 0xF0C2, "00 bf 00 bf"); //sprite_hk
	// tmp->addPatch("libgdkit.so", 0xF016, "00 bf 00 bf"); //create_hk
	// tmp->addPatch("libgdkit.so", 0xF15E, "00 bf 00 bf"); //pause_hk
	// tmp->addPatch("libgdkit.so", 0xF1FE, "00 bf 00 bf"); //levelinfoinit_hk

	// add 1 more icon to ProfilePage
	tmp->addPatch("libcocos2dcpp.so", 0x397C6C, "BB F1 08 0F");

	// menu

	tmp->addPatch("libcocos2dcpp.so", 0x237A6C, "00 bf");
	tmp->addPatch("libcocos2dcpp.so", 0x237904, "00 bf");

	// setup zoom stuff
	tmp->addPatch("libcocos2dcpp.so", 0x3DAE5E, "00 BF");

	// color trigger stuff
	tmp->addPatch("libcocos2dcpp.so", 0x2D9322, "00 BF");
	tmp->addPatch("libcocos2dcpp.so", 0x2D98F8, "00 BF");

	// collision trigger info button
	tmp->addPatch("libcocos2dcpp.so", 0x3C35E6, "00 bf");
	// rotate trigger info button
	tmp->addPatch("libcocos2dcpp.so", 0x3FA2E0, "00 bf");

	// cube
	tmp->addPatch("libcocos2dcpp.so", 0x2EB1B8, "FC 29");
	tmp->addPatch("libcocos2dcpp.so", 0x2EBC4A, "4F F0 FC 09");
	tmp->addPatch("libcocos2dcpp.so", 0x2EACC4, "FC 27");
	tmp->addPatch("libcocos2dcpp.so", 0x26B53E, "FC 29");
	tmp->addPatch("libcocos2dcpp.so", 0x2653E4, "FC 29");

	// ship
	tmp->addPatch("libcocos2dcpp.so", 0x2EBC6C, "4F F0 97 09");
	tmp->addPatch("libcocos2dcpp.so", 0x2EAC76, "97 27");
	tmp->addPatch("libcocos2dcpp.so", 0x26B552, "97 2A");
	tmp->addPatch("libcocos2dcpp.so", 0x2658E8, "97 29");

	// ball
	tmp->addPatch("libcocos2dcpp.so", 0x2EBC8E, "4F F0 8F 09");
	tmp->addPatch("libcocos2dcpp.so", 0x2EAC82, "8F 27");
	tmp->addPatch("libcocos2dcpp.so", 0x265504, "8F 29");

	// ufo and wave use the same value ☠☠☠
	tmp->addPatch("libcocos2dcpp.so", 0x2EBCD2, "4F F0 73 09");
	tmp->addPatch("libcocos2dcpp.so", 0x2EAC8E, "73 27");
	tmp->addPatch("libcocos2dcpp.so", 0x265B20, "73 29");
	tmp->addPatch("libcocos2dcpp.so", 0x265744, "73 29");

	// robot
	tmp->addPatch("libcocos2dcpp.so", 0x2EBCF0, "4F F0 7E 09");
	tmp->addPatch("libcocos2dcpp.so", 0x2EACA0, "7E 27");
	tmp->addPatch("libcocos2dcpp.so", 0x265330, "7E 29");
	tmp->addPatch("libcocos2dcpp.so", 0x387CA6, "7E 29");
	// tmp->addPatch("libcocos2dcpp.so", 0x387C96, "7E 22");

	// FUCKNIG SPIDERS HOLY FUCK WHY DOESNT IT WORK
	tmp->addPatch("libcocos2dcpp.so", 0x2EBD78, "4F F0 74 09");
	tmp->addPatch("libcocos2dcpp.so", 0x2EACAC, "74 27");
	tmp->addPatch("libcocos2dcpp.so", 0x265346, "74 29");
	tmp->addPatch("libcocos2dcpp.so", 0x387F0A, "74 29");

	tmp->Modify();

	pauseBtn->addPatch("libcocos2dcpp.so", 0x7896A9, "32");

	// removing the toggles from SetGroupIDLayer
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E1D90, "00 bf 00 bf"); // Dont Fade
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E1E34, "00 bf 00 bf"); // Dont Enter
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E1F3E, "00 bf 00 bf"); // No Effects
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E20EC, "00 bf 00 bf"); // Group Parent
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E216E, "00 bf 00 bf"); // High Detail
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E21EC, "00 bf 00 bf"); // Untouchable
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E226A, "00 bf 00 bf"); // Passable
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E22EC, "00 bf 00 bf"); // Hide
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E2374, "00 bf 00 bf"); // NonStick
	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E2458, "00 bf 00 bf"); // IceBlock

	groupIDLayerPatches->addPatch("libcocos2dcpp.so", 0x3E2494, "00 21"); // Order input bg

	groupIDLayerPatches->Modify();

	// moving stuff in SetupCameraRotatePopup
	auto cameraRotatePopupPatches = new patch();
	cameraRotatePopupPatches->addPatch("libcocos2dcpp.so", 0x3FA672, "27 EE 88 9A"); // VNMLS.F32  S18, S15, S16 -> VMUL.F32 S18, S15, S16
	cameraRotatePopupPatches->addPatch("libcocos2dcpp.so", 0x3FA348, "27 EE 88 7A"); //	VNMLS.F32  S14, S15, S16 -> VMUL.F32 S14, S15, S16

	cameraRotatePopupPatches->Modify();

	tmp->Modify();
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	// init_handle();
	pthread_t t;
	pthread_create(&t, NULL,
				   reinterpret_cast<void *(*)(void *)>(loader), NULL);
	return JNI_VERSION_1_6;
}

//(?=.*playlayer)(?=.*init)

/*





// have to test this more
bool (*onDelete)(LevelInfoLayer*, CCObject*, int, int, int, int);
bool onDelete_hk(LevelInfoLayer* self, GJSearchObject* a2, int a3, int a4, int a5, int a6) {

	auto ret = onDelete(self, a2, a3, a4, a5, a6);
	self->playMenu_->onExit();

	return ret;
}

bool (*rate)(RateStarsLayer*, int, bool);
bool rate_hk(RateStarsLayer* self, int a2, bool a3) {

	auto ret = rate(self, a2, a3);

	//https://media.discordapp.net/attachments/847950548921614366/942101116571746314/unknown.png


	return ret;
}

*/
