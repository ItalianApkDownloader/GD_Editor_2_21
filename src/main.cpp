#include <jni.h>
#include <dlfcn.h>
#include <cstdio>
#include <pthread.h>
#include <Logger.h>
#include "cocos2d.h"
#include "gd.h"
#include "hooking.h"
#include "obfuscate.h"
#include <bits/stdc++.h>
#include "GDPSManager.h"
#include "GDPSHelper.h"
#include "FunctionHelper.h"
#include "../include/FunctionHelper.h"
#include "CCDrawNode.h"
#include <unwind.h>
#include <sys/stat.h>

#include "hooks/MenuLayerExt.h"
#include "hooks/EditLevelLayerExt.h"
#include "hooks/LevelEditorLayerExt.h"
#include "hooks/EditorPauseLayerExt.h"
#include "hooks/ShaderDebug.h"
#include "hooks/DPADControl.h"
#include "hooks/CollisionFix.h"
#include "hooks/ShaderFix.h"
#include "hooks/SpeedrunTimer.h"
#include "hooks/AdvancedLevelInfo.h"
#include "hooks/MoreSearchLayerExt.h"
#include "hooks/SwingIconFix.h"
#include "hooks/Options.h"

/*
		FLAG USED FOR DEVELOPER MODE DEBUGGING LIKE SHADERS

		!!!!!!!!!!!!!!!!!!!!!!!! COMMENT OUT BEFORE RELEASING APK !!!!!!!!!!!!!!!!!!!!!!!!
*/

__attribute__((naked)) void asmtest() {
	asm("nop;"
		"mov R1, #4;"
		"add R1, #4;"
	);
}


std::string passwordTemp = "";
int nFont = 0;
bool first = true;
void(*GameManager_tryShowAdO)();
void GameManager_tryShowAdH() {}

float randFloat(float X) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/X)); }

void UpdatePasswordTemp() {
	const auto m_sFileName = "password.dat";

	const auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_sFileName;

	std::ifstream infile(path.c_str());
	if (infile.good())
	{
		string myText;
		while (getline(infile, myText))
			passwordTemp = myText;
		//	CCLog("password done");
	}
	else
	{
		passwordTemp = "0";
		//	CCLog("no file found");
	}
}

FUNCTIONHOOK(bool, UILayer_init, UILayer* self) {
	if(!UILayer_initO(self)) return false;
	
	#ifdef SHADERDEBUG
	reinterpret_cast<UILayerDebug*>(self)->doInit();
	#endif
	auto menu = (CCMenu*)self->getChildren()->objectAtIndex(0);
	auto pauseBtn = (CCMenuItemSpriteExtra*)self->getChildren()->objectAtIndex(0);
	pauseBtn->setVisible(!GM->getGameVariable("1000012"));
	//GDPSHelper::createLabels(menu, menu->getChildren(), {0, 0}, true);
	
	if(!MEMBERBYOFFSET(bool, self, 0x206)) return true;
	
	DPADHooks::UILayerInit(self);
	
	return true;
}   
   
bool(*isIconUnlockedO)(void *, int, int);
bool isIconUnlockedH(void *self, int a1, int a2)
{ return true; }

CCSprite* (*spriteCreateO)(const char *textureName);
CCSprite* spriteCreateH(const char *textureName)
{
	if (containss(textureName, "GJ_square05.png"))
	return spriteCreateO("GJ_square04.png");

	auto ret = spriteCreateO(textureName);
	if (ret != nullptr)
		return ret;

	return spriteCreateO("pixel.png");
}



FUNCTIONHOOK(void, MoreOptionsLayer_onToggle, MoreOptionsLayer* self, CCMenuItemToggler* sender) {
	
	bool on = !sender->_isToggled();
	int gamevariable = sender->getTag();
	
	//normal gdps settings
	if(gamevariable > 1000000 && 200000 > gamevariable) {
		switch(gamevariable)
		{
			case 100005: 
				if(on)
				FLAlertLayer::create(nullptr, "DISCLAIMER", "<cg>Pixel blocks</c> are <cr>not official</c> and for that reason levels containing these blocks <cr>will not look good in the official 2.2.</c>\n    <co>(textures will change)\n</c><cr>Don't use pixel blocks if you want your level to be playable in 2.2.</c>", "OK", nullptr, 500, false, 300)->show();
		}
	}
	
	if(gamevariable > 200000) {
		
		patch p;
		
		switch(gamevariable) 
		{		
			case 200001: // Safe NoClip
				p.addPatch(0x00276df8, on ? "7047" : "2DE9"); // PlayLayer::destroyPlayer
			//	p.addPatch(0x00276934, on ? "CFE6" : "2DE9"); // PlayLayer::levelComplete
			break;
			/*
			case 200002: // Safe Mode
				p.addPatch(0x00276934, on ? "CFE6" : "2DE9"); // PlayLayer::levelComplete
			break;
			

            case 200003: // No Death Effect (I hope this doesnt effect any codes here since i make the PlayerObject::playerDestroyed to (null)
				p.addPatch(0x0029c23c, on ? "7047" : "90F8"); // PlayerObject::playerDestroyed
			break;
			*/
		}
		p.Modify();
	}
	
	MoreOptionsLayer_onToggleO(self, sender);
	
}


bool isHackActive() {
	return 
	GM->ggv("200001") || GM->ggv("200002") || GM->ggv("200003");
}

class temp {
	public:
void showHackAlert(CCObject* sender) {
	FLAlertLayer::create(nullptr, "DISCLAIMER", "aaaaaaaaaa", "OK", nullptr, 500, false, 300)->show();
}
};
FUNCTIONHOOK(void, PlayLayer_levelCompleted, PlayLayer* self) {
	
	if(!isHackActive())
		PlayLayer_levelCompletedO(self);
	else {
		self->onQuit();
	}
}

bool doRequest;

const char *(*getStringO)(LoadingLayer *self);
const char *getStringH(LoadingLayer *self)
{

	doRequest = true;
	GM->setGameVariable("0122", false);
	GM->setGameVariable("0023", false); //smooth fix
	GM->setGameVariable("0074", true);  //show restart button
	return "Italian APK Downloader\nCatto_\niAndy_HD3\nTr1NgleBoss\nEitan";
}

CCSprite * (*spriteCreateFrameNameO)(const char *textureName);
CCSprite* spriteCreateFrameNameH(const char *textureName)
{
	if (containss(textureName, "GJ_fullBtn_001.png"))
		return spriteCreateFrameNameO("GJ_creatorBtn_001.png");

	auto ret = spriteCreateFrameNameO(textureName);
	if (ret != nullptr)
		return ret;

	//return spriteCreateFrameNameO("GJ_optionsTxt_001.png");
	return spriteCreateFrameNameO("GJ_checkOff_001.png");
}

#include "cocos2dx/extensions/network/HttpClient.h"
#include "cocos2dx/extensions/network/HttpRequest.h"
#include "cocos2dx/extensions/network/HttpResponse.h"
#include "obfuscate.h"

const char *gdpseditor = AY_OBFUSCATE("http://game.gdpseditor.com/server");
const char *boomlings = AY_OBFUSCATE("http://www.boomlings.com/database");

//epic servers obfuscate moment

inline string replaceServers(std::string original)
{
	if(original.find("gdpseditor.com") != std::string::npos)
		return original;
	
	int c = strlen(gdpseditor);
	for (int i = 0; i < c; i++)
		original[i] = gdpseditor[i];

	return original;
}

void *(*LevelProcessO)(GameManager *gm, string a1, string a2, string a3, int a4);
void *LevelProcessH(GameManager *gm, string a1, string postData, string a3, int a4)
{
	//fmtlog("postData: {}", postData);
	if(postData.find("epic=") != std::string::npos) 
	{
		postData += fmt::format("&godlike={}", (int)GLM->getBoolForKey("legendary_filter_custom"));
		
		//the platformer filter is actually a length filter
		//however thats buggy if i add it like that so i will just use the old code
		//to add it in advanced filters
		if(GLM->getBoolForKey("platform_filter_custom"))
		{
			int lenpos = postData.find("len=");
			if(postData[lenpos + 4] != '-')
				postData.insert(postData.find("&page"), ",5");
			else
				postData[lenpos + 4] = '5';
		}
	}
	return LevelProcessO(gm, replaceServers(a1), postData, a3, a4);
}

void *(*MusicProcessO)(void *idk, string a1, string a2, string a3, int a4);
void *MusicProcessH(void *idk, string a1, string a2, string a3, int a4)
{
	return MusicProcessO(idk, replaceServers(a1), a2, a3, a4);
}

void *(*AccountProcessO)(void *idk, string a1, string a2, string a3, int a4);
void *AccountProcessH(void *idk, string a1, string postData, string a3, int a4)
{
	if (!passwordTemp.empty())
	{
		auto AM = GJAccountManager::sharedState();
		postData = fmt::format("{}&password={}&gjp={}", postData, passwordTemp, FunctionHelper::gjp(passwordTemp));
	}
	return AccountProcessO(idk, replaceServers(a1), postData, a3, a4);
}

bool(*canPlayOnlineLevelsO)(CreatorLayer *self);
bool canPlayOnlineLevelsH(CreatorLayer *self)
{
	return true;
}

CCArray * (*getTriggerGroupO)(LevelEditorLayer *self, int a1);
CCArray* getTriggerGroupH(LevelEditorLayer *self, int a1)
{
	CCArray *a = CCArray::create();
	return a;
}

void *(*addToGroupO)(GJBaseGameLayer *self, GameObject *a2, int a3, bool a4);
void *addToGroupH(GJBaseGameLayer *self, GameObject *a2, int a3, bool a4)
{
	*((bool*) a2 + 1157) = false;

	return addToGroupO(self, a2, a3, a4);
}
#include "AccountLoginLayer.h"

void *(*AccountSubmitO)(AccountLoginLayer *self, CCObject *a2, void *a3, void *a4);
void *AccountSubmitH(AccountLoginLayer *self, CCObject *a2, void *a3, void *a4)
{
	passwordTemp = self->_inputPassword()->getString();
	//CCLog(passwordTemp.c_str());
	
	return AccountSubmitO(self, a2, a3, a4);
}

void *(*LoginFinishedO)(AccountLoginLayer *self, void *a2, void *a3);
void *LoginFinishedH(AccountLoginLayer *self, void *a2, void *a3)
{
	const auto m_sFileName = "password.dat";

	const auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_sFileName;
	ofstream MyFile(path.c_str());

	MyFile << passwordTemp;

	MyFile.close();

	return LoginFinishedO(self, a2, a3);

}

bool(*LevelInfoLayerInitO)(LevelInfoLayer *, GJGameLevel *, bool);
bool LevelInfoLayerInitH(LevelInfoLayer *self, GJGameLevel *level, bool a3)
{
	if(!LevelInfoLayerInitO(self, level, a3)) return false;
	
	auto sprite = CCSprite::createWithSpriteFrameName("communityCreditsBtn_001.png");
	sprite->setScale(2);
	auto btn = CCMenuItemSpriteExtra::create(sprite, sprite, self, menu_selector(LevelInfoLayer::onClone));
	auto menu = CCMenu::create();
	menu->setPositionY(menu->getPositionY() - 50);
	menu->addChild(btn, 500);
	self->addChild(menu, 500);

	return true;

}

inline long mid_num(const std::string &s)
{
	return std::strtol(&s[s.find('_') + 1], nullptr, 10);
}

FUNCTIONHOOK(GameObject*, GameObject_create, int key)
{
	auto tb = ObjectToolbox::sharedState()->intKeyToFrame(key);
	
	if(key == 2013) return GameObject_createO(1);
	
	if(containss(tb, "pixelb"))
		return GameObject_createO(1);

	if(containss(tb, "pixel")) {
		
		if(containss(tb, "b_"))
			return GameObject_createO(1);

		auto pixelKey = mid_num(tb);
		return GameObject_createO(pixelKey > 140 ? 136 : key);
	}

	return GameObject_createO(key);
}

const char *(*keyToFrameO)(ObjectToolbox *self, int key);
const char *keyToFrameH(ObjectToolbox *self, int key)
{
	//	auto ret = keyToFrameO(self, key);
	//	CCLog("%d | | %s", key, ret);

	if (key >= 2925) 
	{
		switch (key)
		{
			case 1933:

				return "portal_18_front_001.png";
				break;

			case 2925:
				return "edit_eCamModeBtn_001.png";
				break;

			case 2999:
				return "edit_eSetupMGBtn_001.png";
				break;

			case 3016:
				return "edit_eAdvFollowBtn_001.png";
				break;

			case 3024:
				return "edit_eAreaStopBtn_001.png";
				break;

			case 3020:
				return "edit_eFadeEnterBtn_001.png";
				break;

			case 2066:
				return "edit_eGravityBtn_001.png";
				break;

			case 3017:
				return "edit_eMoveEnterBtn_001.png";
				break;

			case 3018:
				return "edit_eRotateEnterBtn_001.png";
				break;

			case 3019:
				return "edit_eScaleEnterBtn_001.png";
				break;

			case 3021:
				return "edit_eTintEnterBtn_001.png";
				break;

			case 3022:
				return "edit_eTeleportBtn_001.png";
				break;

			case 3023:
				return "edit_eStopEnterBtn_001.png";
				break;

			default:
				return keyToFrameO(self, key);
		}
	}
}

//7DA3E7
void *(*GameObject_customSetupO)(GameObject *obj);
void *GameObject_customSetupH(GameObject *obj){
	auto ret = GameObject_customSetupO(obj);


	if(obj->_objectID() == 2100){
		obj->_objectID() = 1598;
	}

	return ret;
}
void *(*PlayLayer_addObjectO)(PlayLayer *self, GameObject *obj);
void *PlayLayer_addObjectH(PlayLayer *self, GameObject *obj)
{
	int id = obj->_objectID();
	//	CCLog("Object ID: %d", id);

	if (id == 1331)
	{
		patch s = patch();
		string spider = "70 6F 72 74 61 6C 5F 31 37 5F 62 61 63 6B 5F 30 30 31 2E 70 6E 67";
		s.addPatch("libcocos2dcpp.so", 0x7E19BA, spider);
		s.Modify();
	}

	else if (id == 1933)
	{
		patch p = patch();
		string swing = "70 6F 72 74 61 6C 5F 31 38 5F 62 61 63 6B 5F 30 30 31 2E 70 6E 67";
		p.addPatch("libcocos2dcpp.so", 0x7E19BA, swing);
		p.Modify();
	}

	return PlayLayer_addObjectO(self, obj);

}


void showStackTrace() {
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

// moving funny loading text
bool(*LoadingLayer_initO)(LoadingLayer *, bool);
bool LoadingLayer_initH(LoadingLayer *self, bool fromReload)
{
	#ifdef EMUI_FIX
	
		patch tmp;
		//patch out the update progress function, which crashes because we didnt call init
		tmp.addPatch("libcocos2dcpp.so", 0x270594, "00BF00BF");
		tmp.Modify();
		
		self->loadAssets();
		
		auto spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();
		spriteCache->addSpriteFramesWithFile("GJ_LaunchSheet.plist");
		
	#else

		if (!LoadingLayer_initO(self, fromReload)) return false;
	
		auto text = *reinterpret_cast< CCNode **>(reinterpret_cast<uintptr_t> (self) + 0x144);
		text->setPositionY(text->getPositionY() - 10);

	
	#endif

	UpdatePasswordTemp();
	self->makeNewsRequest();
	return true;

}

void *(*EditorUI_SelectObjectsO)(EditorUI *self, CCArray *objects, bool a3);
void *EditorUI_SelectObjectsH(EditorUI *self, CCArray *objects, bool a3)
{
	
	int cl = MBO(int, self->_levelEditor(), 0x2C50);
	if (cl != -1)
	{
		int count = objects->count();
		auto toDelete = CCArray::create();

		for (int i = 0; i < count; i++)
		{
			//	CCLog("loop: %d", i);
			auto obj = (GameObject*) objects->objectAtIndex(i);
			int l1 = MBO(int, obj, 0x450);
			int l2 = MBO(int, obj, 0x454);
			//	CCLog("CL: %d | L1: %d | L2: %d", cl, l1, l2);
				
			bool shouldBeVisible = (cl == l1 || (cl == l2 && l2 != 0) || cl == -1);
			
			if (!shouldBeVisible)
			//if (cl != l1 && l2 == 0)
			{
				toDelete->addObject(obj);
				//	CCLog("object %d removed from array", i);
			}
		}

		objects->removeObjectsInArray(toDelete);
	}
	

	return EditorUI_SelectObjectsO(self, objects, a3);
}

void (*EditorUI_SelectObjectO)(EditorUI *self, GameObject *object, bool a3);
void EditorUI_SelectObjectH(EditorUI *self, GameObject *object, bool a3)
{
//	CCLog("enter");
	int cl = MBO(int, self->_levelEditor(), 0x2C50);

	int l1 = MBO(int, object, 0x450);
	int l2 = MBO(int, object, 0x454);
								
	bool shouldBeVisible = (cl == l1 || (cl == l2 && l2 != 0) || cl == -1);
//	CCLog("shouldBeVisible: %d", shouldBeVisible);
//	CCLog("CL: %d | L1: %d | L2: %d", cl, l1, l2);
	
	if(shouldBeVisible)
		EditorUI_SelectObjectO(self, object, a3);
}



bool(*EditorUI_InitO)(EditorUI *self, LevelEditorLayer *editor);
bool EditorUI_InitH(EditorUI *self, LevelEditorLayer *editor)
{
	if (!EditorUI_InitO(self, editor))
		return false;

	DPADHooks::EditorUIInit(self);

	if (!GM->getGameVariable("100005"))
	{
		auto menu = self->_objectMenu();
		auto btns = menu->getChildren();

		auto pixelblocksBtn = (CCMenuItemSpriteExtra*) btns->objectAtIndex(8);
		btns->removeObject(pixelblocksBtn, false);
		menu->alignItemsHorizontallyWithPadding(-2);
	}

	return true;
}
/*
int(*onToggleTrampoline)(void *pthis, const char *val);
void hook_onToggle(void *pthis, const char *val)
{
	int v = atoi(val);

	onToggleTrampoline(pthis, val);

	if (v > 100000)
	{
		if (v == 100005 && GM->getGameVariable("100005"))
		{
			FLAlertLayer::create(nullptr, "DISCLAIMER", "<cg>Pixel blocks</c> are <cr>not official</c> and for that reason levels containing these blocks <cr>will not look good in the official 2.2.</c>\n    <co>(textures will change)</c><cr>Don't use pixel blocks if you want your level to be playable in 2.2.</c>", "OK", nullptr, 500, false, 300)->show();
		}
	}
}
*/


bool(*SelectArtLayer_initO)(SelectArtLayer *, SelectArtType);
bool SelectArtLayer_initH(SelectArtLayer *self, SelectArtType type)
{
	if (!SelectArtLayer_initO(self, type))
		return false;
	
	if(type == smartTemplate)
		return true;
	

	auto menu = self->_bgSelectMenu();
	auto array = self->_someArray();
	
	//setting
	
	int toRemove;
	int maxTextures;
	
	if(type == background) {
		toRemove = 45;
		maxTextures = 24;
	}
	if(type == ground) {
		toRemove = 30;
		maxTextures = 17;
	}
	
	if(type == mg) {
		toRemove = 3;
		maxTextures = 3;
	}
	
	for(int i = 0; i < toRemove; i++) {
	
		auto node = (CCNode*)menu->getChildren()->objectAtIndex(0);
		auto node2 = (CCNode*)array->objectAtIndex(0);
		
		if(i <= maxTextures) {
		
		
			const char *frameSprStr;

				if (type == ground)
					frameSprStr = "gIcon_%02d_001.png";
				else if(type == background)
					frameSprStr = "bgIcon_%02d_001.png";
				else
					frameSprStr = "fgIcon_%02d_001.png";


				auto frameStr = CCString::createWithFormat(frameSprStr, i + 1)->getCString();
				
			
			auto spr = CCSprite::createWithSpriteFrameName(frameStr);
			auto btn = CCMenuItemSpriteExtra::create(spr, spr, self, menu_selector(SelectArtLayer::selectArt));
			if(type == background) {
				btn->setPosition(node->GPX(), node->GPY() - (20 *  (i / 10)));
				spr->setScale(.8);
			}
			else
			btn->setPosition(node->getPosition());
			
			btn->setTag(type != mg ? i + 1 : i);
			menu->addChild(btn);
			array->addObject(btn);
		}	
		
		
		menu->getChildren()->removeObjectAtIndex(0, false);
		array->removeObjectAtIndex(0, false);
	}
	
	self->selectArt(0);
	
	return true;
}
#include "AccountRegisterLayer.h"

bool(*AccountRegisterLayer_InitO)(AccountRegisterLayer*);
bool AccountRegisterLayer_InitH(AccountRegisterLayer*)
{
	cocos2d::CCApplication::sharedApplication()->openURL("http://game.gdpseditor.com/server/tools/account/registerAccount.php");
}

#include "SetupPickupTriggerPopup.h"

bool(*SetupPickupTriggerO)(SetupPickupTriggerPopup *, EffectGameObject *, cocos2d::CCArray *, float, float);
bool SetupPickupTriggerH(SetupPickupTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects, float a, float b)
{
	auto ret = SetupPickupTriggerO(self, object, objects, a, b);
	//pickup

	EffectGameObject *obj = MBO(EffectGameObject *, self, 0x1F8);
	bool overrideCount = MBO(bool, obj, 0x5C5);
	CCMenu *menu = self->_menu();

	auto multitrigger = (CCMenuItemToggler*) menu->getChildren()->objectAtIndex(9);

	GDPSHelper::createToggleButton("Override\nCount",
		{ 55, 0 },
		.7,
		0.5,
		self,
		menu_selector(SetupPickupTriggerPopup::onOverrideCount),
		menu, overrideCount, true);

	multitrigger->setPositionY(multitrigger->getPositionY() + 36);
	multitrigger->setPositionX(multitrigger->getPositionX() - 23);

	auto layer2 = (CCLayer*) self->getChildren()->objectAtIndex(0);
	auto label = (CCLabelBMFont*) layer2->getChildren()->objectAtIndex(9);

	label->setPositionY(label->getPositionY() + 36);
	label->setPositionX(label->getPositionX() - 23);


	return ret;

}

FUNCTIONHOOK(const char*, GameManager_getMGTexture, GameManager* self, int a2) {

	//avoid temp variables
	GM->loadMiddleground(2);
	return CCString::createWithFormat("fg_%02d.png", a2)->getCString();
}

void(*togglePracticeModeO)(PlayLayer *self, bool *on);
void togglePracticeModeH(PlayLayer *self, bool *on)
{
	MBO(bool, self, 0x2D81) = GM->getGameVariable("0125");

	togglePracticeModeO(self, on);
}

// open editor from pause
FUNCTIONHOOK(void, PauseLayer_onEdit, PauseLayer* self, CCObject*) {
	GM->_playLayer()->stopAllActions();
	GM->_playLayer()->unscheduleAllSelectors();
	GSM->stopBackgroundMusic();

	GM->_playLayer()->removeAllObjects();

	self->runAction(
		CCSequence::create(
			CCDelayTime::create(0),
			CCCallFunc::create(self, callfunc_selector(PauseLayer::goEditFix)),
			nullptr
		)
	);
}

void PauseLayer::goEditFix() {
	auto level = GM->_playLayer()->_level();

	auto layer = LevelEditorLayer::create(level, false);
	auto scene = CCScene::create();
	scene->addChild(layer);

	CCDirector::sharedDirector()->replaceScene(
		CCTransitionFade::create(.5, scene)
	);
}

// open editor from endlayer
FUNCTIONHOOK(void, EndLevelLayer_onEdit, EndLevelLayer* self, CCObject*) {
	GM->_playLayer()->stopAllActions();
	GM->_playLayer()->unscheduleAllSelectors();
	GSM->stopBackgroundMusic();

	GM->_playLayer()->removeAllObjects();

	self->runAction(
		CCSequence::create(
			CCDelayTime::create(0),
			CCCallFunc::create(self, callfunc_selector(EndLevelLayer::goEditFix)),
			nullptr
		)
	);
}

void EndLevelLayer::goEditFix() {
	this->exitLayer(nullptr);
	reinterpret_cast<PauseLayer*>(this)->goEditFix();
}



FUNCTIONHOOK(void, GJBaseGameLayer_addToSection, GJBaseGameLayer* self, GameObject* object) {
	if(object) GJBaseGameLayer_addToSectionO(self, object);
}

FUNCTIONHOOK(void, GJBaseGameLayer_removeObjectFromSection, GJBaseGameLayer* self, GameObject* object) {
	if(object) {
		GJBaseGameLayer_removeObjectFromSectionO(self, object);

		if(GM->_inEditor()) object->removeFromParent();
	}
}

// fix duals in playtest
FUNCTIONHOOK(void, PlayerObject_spawnDualCircle, PlayerObject* self) {
	if(!GM->_inEditor()) PlayerObject_spawnDualCircleO(self);
}

// testing big levels
FUNCTIONHOOK(bool, LevelInfoLayer_init, LevelInfoLayer* self, GJGameLevel* level, bool idk) {
	if(!LevelInfoLayer_initO(self, level, idk)) return false;

	#ifdef DEVDEBUG
	auto copyBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("bgIcon_01_001.png"), nullptr, self, menu_selector(LevelInfoLayer::onClone));
	auto menu = CCMenu::createWithItem(copyBtn); //LOL didnt know this existed...
	self->addChild(menu, 1000);
	#endif


		
	auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	sprite->setScale(.7);
	auto optionsBtn = CCMenuItemSpriteExtra::create(sprite, nullptr, self, menu_selector(MenuLayer::onOptions));
	auto bottomMenu2 = CCMenu::createWithItem(optionsBtn);
	reinterpret_cast<CCSprite *>(bottomMenu2)->setPosition({CCLEFT + 80, CCTOP - 25});
	self->addChild(bottomMenu2);

	return true;
}

FUNCTIONHOOK(bool, shouldShowInterstitial, GameManager* self, int a, int b, int c) {
	return false;
}

FUNCTIONHOOK(GameObject*, LevelEditorLayer_addObjectFromVector, LevelEditorLayer* self, void* vec) {
	auto obj = LevelEditorLayer_addObjectFromVectorO(self, vec);

	if(obj != nullptr) {
		int objID = MBO(int, obj, 0x3A0);
		if(objID < 3016 && objID >= 3006) {
			self->generateEnterEasingBuffers((EnterEffectObject*)obj);
		}

		return obj;
	}

	return GameObject::createWithKey(1);
}

FUNCTIONHOOK(void, DrawGridLayer_update, DrawGridLayer* self, float delta) {
	auto editor = MEMBERBYOFFSET(LevelEditorLayer*, self, 0x15C);
	
	//CCLog("%f", editor->_timeScale());

	if(MBO(bool, editor->_editorUI(), 0x18C)) {
		auto fmod = FMODAudioEngine::sharedEngine();

		if(self->_musicTime() == -1) self->_musicTime() = MBO(float, editor->_editorUI(), 0x28F4);

		float bgMusicTime;
		if(fmod->isBackgroundMusicPlaying()) {
			bgMusicTime = fmod->getBackgroundMusicTime();
		}
		else {
			bgMusicTime = self->_musicTime() + delta;
		}

		if(self->_musicTime() == -1 || fabsf(bgMusicTime - self->_musicTime()) >= 9999) {
			self->_anotherTime() = bgMusicTime;
		}
		else self->_anotherTime() = self->_anotherTime() + delta;

		self->_musicTime() = bgMusicTime;
	}
}


FUNCTIONHOOK(void, triggerShader, void* a1, void* a2) {
	
	if(GM->_inEditor()) {
		if(!GM->getGameVariable("69234"))
			return triggerShaderO(a1, a2);
	}
	else {
		if(!GM->ggv("200003"))
			return triggerShaderO(a1, a2);
	}

}
FUNCTIONHOOK(bool, LevelSettingsLayer_init, LevelSettingsLayer* self, LevelSettingsObject* settings, LevelEditorLayer* levelEditor) {
	
	if(!LevelSettingsLayer_initO(self, settings, levelEditor))
		return false;
	
	bool isStartPos = settings->_isStartPos();
	
	auto spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	
	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*)layer->getChildren()->objectAtIndex(1);
	
	
	if(!isStartPos) {
		
		auto mg_label = (CCLabelBMFont*)layer->getChildren()->objectAtIndex(13);
		auto mini_label = (CCLabelBMFont*)layer->getChildren()->objectAtIndex(14);
		auto dual_label = (CCLabelBMFont*)layer->getChildren()->objectAtIndex(15);

		mg_label->setPosition(mg_label->getPositionX() - 50, mg_label->getPositionY() - 65);
		mini_label->setPositionY(mini_label->getPositionY() - 60);
		dual_label->setPositionY(dual_label->getPositionY() - 55);

		auto platformer_toggle = (CCMenuItemToggler*)menu->getChildren()->objectAtIndex(29);
		auto dual_toggle = (CCMenuItemToggler*)menu->getChildren()->objectAtIndex(25);
		auto mini_toggle = (CCMenuItemToggler*)menu->getChildren()->objectAtIndex(24);
		auto font = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(26);
		auto middleground = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(23);
		auto ground = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(22);
		auto background = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(21);

		auto middlegroundspr = MBO(CCSprite*, self, 0x22C);
		auto backgroundspr = MBO(CCSprite*, self, 0x224);
		auto groundspr = MBO(CCSprite*, self, 0x228);
		
		
		int bgi = MBO(int, GM, 0x2BC);
		int gi = MBO(int, GM, 0x2C0);
		int mgi = MBO(int, GM, 0x2C4);
		
		const char *bgStr = "bgIcon_%02d_001.png";
		const char *gStr = "gIcon_%02d_001.png";

		bgStr = CCString::createWithFormat(bgStr, bgi)->getCString();
		gStr = CCString::createWithFormat(gStr, gi)->getCString();
		
		backgroundspr->CCSprite::setDisplayFrame(spriteCache->spriteFrameByName(bgStr));
		groundspr->CCSprite::setDisplayFrame(spriteCache->spriteFrameByName(gStr));
	
		font->setPosition(font->getPositionX() - 70, font->getPositionY() - 10);
		dual_toggle->setPositionY(dual_toggle->getPositionY() - 55);
		mini_toggle->setPositionY(mini_toggle->getPositionY() - 60);
		middleground->setPositionY(middleground->getPositionY() - 65);
		middleground->setPositionX(middleground->getPositionX() - 50);

		platformer_toggle->setPositionX(platformer_toggle->getPositionX() + 70);

		auto label = CCLabelBMFont::create("Platformer\nMode:", "goldFont.fnt");
		label->setAlignment(kCCTextAlignmentCenter);
		label->setPosition(platformer_toggle->getPositionX() - 55, platformer_toggle->getPositionY());
		label->setScale(.4);
		menu->addChild(label);
	}



	//nothing works wtfff
	int swingBtnIndex = !isStartPos ? 20 : 7;
	auto swing_toggle = (CCMenuItemToggler*)menu->getChildren()->objectAtIndex(swingBtnIndex);
	swing_toggle->setVisible(false);
	
	CCArray* togglerArray = MBO(CCArray*, self, 0x238);
	
	auto sprOff = CCSprite::createWithSpriteFrameName("gj_swingBtn_off_001.png");
	auto sprOn = CCSprite::createWithSpriteFrameName("gj_swingBtn_on_001.png");
	
	auto newToggle = CCMenuItemToggler::create(sprOff, sprOn, self, menu_selector(LevelSettingsLayer::onSelectMode));
	newToggle->setTag(7);
	newToggle->toggle(MBO(int, settings, 0x108) == 7);
	newToggle->setPosition(swing_toggle->getPosition());
	newToggle->setSizeMult(1.5);
	togglerArray->replaceObjectAtIndex(7, newToggle, false);
	menu->getChildren()->replaceObjectAtIndex(swingBtnIndex, newToggle, false);
	menu->addChild(newToggle);

	#ifdef DEVDEBUG
	//GDPSHelper::createLabels(layer, layer->getChildren(), {0, 0}, true);
		

	#endif

	auto miniToggle = (CCMenuItemToggler*)menu->getChildren()->objectAtIndex(!isStartPos ? 24 : 8);

	GDPSHelper::createToggleButton(
		"",
		miniToggle->getPosition(),
		.80, 8,
		self,
		menu_selector(LevelSettingsLayer::onMini),
		menu,
		MBO(bool, settings, 0x110),
		true
	);
	miniToggle->setPositionY(11111111);

	return true;
}


FUNCTIONHOOK(void, LevelSettingsLayer_selectArtClosed, LevelSettingsLayer* self, void* artLayer) {
	
	LevelSettingsLayer_selectArtClosedO(self, artLayer);
	
	LevelSettingsObject* settings = MBO(LevelSettingsObject*, self, 0x230);
	
	bool isStartPos = settings->_isStartPos();
	if(isStartPos)
		return;

	auto spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	
	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*)layer->getChildren()->objectAtIndex(1);
	

	auto backgroundspr = MBO(CCSprite*, self, 0x224);
	auto groundspr = MBO(CCSprite*, self, 0x228);
	
	int bgi = MBO(int, GM, 0x2BC);
	int gi = MBO(int, GM, 0x2C0);
	//int mgi = MBO(int, GM, 0x2C4);

	const char *bgStr = "bgIcon_%02d_001.png";
	const char *gStr = "gIcon_%02d_001.png";

	bgStr = CCString::createWithFormat(bgStr, bgi)->getCString();
	gStr = CCString::createWithFormat(gStr, gi)->getCString();

	backgroundspr->CCSprite::setDisplayFrame(spriteCache->spriteFrameByName(bgStr));
	groundspr->CCSprite::setDisplayFrame(spriteCache->spriteFrameByName(gStr));

}

FUNCTIONHOOK(void, onSelectMode, LevelSettingsLayer* self, CCObject* sender) {
	
	onSelectModeO(self, sender);
	
		
		if(sender && sender->getTag() == 7) {
			MBO(int, MBO(LevelSettingsObject*, self, 0x230), 0x108) = 7;
		}
	
}

#include "SelectFontLayer.h"
FUNCTIONHOOK(void, updateFontLabel, SelectFontLayer* self, CCObject* a2) {
	
	
	auto editor = MBO(LevelEditorLayer*, self, 0x1F0);
	void* unk = MBO(void*, editor, 0x33C);
	int font = MBO(int, unk, 0x124);
	
	int tag = a2->getTag();
		
		/*
		CCLog("font: %d", font);
		CCLog("tag : %d", tag);
		*/
		
		if(tag == 0)
		return updateFontLabelO(self, a2);
	
		if(font != 11)
		return updateFontLabelO(self, a2);

}


FUNCTIONHOOK(bool, validGroup, GameObject* obj, int group) 
{ return true; }

#include "handler.h"


void patchIcons(int gameMode, int amountt) {
	
	if(amountt > 255) //2 bytes limit
		amountt = 255;
		
	patch tms = patch();
	
	string amount = FunctionHelper::itohex(amountt);
	
	uintptr_t countForType;
	uintptr_t updatePlayerFrame;
	uintptr_t updatePlayerFrameExtra;
	uintptr_t baseKey;
	
	
	switch(gameMode) {
			
		case 1: //cube
			 countForType = 0x2802B2;
			 updatePlayerFrame = 0x2991DC;
			 baseKey = 0x28649A;	
		break;
		
		case 2: //ship
			 countForType = 0x280264;
			 updatePlayerFrame = 0x2997B0;
			 baseKey = 0x28649E;
		break;
		
		case 3: //ball
			countForType = 0x280292;
			updatePlayerFrame = 0x299320;
			baseKey = 0x2864A2;
		break;
		
		case 4: //ufo
			countForType = 0x280296;
			updatePlayerFrame = 0x299A44;
			baseKey = 0x2864A6;
		break;
		
		case 5: //wave
			countForType = 0x280270;
			updatePlayerFrame = 0x2995C0;
			baseKey = 0x2864AA;
	
		break;
		
		case 6: //robot
			countForType = 0x28029A;
			updatePlayerFrame = 0x299108;
			updatePlayerFrameExtra = 0x3AAC42;
			baseKey = 0x2864AE;
		break;
		
		case 7: //spider
			countForType = 0x2802A2;
			updatePlayerFrame = 0x29911A;
			updatePlayerFrameExtra = 0x3AAC3A;
			baseKey = 0x2864B2;
		break;
		
	}
	
	tms.addPatch("libcocos2dcpp.so", countForType, amount + "20"); //GameManager::countForType
	tms.addPatch("libcocos2dcpp.so", updatePlayerFrame, amount + "29"); //PlayerObjet::updatePlayer(gamemode)Frame
	tms.addPatch("libcocos2dcpp.so", baseKey, amount + "34"); // GameManager::calculateBaseKeyForIcons
	
	if(gameMode == 6 || gameMode == 7)
	tms.addPatch("libcocos2dcpp.so", updatePlayerFrameExtra, amount + "29"); // GJRobotSprite::updateFrame
	
	tms.Modify();	
}


FUNCTIONHOOK(void*, GJItemIcon_Init, int type, int key)
{
	if(type == 11 && key > 16)
		return nullptr;
	
	return GJItemIcon_InitO(type, key);
}

//TODO: test the p1 platformer, collision blocks(?), swing selection, port over mod badges


FUNCTIONHOOK(void, GJBaseGameLayer_toggleDual, GJBaseGameLayer* self, void* a1, void* a2, void* a3, void* a4) {

	GJBaseGameLayer_toggleDualO(self, a1, a2, a3, a4);
	
	if(GM->_inEditor())
	return;

	auto ui = MBO(UILayer*, GM->_playLayer(), 0x2CA0);

	if(!(MBO(bool, ui, 0x206)))
	return;

	auto dpadRight = ui->getChildByTag(0xBAE);
	if(!dpadRight)
	return;

	dpadRight->setVisible(ui->isTwoPlayer() && GM->_playLayer()->_isDual() && !GM->getGameVariable("10007"));
	
	//...
}

class GarageLayerCallback {
	public: void onShatter(CCObject* pSender) { GM->setGameVariable("shatterFX", reinterpret_cast<CCMenuItemToggler*>(pSender)->_isToggled()); }
};

FUNCTIONHOOK(bool, GJGarageLayer_init, GJGarageLayer* self) {
	
	if(!GJGarageLayer_initO(self))
	return false;

	auto dir = CCDirector::sharedDirector();
	
	auto copyBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("bgIcon_01_001.png"), nullptr, self, menu_selector(GJGarageLayer::onSelectTab));
	copyBtn->setTag(7);
	auto menu = CCMenu::create();
	menu->setPositionX(CCLEFT + 100);
	self->addChild(menu, 1000);
	
	auto sprOff = CCSprite::createWithSpriteFrameName("gj_swingBtn_off_001.png");
	sprOff->setScale(.9);
	auto sprOn = CCSprite::createWithSpriteFrameName("gj_swingBtn_on_001.png");
	sprOn->setScale(.9);
	
	auto swingToggle = CCMenuItemToggler::create(sprOff, sprOn, self, menu_selector(GJGarageLayer::onSelectTab));
	swingToggle->setTag(7);
	
	CCArray* btns = MBO(CCArray*, self, 0x158);
	
	//the menu is not a class member :skullirl:
	auto btnMenu = (CCMenu*)self->getChildren()->objectAtIndex(8);
		
	auto arrowMenu = CCMenu::create();
	arrowMenu->setPosition(btnMenu->getPosition());
	arrowMenu->addChild((CCNode*)btnMenu->getChildren()->objectAtIndex(9));
	arrowMenu->addChild((CCNode*)btnMenu->getChildren()->objectAtIndex(10));
	self->addChild(arrowMenu);
	
	btnMenu->getChildren()->removeLastObject(false);
	btnMenu->getChildren()->removeLastObject(false);
	
	
	btnMenu->addChild(swingToggle);
	btns->insertObject(swingToggle, 7);
	btnMenu->getChildren()->exchangeObjectAtIndex(7, 9);
	btnMenu->getChildren()->exchangeObjectAtIndex(8, 9);
	
	btnMenu->alignItemsHorizontallyWithPadding(-4.0);
	
	auto swing = (CCNode*)btnMenu->getChildren()->objectAtIndex(7);
	auto trails = (CCNode*)btnMenu->getChildren()->objectAtIndex(8);
	auto effects = (CCNode*)btnMenu->getChildren()->objectAtIndex(9);
	
	swing->setPositionX(swing->GPX() + 5);
	trails->setPositionX(trails->GPX() + 10);
	effects->setPositionX(effects->GPX() + 10);

	// shatter effect toggle
	auto btnToggleMenu = CCMenu::create();

	auto shatterToggle = CCMenuItemToggler::create(
		CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
		CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
		self,
		menu_selector(GarageLayerCallback::onShatter)
	);
	shatterToggle->toggle(!GM->getGameVariable("shatterFX"));
	btnToggleMenu->addChild(shatterToggle);

	btnToggleMenu->setPosition(dir->getScreenRight() - 30, 120);

	self->addChild(btnToggleMenu, 10);
	
	return true;
}

#include "SongInfoLayer.h"
int eSongID = 0;
FUNCTIONHOOK(bool, SongInfoLayer_init, SongInfoLayer* self, std::string a1,std::string a2,std::string a3,std::string a4,std::string a5,std::string a6, int a7) {
	
	if(!SongInfoLayer_initO(self, a1, a2, a3, a4, a5, a6, a7))
	return false;


	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(0);

	
	int songID = MBO(int, self, 0x1FC);
	eSongID = songID;
	int menuMusicSongID = MBO(int, GM, 0x32C);

	int practiceMusicSongID = MBO(int, GM, 0x33C);
	

	GDPSHelper::createToggleButton("Menu\nMusic",
	{ 10, -245 },
	.7,
	0.5,
	layer,
	menu_selector(SongInfoLayer::onMenuMusicFix),
	MBO(CCMenu*, self, 0x1B4), songID == menuMusicSongID, true);
	
	GDPSHelper::createToggleButton("Practice\nMusic",
	{ 100, -245 },
	.7,
	0.5,
	layer,
	menu_selector(SongInfoLayer::onPracticeMusicFix),
	MBO(CCMenu*, self, 0x1B4), songID == practiceMusicSongID, true);
	
	return true;
}

#include "SetGroupIDLayer.h"
FUNCTIONHOOK(bool, SetupObjectOptionsPopup_init, CCNode* self, GameObject* obj, CCArray* objs, SetGroupIDLayer* groupIDLayer) {
	
	if(!SetupObjectOptionsPopup_initO(self, obj, objs, groupIDLayer))
	return false;

	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = MBO(CCMenu*, self, 0x1B4);

	auto label = (CCLabelBMFont*)layer->getChildren()->objectAtIndex(12);
	if(!label || !containss(label->CCLabelBMFont::getString(), "Reverse")) 
	return true;

	label = (CCLabelBMFont*)layer->getChildren()->objectAtIndex(19);
	auto reflabel = (CCNode*)layer->getChildren()->objectAtIndex(18);
	label->setPosition(reflabel->getPosition());
	label->setPositionY(label->GPY() - 35);

	auto btn = (CCNode*)menu->getChildren()->objectAtIndex(19);
	auto refBtn = (CCNode*)menu->getChildren()->objectAtIndex(18);
	btn->setPosition(refBtn->getPosition());
	btn->setPositionY(btn->GPY() - 35);
	
	return true;
}

FUNCTIONHOOK(void, SupportLayer_onRestore, CCLayer* self, CCObject* sender) {
	/*
	auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + "crash.txt";
	std::ifstream ifs( path );
	std::stringstream ss;
	if (ifs.good())
	{
		CCLog("good");
		mkdir("sdcard/GDPS_Editor", 0777);
		system(fmt::format("cp -F {} sdcard/GDPS_Editor", path).c_str());
	}
	else
		CCLog("Bad");
	*/
}

FUNCTIONHOOK(void, PauseLayer_customSetup, PauseLayer* self) {
	
	PauseLayer_customSetupO(self);
	
	auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	sprite->setScale(.7);
	auto optionsBtn = CCMenuItemSpriteExtra::create(sprite, nullptr, self, menu_selector(MenuLayer::onOptions));
	auto bottomMenu2 = CCMenu::createWithItem(optionsBtn);
	reinterpret_cast<CCSprite *>(bottomMenu2)->setPosition({CCRIGHT - 80, CCTOP - 30});
	self->addChild(bottomMenu2);
	
}

FUNCTIONHOOK(void, onMoreGames, MenuLayer* self, CCObject* sender) {
	
	auto dir = cocos2d::CCDirector::sharedDirector( );
	auto scene = cocos2d::CCTransitionFade::create(0.5, MapSelectLayer::scene());
	dir->replaceScene( scene );
}

FUNCTIONHOOK(bool, SetupShaderEffectPopup_init, SetupTriggerPopup* self, EffectGameObject* object, CCArray* objects, int objectID) {
	auto ret = SetupShaderEffectPopup_initO(self, object, objects, objectID);

	/*
		fix shader triggers not toggling checkboxes
	*/
	self->determineStartValues();

	self->updateTouchTriggered();
	self->updateSpawnedByTrigger();

	auto spawnToggle = MBO(CCMenuItemToggler*, self, 0x204);
	spawnToggle->toggle(MBO(bool, self, 0x200));

	auto touchToggle = MBO(CCMenuItemToggler*, self, 0x208);
	touchToggle->toggle(MBO(bool, self, 0x201));

	// multi trigger
	auto arr = MBO(CCArray*, self, 0x20C);
	auto multiTrigger = (CCMenuItemToggler*)arr->objectAtIndex(0);
	multiTrigger->toggle(MBO(bool, self, 0x210));

	return ret;
}

template<class Type = cocos2d::CCNode*>
static Type getChildOfType(cocos2d::CCNode* node, size_t index) {
    auto indexCounter = static_cast<size_t>(0);
    for (size_t i = 0; i < node->getChildrenCount(); ++i) {
        auto obj = reinterpret_cast<Type>(
            node->getChildren()->objectAtIndex(i)
        );
        if (obj != nullptr) {
            if (indexCounter == index) {
                return obj;
            }
            ++indexCounter;
        }
    }
    return nullptr;
}

// stupid ass fix for playtest bg
FUNCTIONHOOK(void, GJBaseGameLayer_updateCameraBGArt, GJBaseGameLayer* self, CCPoint pos) {
	if(MBO(bool, self, 0x2780)) {
		auto posThing = self->_background()->getPosition();
		GJBaseGameLayer_updateCameraBGArtO(self, pos);
		self->_background()->setPosition(posThing);
	}
	else GJBaseGameLayer_updateCameraBGArtO(self, pos);
}

// area trigger in playtest
FUNCTIONHOOK(GameObject*, LevelEditorLayer_createObject, LevelEditorLayer* self, int objectID, CCPoint pos, bool idk) {
	auto obj = LevelEditorLayer_createObjectO(self, objectID, pos, idk);

	if(objectID < 3016 && objectID >= 3006) {
		self->generateEnterEasingBuffers((EnterEffectObject*)obj);
	}

	return obj;
}

FUNCTIONHOOK(void, PlayerObject_playDeathEffect2, PlayerObject* self) {
	PlayerObject_playDeathEffect2O(self);

	// shatter effect from sneak peek 1
	if(!GM->getGameVariable("shatterFX")) return;

	auto size = self->_playerScale() * MBO(float, self, 0x7C8);
	auto pos = self->getPosition();

	auto tex = CCRenderTexture::create(size * 2, size * 2);

	self->setPosition(CCPoint(25, 25));
	tex->beginWithClear(0, 0, 0, 0);
	self->visit();
	tex->end();

	self->setPosition(pos);
	auto exp = ExplodeItemNode::create(tex);
	MBO(float, exp, 68) = -100;

	// add to layer
	GM->_playLayer()->_gameLayer()->addChild(exp, 10000);
	exp->setPosition(self->getPosition());

	srand(time(nullptr)); // random seed
	float randGenFloat = randFloat(4) + 2;
	exp->createSprites(
		4 + rand() % 4, //6,
		4 + rand() % 4, //6,
		randGenFloat / 2, //3,
		randGenFloat, //5,
		4,
		4,
		6.4,
		0,
		ccc4FFromccc4B({255, 255, 255, 255}),
		ccc4FFromccc4B({255, 255, 255, 255}),
		true
	);

	self->setVisible(false);
}


FUNCTIONHOOK(const char*, GJSearchObject_getKey, void* self) {
	
	const char* toAdd = CCString::createWithFormat(
		"_%i_%i", 
		GLM->getBoolForKey("legendary_filter_custom"),
		GLM->getBoolForKey("platform_filter_custom")
	)->getCString();
	
	return CCString::createWithFormat("%s%s", GJSearchObject_getKeyO(self), toAdd)->getCString();
}
#include "LevelSearchLayer.h"


FUNCTIONHOOK(void, LevelSearchLayer_toggleTime, LevelSearchLayer* self, CCObject* sender) {
	LevelSearchLayer_toggleTimeO(self, sender);
	int tag = sender->getTag();
	//CCLog("tag: %d", tag);
	if(tag != 5)
		return;
	
	//002F0F24
	bool idk = self->checkTime(tag);
	GLM->setBoolForKey(idk, "platform_filter_custom");
}

FUNCTIONHOOK(void, LevelSearchLayer_toggleTimeNum, void* self, int filterNum, bool state) {
//	CCLog("filternum: %d, on: %d", filterNum, state);
	LevelSearchLayer_toggleTimeNumO(self, filterNum, state);
	
	if(filterNum == 5)
	GLM->setBoolForKey(state, "platform_filter_custom");
}


#include "SearchButton.h"
FUNCTIONHOOK(bool, LevelSearchLayer_init, CCLayer* self) {
	
	if(!LevelSearchLayer_initO(self)) 
		return false;
		
	//GDPSHelper::createLabels(self, self->getChildren(), {0, 0}, true);

		
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
	
	auto barbg = reinterpret_cast<CCNode*>(self->getChildren()->objectAtIndex(4));
	barbg->setContentSize(CCSize(410, 40));
	barbg->setPositionX(barbg->getPositionX() + 20);
	
	auto lenbg = reinterpret_cast<CCNode*>(self->getChildren()->objectAtIndex(14));
	lenbg->setContentSize(CCSize(420, 40));
	//lenbg->setPositionX(node->getPositionX() + 20);
	
	
	//SearchButton::create(char const*,char const*,float,char const*)


	auto quickSearch = reinterpret_cast<CCLabelBMFont*>(self->getChildren()->objectAtIndex(8));
	quickSearch->setVisible(false);
	
	auto spr = SearchButton::create("GJ_longBtn03_001.png", "Demonlist", 0.6, "rankIcon_all_001.png" );
	
	auto demonlistbtn = CCMenuItemSpriteExtra::create(
		spr,
		spr,
		self,
		//static_cast<cocos2d::SEL_MenuHandler>(&LevelSearchLayer::onClearString)
		static_cast<cocos2d::SEL_MenuHandler>(&LevelSearchLayer::onDemonList)
	);
	
	auto hofspr = SearchButton::create("GJ_longBtn03_001.png", "Hall of Fame", 0.5, "GJ_diamondsIcon_001.png" );
	
	auto hofBtn = CCMenuItemSpriteExtra::create(
		hofspr,
		hofspr,
		self,
		//static_cast<cocos2d::SEL_MenuHandler>(&LevelSearchLayer::onClearString)
		static_cast<cocos2d::SEL_MenuHandler>(&CreatorLayer::onFameLevels)
	);
	//
	
	auto btnsbg = reinterpret_cast<CCSprite*>(self->getChildren()->objectAtIndex(9));
	auto cs = btnsbg->getContentSize();
	btnsbg->setContentSize(CCSize(cs.width, cs.height + 35));
	
	auto searchBtnMenu = reinterpret_cast<CCMenu*>(self->getChildren()->objectAtIndex(10));
	searchBtnMenu->setPositionY(searchBtnMenu->getPositionY() - 19);
	auto dwnbtn = reinterpret_cast<CCMenuItemSpriteExtra*>(searchBtnMenu->getChildren()->objectAtIndex(0));
	demonlistbtn->setPosition(dwnbtn->GPX(), dwnbtn->GPY() + 35);
	
	auto likesbtn = reinterpret_cast<CCMenuItemSpriteExtra*>(searchBtnMenu->getChildren()->objectAtIndex(1));
	hofBtn->setPosition(likesbtn->GPX(), likesbtn->GPY() + 35);

	searchBtnMenu->addChild(demonlistbtn);
	searchBtnMenu->addChild(hofBtn);
	
	
	auto filtersLabel = reinterpret_cast<CCLabelBMFont*>(self->getChildren()->objectAtIndex(11));
	filtersLabel->setVisible(false);
	
	return true;			
}

FUNCTIONHOOK(void, LevelSearchLayer_onClearFilters, LevelSearchLayer* self) {
	
	LevelSearchLayer_onClearFiltersO(self);
	
	GLM->setBoolForKey(false, "platform_filter_custom");
	GLM->setBoolForKey(false, "legendary_filter_custom");
	
	self->toggleTimeNum(5, false);
}


FUNCTIONHOOK(void, SupportLayer_onEmail, void* self, void* a2) {
	
    FLAlertLayer::create( nullptr, "Version", version2str, "OK", nullptr, 250., false, 150. )->show( );
}
//cocos2d::CCSpriteFrameCache::removeSpriteFramesFromFile


FUNCTIONHOOK(void, LevelCell_loadCustomLevelCell, CCLayer* self) {
	
	LevelCell_loadCustomLevelCellO(self);
	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(1);
	//CCLog("count: %d", layer->getChildrenCount());
	//GDPSHelper::createLabels(layer);
	auto level = MBO(void*, self, 0x198);
	
	int likes = MBO(int, level, 0x200) - MBO(int, level, 0x204);
	int downloads = MBO(int, level, 0x178);
	int count = layer->getChildrenCount();
	
	if(1000 > downloads && 1000 > likes)
		return;
	
	for(int i = 0; i < count; i++)
	{
		auto node = (CCNode*)layer->getChildren()->objectAtIndex(i);
		
		if(auto label = dynamic_cast<CCLabelBMFont*>(node))
		{
			if(downloads > 1000 && atoi(label->CCLabelBMFont::getString()) == downloads)
			{
				label->CCLabelBMFont::setString(FunctionHelper::intToFormatString(downloads).c_str());
				label->setScale(label->getScale() * 1.1);
			}
			else if(likes > 1000 && atoi(label->CCLabelBMFont::getString()) == likes)
			{
				label->CCLabelBMFont::setString(FunctionHelper::intToFormatString(likes).c_str());
				label->setScale(label->getScale() * 1.1);
			}
		}
	}
}


FUNCTIONHOOK(std::string, GameLevelManager_getBasePostString, void* self) {
	
	auto ret = GameLevelManager_getBasePostStringO(self);
	//CCLog("posstring: %s", ret.c_str());
	
	if (!passwordTemp.empty())
	{
		auto AM = GJAccountManager::sharedState();

		if (ret.find("userName") != std::string::npos)
			ret = fmt::format("{}&password={}&gjp={}&", ret, passwordTemp, FunctionHelper::gjp(passwordTemp));
		else
			ret = fmt::format("{}&password={}&gjp={}&userName={}", ret, passwordTemp, FunctionHelper::gjp(passwordTemp), AM->_username());
	}
	
	//CCLog("after possttrn: %s", ret.c_str());


	return ret;
}
void loader()
{
	auto cocos2d = dlopen(targetLibName != "" ? targetLibName : NULL, RTLD_LAZY);

	#ifndef EMUI_FIX
	//Crash_Handler();
	#endif
	
	
	MenuLayerExt::ApplyHooks();
	EditLevelLayerExt::ApplyHooks();
	LevelEditorLayerExt::ApplyHooks();
	EditorPauseLayerExt::ApplyHooks();
	DPADHooks::ApplyHooks();
	CollisionFix::ApplyHooks();
	ShaderFix::ApplyHooks();
	SpeedrunTimer::ApplyHooks();
	AdvancedLevelInfo::ApplyHooks();
	GDPSManager::ApplyHooks();
	MoreSearchLayerExt::ApplyHooks();
	SwingIconFix::ApplyHooks();
	Options::ApplyHooks();

	#ifdef SHADERDEBUG
	DevDebugHooks::ApplyHooks();
	#endif
	
	//HOOK("_ZN11GJGameLevel6createEPN7cocos2d12CCDictionaryEb", testingshitH, testingshitO);
	HOOK("_ZN16GameLevelManager17getBasePostStringEv", GameLevelManager_getBasePostStringH, GameLevelManager_getBasePostStringO);
	HOOK("_ZN9LevelCell19loadCustomLevelCellEv", LevelCell_loadCustomLevelCellH, LevelCell_loadCustomLevelCellO);
	HOOK("_ZN9PlayLayer13levelCompleteEv", PlayLayer_levelCompletedH, PlayLayer_levelCompletedO);
	HOOK("_ZN16MoreOptionsLayer8onToggleEPN7cocos2d8CCObjectE", MoreOptionsLayer_onToggleH, MoreOptionsLayer_onToggleO);
	HOOK("_ZN18LevelSettingsLayer15selectArtClosedEP14SelectArtLayer", LevelSettingsLayer_selectArtClosedH, LevelSettingsLayer_selectArtClosedO);
	HOOK("_ZN16LevelSearchLayer13toggleTimeNumEib", LevelSearchLayer_toggleTimeNumH, LevelSearchLayer_toggleTimeNumO);
	HOOK("_ZN12SupportLayer7onEmailEPN7cocos2d8CCObjectE", SupportLayer_onEmailH, SupportLayer_onEmailO);
	HOOK("_ZN16LevelSearchLayer12clearFiltersEv", LevelSearchLayer_onClearFiltersH, LevelSearchLayer_onClearFiltersO);
	HOOK("_ZN16LevelSearchLayer4initEv", LevelSearchLayer_initH, LevelSearchLayer_initO);
	HOOK("_ZN14GJSearchObject6getKeyEv", GJSearchObject_getKeyH, GJSearchObject_getKeyO);
	HOOK("_ZN16LevelSearchLayer10toggleTimeEPN7cocos2d8CCObjectE", LevelSearchLayer_toggleTimeH, LevelSearchLayer_toggleTimeO);
	HOOK("_ZN12PlayerObject15playDeathEffectEv", PlayerObject_playDeathEffect2H, PlayerObject_playDeathEffect2O);
	HOOK("_ZN16LevelEditorLayer12createObjectEiN7cocos2d7CCPointEb", LevelEditorLayer_createObjectH, LevelEditorLayer_createObjectO);
	HOOK("_ZN15GJBaseGameLayer17updateCameraBGArtEN7cocos2d7CCPointE", GJBaseGameLayer_updateCameraBGArtH, GJBaseGameLayer_updateCameraBGArtO);
	HOOK("_ZN22SetupShaderEffectPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayEi", SetupShaderEffectPopup_initH, SetupShaderEffectPopup_initO); //this is the longest symbol i have ever seen
	HOOK("_ZN9MenuLayer11onMoreGamesEPN7cocos2d8CCObjectE", onMoreGamesH, onMoreGamesO);
	HOOK("_ZN10PauseLayer11customSetupEv", PauseLayer_customSetupH, PauseLayer_customSetupO);
	HOOK("_ZN12SupportLayer9onRestoreEPN7cocos2d8CCObjectE", SupportLayer_onRestoreH, SupportLayer_onRestoreO);
	HOOK("_ZN23SetupObjectOptionsPopup4initEP10GameObjectPN7cocos2d7CCArrayEP15SetGroupIDLayer", SetupObjectOptionsPopup_initH, SetupObjectOptionsPopup_initO);
	HOOK("_ZN13SongInfoLayer4initESsSsSsSsSsSsi", SongInfoLayer_initH, SongInfoLayer_initO);
	HOOK("_ZN13GJGarageLayer4initEv", GJGarageLayer_initH, GJGarageLayer_initO);
	HOOK("_ZN10GJItemIcon17createBrowserItemE10UnlockTypei", GJItemIcon_InitH, GJItemIcon_InitO);
	HOOK("_ZN15GJBaseGameLayer14toggleDualModeEP10GameObjectbP12PlayerObjectb", GJBaseGameLayer_toggleDualH, GJBaseGameLayer_toggleDualO);
	HOOK("_ZN16LevelEditorLayer10validGroupEP10GameObjectb", validGroupH, validGroupO);
	HOOK("_ZN14SelectArtLayer4initE13SelectArtType", SelectArtLayer_initH, SelectArtLayer_initO);
	HOOK("_ZN15SelectFontLayer12onChangeFontEPN7cocos2d8CCObjectE", updateFontLabelH, updateFontLabelO);
	HOOK("_ZN18LevelSettingsLayer12onSelectModeEPN7cocos2d8CCObjectE", onSelectModeH, onSelectModeO)
	HOOK("_ZN18LevelSettingsLayer4initEP19LevelSettingsObjectP16LevelEditorLayer", LevelSettingsLayer_initH, LevelSettingsLayer_initO);
	HOOK("_ZN15GJBaseGameLayer20triggerShaderCommandEP16ShaderGameObject", triggerShaderH, triggerShaderO);
	HOOK("_ZN13DrawGridLayer6updateEf", DrawGridLayer_updateH, DrawGridLayer_updateO);
	HOOK("_ZN16LevelEditorLayer19addObjectFromVectorERSt6vectorISsSaISsEE", LevelEditorLayer_addObjectFromVectorH, LevelEditorLayer_addObjectFromVectorO);
	HOOK("_ZN11GameManager22shouldShowInterstitialEiii", shouldShowInterstitialH, shouldShowInterstitialO);
	HOOK("_ZN14LevelInfoLayer4initEP11GJGameLevelb", LevelInfoLayer_initH, LevelInfoLayer_initO);
	HOOK("_ZN10GameObject11customSetupEv", GameObject_customSetupH,GameObject_customSetupO);
	HOOK("_ZN7UILayer4initEv", UILayer_initH, UILayer_initO);
	HOOK("_ZN10PauseLayer6onEditEPN7cocos2d8CCObjectE", PauseLayer_onEditH, PauseLayer_onEditO);
	HOOK("_ZN13EndLevelLayer6onEditEPN7cocos2d8CCObjectE", EndLevelLayer_onEditH, EndLevelLayer_onEditO);
	HOOK("_ZN9PlayLayer18togglePracticeModeEb", togglePracticeModeH, togglePracticeModeO);
	HOOK("_ZN9PlayLayer9addObjectEP10GameObject", PlayLayer_addObjectH, PlayLayer_addObjectO);
	HOOK("_ZN23SetupPickupTriggerPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayE", SetupPickupTriggerH, SetupPickupTriggerO);
	HOOK("_ZN20AccountRegisterLayer4initEv", AccountRegisterLayer_InitH, AccountRegisterLayer_InitO);
	HOOK("_ZN11GameManager12getMGTextureEi", GameManager_getMGTextureH, GameManager_getMGTextureO);
	HOOK("_ZN13ObjectToolbox13intKeyToFrameEi", keyToFrameH, keyToFrameO);
	HOOK("_ZN8EditorUI4initEP16LevelEditorLayer", EditorUI_InitH, EditorUI_InitO);
	HOOK("_ZN8EditorUI13selectObjectsEPN7cocos2d7CCArrayEb", EditorUI_SelectObjectsH, EditorUI_SelectObjectsO);
	HOOK("_ZN8EditorUI12selectObjectEP10GameObjectb", EditorUI_SelectObjectH, EditorUI_SelectObjectO);
	HOOK("_ZN10GameObject13createWithKeyEi", GameObject_createH, GameObject_createO);
	HOOK("_ZN12LoadingLayer4initEb", LoadingLayer_initH, LoadingLayer_initO);
	HOOK("_ZN7cocos2d8CCSprite6createEPKc", spriteCreateH, spriteCreateO);
	HOOK("_ZN7cocos2d8CCSprite25createWithSpriteFrameNameEPKc", spriteCreateFrameNameH, spriteCreateFrameNameO);
	HOOK("_ZN16GameStatsManager14isItemUnlockedE10UnlockTypei", isIconUnlockedH, isIconUnlockedO);
	//HOOK("_ZN12OptionsLayer11customSetupEv", OptionsLayerInitH, OptionsLayerInitO);
	HOOK("_ZN16GameLevelManager18ProcessHttpRequestESsSsSs10GJHttpType", LevelProcessH, LevelProcessO);
	HOOK("_ZN20MusicDownloadManager18ProcessHttpRequestESsSsSs10GJHttpType", MusicProcessH, MusicProcessO);
	HOOK("_ZN16GJAccountManager18ProcessHttpRequestESsSsSs10GJHttpType", AccountProcessH, AccountProcessO);
	HOOK("_ZN12CreatorLayer19canPlayOnlineLevelsEv", canPlayOnlineLevelsH, canPlayOnlineLevelsO);	
	HOOK("_ZN17AccountLoginLayer8onSubmitEPN7cocos2d8CCObjectE", AccountSubmitH, AccountSubmitO);
	HOOK("_ZN17AccountLoginLayer20loginAccountFinishedEii", LoginFinishedH, LoginFinishedO);
	HOOK("_ZN12LoadingLayer16getLoadingStringEv", getStringH, getStringO);
	
	patch tms = patch();
	
	#define NOP4(a, b) a.addPatch("libcocos2dcpp.so", b, "00 BF 00 BF")
	#define NOP2(a, b) a.addPatch("libcocos2dcpp.so", b, "00 BF")

	//ads
	NOP4(tms, 0x2726A6);
	NOP4(tms, 0x2EAE24);
	NOP4(tms, 0x2EBE3C);
	NOP4(tms, 0x2726A0);
	NOP4(tms, 0x34909C);
	NOP4(tms, 0x34B5D4);
	NOP4(tms, 0x3539CC);
	NOP4(tms, 0x27FDCE);
	NOP4(tms, 0x27FE9E);
	NOP4(tms, 0x27FE9E);
	

	NOP4(tms, 0x2EDA9E); //versus
	NOP4(tms, 0x26FF0E); //free levels runaction
	
	patchIcons(1, 154); //cube
	patchIcons(2, 169); //ship
	patchIcons(3, 114); //ball
	patchIcons(4, 148); //ufo
	patchIcons(5, 96); //wave
	patchIcons(6, 68); //robot
	patchIcons(7, 69); //spider

	tms.addPatch("libcocos2dcpp.so", 0x2802A6, "2120"); //swing
	
	//platformer filter
	tms.addPatch("libcocos2dcpp.so", 0x2F379A, "062D"); 
	tms.addPatch("libcocos2dcpp.so", 0x302C72, "062E");
	tms.addPatch("libcocos2dcpp.so", 0x2F38E2, "062E");
	
	
	//contact -> version in settings -> help
	tms.addPatch("libcocos2dcpp.so", 0x804FA9, "56 65 72 73 69 6F 6E");
	
		
//	NOP2(tms, 0x2D7126);
//	NOP4(tms, 0x2D7122);


	
	
	
//	tms.addPatch("libcocos2dcpp.so", 0x81121D, "68 74 74 70 3A 2F 2F 77 77 77 2E 62 6F 6F 6D 6C 69 6E 67 73 2E 63 6F 6D 2F 64 61 74 61 62 61 73 65 2F 67 65 74 47 4A 53 6F 6E 67 49 6E 66 6F 2E 70 68 70");
	
	//tms.addPatch("libcocos2dcpp.so", 0x382266, "4F F0 01 03"); // fix bg
	
	
	// I got no fucking idea why rob does this but.....
	// death effect reset fix
	NOP4(tms, 0x27B574);
	NOP4(tms, 0x27B592);
	NOP4(tms, 0x27B596);
	NOP2(tms, 0x27B59A);
	NOP4(tms, 0x27B59C);

	// custom object bypass
	NOP2(tms, 0x2D7126);
	tms.addPatch("libcocos2dcpp.so", 0x2D7260, "80 42 00 BF");

	tms.addPatch("libcocos2dcpp.so", 0x332442, "002D"); // general icon limit bypass
	tms.addPatch("libcocos2dcpp.so", 0x2803D0, "1421"); // general icon limit bypass
	
	
	
	// levelsettingslayer | all gd custom songs bypass, not only explorers/dash
	tms.addPatch("libcocos2dcpp.so", 0x2C437C, "27 29"); 
	tms.addPatch("libcocos2dcpp.so", 0x2C4384, "27 21");
	tms.addPatch("libcocos2dcpp.so", 0x2C43A6, "27 29");
	
	//filters | all gd custom song bypass, not only explorers/dash
	tms.addPatch("libcocos2dcpp.so", 0x2F165C, "28 28");
	tms.addPatch("libcocos2dcpp.so", 0x2F1622, "28 21");
	tms.addPatch("libcocos2dcpp.so", 0x2F161C, "28 29");
	
//002F165C

	tms.addPatch("libcocos2dcpp.so", 0x2EB9EE, "01 21"); // fix level name in pause

	tms.addPatch("libcocos2dcpp.so", 0x2C44EA, "4F F0 02 0A"); // FG

	tms.Modify();
	
	//memory leak goes brrrr
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	// init_handle();
	pthread_t t;
	pthread_create(&t, NULL,
		reinterpret_cast< void *(*)(void*) > (loader), NULL);

	return JNI_VERSION_1_6;
}