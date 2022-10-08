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


#include "hooks/MultiplayerLayerExt.h"
#include "hooks/MenuLayerExt.h"
#include "hooks/EditLevelLayerExt.h"
#include "hooks/LevelEditorLayerExt.h"
#include "hooks/EditorPauseLayerExt.h"
#include "hooks/PauseLayerExt.h"
#include "hooks/ShaderDebug.h"
#include "hooks/DPADControl.h"
#include "hooks/CollisionFix.h"
#include "hooks/ShaderFix.h"
#include "hooks/SpeedrunTimer.h"
#include "hooks/AdvancedLevelInfo.h"

/*
		FLAG USED FOR DEVELOPER MODE DEBUGGING LIKE SHADERS

		!!!!!!!!!!!!!!!!!!!!!!!! COMMENT OUT BEFORE RELEASING APK !!!!!!!!!!!!!!!!!!!!!!!!
*/


string passwordTemp = "";
bool colorPopup = true;
int nFont = 0;
bool first = true;
string testt = FunctionHelper::itos(3);
void(*GameManager_tryShowAdO)();
void GameManager_tryShowAdH() {}

bool test = false;


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

	if(!MEMBERBYOFFSET(bool, self, 0x206)) return true;
	
	DPADHooks::UILayerInit(self);

	return true;
}   
   
bool(*isIconUnlockedO)(void *, int, int);
bool isIconUnlockedH(void *self, int a1, int a2)
{ return true; }

CCSprite * (*spriteCreateO)(const char *textureName);
CCSprite* spriteCreateH(const char *textureName)
{
	if (containss(textureName, "GJ_square05.png"))
	return spriteCreateO("GJ_square04.png");

	auto ret = spriteCreateO(textureName);
	if (ret != nullptr)
		return ret;

	return spriteCreateO("pixel.png");
}

bool isGDPSSettings;

void(*OptionsLayerInitO)(OptionsLayer *self);
void OptionsLayerInitH(OptionsLayer *self)
{
	auto dir = CCDirector::sharedDirector();
	auto midx = dir->getScreenRight() / 2;
	auto midy = dir->getScreenTop() / 2;
	auto old_menu = CCMenu::create();
	auto layer = reinterpret_cast<CCLayer*> (self->getChildren()->objectAtIndex(0));

	auto oldSprite = cocos2d::CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
	auto old_btn = CCMenuItemSpriteExtra::create(
		oldSprite,
		oldSprite,
		self,
		menu_selector(OptionsLayer::onGDPSSettings));

	old_menu->addChild(old_btn, 100);
	old_btn->setPositionX(midx - 110);
	old_btn->setPositionY(midy);
	old_menu->setPosition({ 0, 0 });
	layer->addChild(old_menu, 100);

	return OptionsLayerInitO(self);
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

void(*addToggle_trp)(MoreOptionsLayer *self, const char *title, const char *code, const char *desc);
void addToggle_hk(MoreOptionsLayer *self, const char *title, const char *code, const char *desc)
{
	if (isGDPSSettings)
	{
		if (strcmp(title, "Show Orb Guide") == 0)
		{
			//   addToggle_trp(self, "Robtop Servers", "100001", "Change servers to robtop servers");
			//   addToggle_trp(self, "Force Platformer", "100002", "All levels are platformer mode");
			addToggle_trp(self, "Instantly open editor\nat startup", "100003", 0);
			//addToggle_trp(self, "Disable editor\nobject visibility", "100004", 0);
			addToggle_trp(self, "Enable pixel blocks\nin the editor", "100005", 0);
			addToggle_trp(self, "Enable Playtest", "100006", 0);
			addToggle_trp(self, "Hide Platformer\nbuttons", "10007", 0);
			addToggle_trp(self, "Swap platformer\njump sides", "100011", 0);
			addToggle_trp(self, "Playtest as\nSave and Play", "100008", "Playtest button makes the save and play action");
			addToggle_trp(self, "Practice Music", "0125", 0);
			addToggle_trp(self, "Disable arrow trigger\nfix", "1000010", 0);
			addToggle_trp(self, "Speedrun Timer", "1000011", "<cr>Red</c> means that the time is <cr>invalid</c>.\n Before starting a speedrun <cr>make sure to die atleast 1 time</c> because the first attempt time takes the enter transition into account and that will always be slower");
			addToggle_trp(self, "Show FPS", "0115", 0);
			#ifdef DEVDEBUG
			addToggle_trp(self, "pixel blocks amount\nby level name", "1000012", 0);
			#endif
			addToggle_trp(self, "Show Trigger\nActivations", "1000013", 0);
			//addToggle_trp(self, "Show Platformer Hitbox", "100009", 0);

			isGDPSSettings = false;
		}
	}
	else
	{
		addToggle_trp(self, title, code, desc);
	}
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
	for (int i = 0; i < strlen(gdpseditor); i++)
		original[i] = gdpseditor[i];

	return original;
}

void *(*LevelProcessO)(GameManager *gm, string a1, string a2, string a3, int a4);
void *LevelProcessH(GameManager *gm, string a1, string a2, string a3, int a4)
{
	return LevelProcessO(gm, replaceServers(a1), a2, a3, a4);
}

void *(*MusicProcessO)(void *idk, string a1, string a2, string a3, int a4);
void *MusicProcessH(void *idk, string a1, string a2, string a3, int a4)
{
	return MusicProcessO(idk, replaceServers(a1), a2, a3, a4);
}

void *(*AccountProcessO)(void *idk, string a1, string a2, string a3, int a4);
void *AccountProcessH(void *idk, string a1, string a2, string a3, int a4)
{
	return AccountProcessO(idk, replaceServers(a1), a2, a3, a4);
}

bool(*canPlayOnlineLevelsO)(CreatorLayer *self);
bool canPlayOnlineLevelsH(CreatorLayer *self)
{
	return true;
}

CCArray * (*getTriggerGroupO)(LevelEditorLayer *self, int a1);
CCArray* getTriggerGroupH(LevelEditorLayer *self, int a1)
{
	//	CCLog("Trigger group func %d", a1);

	//return getTriggerGroupO(self, a1);

	CCArray *a = CCArray::create();
	return a;

	/*
	auto ret = getTriggerGroupO(self, a1);
		CCLog("Trigger group func 2 %d", a1);

	CCLog(typeid(ret).name());
	return getTriggerGroupO(self, a1);
	*/

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
	CCLog(passwordTemp.c_str());
	
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

int customAmountByLevelName = 0;

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
		#ifdef DEVDEBUG
		if(customAmountByLevelName != 0)
			return GameObject_createO(pixelKey > customAmountByLevelName ? 136 : key);
		else
			return GameObject_createO(pixelKey > 140 ? 136 : key);
		#else
			return GameObject_createO(pixelKey > 140 ? 136 : key);
		#endif
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

	auto id = obj->_objectID();

	if(id == 2100){
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
		patch *s = new patch();
		string spider = "70 6F 72 74 61 6C 5F 31 37 5F 62 61 63 6B 5F 30 30 31 2E 70 6E 67";
		s->addPatch("libcocos2dcpp.so", 0x7E19BA, spider);
		s->Modify();
	}

	if (id == 1933)
	{
		patch *p = new patch();
		string swing = "70 6F 72 74 61 6C 5F 31 38 5F 62 61 63 6B 5F 30 30 31 2E 70 6E 67";
		p->addPatch("libcocos2dcpp.so", 0x7E19BA, swing);
		p->Modify();

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

#include "SetupTriggerPopup.h"
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

int lastTriggerObjectID = 0;

bool(*SetupTriggerPopupO)(SetupTriggerPopup *, EffectGameObject *, cocos2d::CCArray *, float, float);
bool SetupTriggerPopupH(SetupTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects, float x, float y)
{
	auto ret = SetupTriggerPopupO(self, object, objects, x, y);

	if (objects == nullptr || objects->count() > 1)
	{
		lastTriggerObjectID = 0;
		return ret;

	}

	if (colorPopup)
	{
		int id = object->_objectID();
		lastTriggerObjectID = id;
	}

	return ret;

}
#include "SetupAreaMoveTriggerPopup.h"
#include "InfoAlertButton.h"
#include "ColorSelectPopup.h"

void *(*ColorSelectPopupInitO)(ColorSelectPopup *, EffectGameObject *, cocos2d::CCArray *, ColorAction *);
void *ColorSelectPopupInitH(ColorSelectPopup *self, EffectGameObject *object, cocos2d::CCArray *objects, ColorAction *a4)
{
	colorPopup = false;
	auto ret = ColorSelectPopupInitO(self, object, objects, a4);
	colorPopup = true;
	return ret;
}

string areaArrows = "<cg>Arrows pointing outwards means the effect is applied when the center id approaches the target id.</c>\n\
<cp>Arrows pointing inwards will apply the area effect on the target id. Center id will  remove the area effect when it approacves the target id.</c>\n\
<cl>Line means the area effect will ignore y/x depending on how the line is placed.</c>\n\
<cy>EffectID: gives the area effect an id which can be stopped or changed by an animate area trigger.</c>";



bool(*infoButton)(string, string, float);
bool infoButton_hk(string title, string desc, float a3)
{
	auto ret = infoButton(title, desc, a3);

	if (GM->_inEditor() && colorPopup && lastTriggerObjectID != 0)
	{
		switch (lastTriggerObjectID)
		{
			case 901:	//move
				desc += "\n-<cp>x/y modifier 4 locks to the camera if lock to camera is enabled</c>";
				return infoButton(title, desc, a3);
				break;

			case 1346:	//rotate 
				desc += "\n- <cp>Follow p follows the players icon rotation. Some modes don't work with this.</c>";
				return infoButton(title, desc, a3);
				break;

			case 2067:	//scale
				desc += "\n- <cp>Scaling a singular object uses  the objects rotated x/y instead of grid x/y.</c>";
				return infoButton(title, desc, a3);
				break;

			case 1585:	//aninamte
				desc += "\n- <cp>Can also animate particle effects and animated object.</c>";
				return infoButton(title, desc, a3);
				break;

			case 2062:	//camera edge
				return infoButton("Camera Edge Trigger", "Stops the camera on a target ID.\n\
			Direction is based on the players directions.\n\
			Target id 0 unlocks camera egdes.\n\
			This only works when having a direction selected", a3);
				break;

			case 1914:	// static camera
				return infoButton("Static Camera", "Locks the camera to a specific object.\nOnly X and Y will will lock the camera to only scroll horizontally and vertically respectfully.\nExit Static moves the camera back to the player. This does not require a group.\nfollow ignores move time. Set move time to 0 to avoid the camera to instantly jump to the target.", a3);
				break;

			case 1817:	//pickup trigger
				return infoButton("Pickup Trigger", "Adds or subtracts a value from a counter.\nOverwrite count sets the value of the counter to that of the pickup trigger.", a3);
				break;

			case 1814:	//follow player Y
				return infoButton("Follow Player Y", "makes objects follow the Y position of the player\nOffset controls how much higher or lower the object should follow. \nDelay controls how much time passes until the object goes to the player Y position.\nMax Speed controls how fast the object should follow the players Y position\nSpeed multiplies the Max Speed.", a3);
				break;

			case 1916:	//camera offset
				return infoButton("Camera Offset", "Offsets the camera by X or Y.\nThe movement will be based on the original camera position, not the current one. This means offsetting the camera twice won't double the offset", a3);
				break;

			case 2016:	//camera guide
				return infoButton("Camera Guide", "Shows the position of the camera if the player was to spawn on the exact position of the Guide Trigger.\n <cg>The green line shows the entire visible area.</c>\n <cy>The yellow lines show where objects start to fade in and out.</c>", a3);
				break;

			case 1934:	//song trigger
				return infoButton(title, "<cy>Sets the time of the song in ms.</c>\n <cr>This means 1000ms = 1 second</c>", a3);
				break;

			case 1935:	//timewarp
				return infoButton("Timewarp", "Changes the speed of the game like a speedhack", a3);
				break;

			case 1811:	//instant count
				return infoButton("Instant Count", "Only checks for the number count of an item ID channel one time instead of continuously checking until being stopped. Equals checks for if the item ID count is the exact same as the set number, smaller and larger check for if the number is smaller or larger.", a3);
				break;

			case 2925:	//camera mode
				return infoButton("Camera Mode", "Enabled free mode for gamemodes with locked camera.\n\
			Padding determines when the camera moves with the player on y.\n\
			Easing eases the camera. (Who could've expected that!)", a3);
				break;

			case 2999:	//Middle Ground Trigger
				return infoButton("Middle Ground Trigger", "It moves the middle ground on Y.", a3);
				break;

			case 2899:	//options trigger
				return infoButton("Options Trigger", "Changes how the game behaves and what it should render.", a3);
				break;

			case 2903:	//gradient trigger
				desc += "\n <cg>Uses base and detail color. Does not work with hsv.</c>\n\
<cb>You can choose on what layer the gradient should be.</c>\n\
<co>Rotating the gradient trigger also rotates the gradient.</c>\n\
<cp>Vertex mode uses math instead of textures to create the gradient. U D L R (BL BR TL TR in vertex mode) places the egdes of the gradient on center ids.</c>\n\
<cr>Change the ID if you use more gradients.\n\
The maximum amount of ids is 255.</c>";
				return infoButton("Gradient Trigger", desc, a3);
				break;

			case 3006:	//Area Move 
				{
					string areaBase = "<cg>Length: 30 = 1 gridspace.</c>\n\
<cy>Offset: offsets the area effect</c>";

					string areaExtra = "\n<co>Easing/ease out: determines the shape of the dropoff of the area effect</c>\n\
	<cr>OffsetY: Offset of the area effect in the Y axis.</c>\n\
	<cp>ModFront/Back: modifier for easing.</c>\n\
	<cl>Deadzone: the range at which the area effect is completly applied from the egde of the area effect to the center.</c>";

					return infoButton("Area Move Trigger", areaBase + areaExtra, a3);
					break;
				}

			case 3007: //Area Rotate
				{

					string aR2 = "\n<co>Easing/ease out: determines the shape of the dropoff of the area effect</c>\n\
	<cr>OffsetY: Offset of the area effect in the Y axis.</c>\n\
	<cp>ModFront/Back: modifier for easing.</c>\n\
	<cl>Deadzone: the range at which the area effect is completly applied from the egde of the area effect to the center.</c>";

					string aR3 = "\n<cg>Rotation: the amount of degrees the objects will spin. Ignores locked rotation objects but also doesnt change the hitbox.</c>";

					string des1 = aR2 + aR3;

					return infoButton("Area Rotate Trigger", des1, a3);

					break;
				}

			case 3008: //Area Scale
				{

					string aR2 = "\n<co>Easing/ease out: determines the shape of the dropoff of the area effect</c>\n\
	<cr>OffsetY: Offset of the area effect in the Y axis.</c>\n\
	<cp>ModFront/Back: modifier for easing.</c>\n\
	<cl>Deadzone: the range at which the area effect is completly applied from the egde of the area effect to the center.</c>";

					string aR3 = "\n<cg>Scalex/y: Scales objects by their relative x/y unless a p group is used.</c>";
					string des1 =  aR2 + aR3;

					return infoButton("Area Scale Trigger", des1, a3);

				}

				break;

			case 3009: //Area Fade
				{

					string aR2 = "\n<cr>OffsetY: Offset of the area effect in the Y axis.</c>\n\
	<cp>ModFront/Back: modifier for easing.</c>\n\
	<cl>Deadzone: the range at which the area effect is completly applied from the egde of the area effect to the center.</c>";

					string aR3 = "\n<co>Opacity: the opacity the object should be set to.</c>";
					string des1 = aR2 + aR3;

					return infoButton("Area Fade Trigger", des1, a3);

				}

				break;

			case 3010: //Area Tint
				{

					string aR2 = "\n<cr>OffsetY: Offset of the area effect in the Y axis.</c>\n\
	<cp>ModFront/Back: modifier for easing.</c>\n\
	<cl>Deadzone: the range at which the area effect is completly applied from the egde of the area effect to the center.</c>";

					string aR3 = "\n <co>Color channel: to what colors the objects should change</c>\n\
			<cr>%: the intensity of the effect.</c>\n\
		<cg>Hsv: uses hsv instead of a color channel.</c>";

					string des1 = aR2 + aR3;

					return infoButton("Area Tint Trigger", des1, a3);

				}

				break;

			case 3011:	//All Area Anim Triggers
			case 3012:
			case 3013:
			case 3014:
			case 3015:
				return infoButton("Area Anim Trigger", "<cg>Animates an area effect.</c>\n\
<cj>Leave NA to not change that value</c>\n\
<co>Duration: the amount of time it takes to apply the effect.</c>\n\
<cy>Use EID: Use Effect ID</c>", a3);
				break;

		}
	}

	return ret;
}


bool(*SetupAreaMoveTriggerPopupO)(SetupAreaMoveTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaMoveTriggerPopuH(SetupAreaMoveTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaMoveTriggerPopupO(self, object, objects);

	colorPopup = false;

	auto layer2 = (CCLayer*) self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*) layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*) menu->getChildren()->objectAtIndex(36);
	auto reference2 = (CCMenuItemSpriteExtra*) menu->getChildren()->objectAtIndex(1);

	string movedist = "<cg>Movedist: how far the objects move.</c>\n\
<cy>MoveAngle: In what direction the objects move.</c>\n\
<cl>Relative: move direction is determined by the center.</c>\n\
<cp>XY mode: uses grid XY to move objects.</c>";

	auto btn = InfoAlertButton::create("Effect Options", areaArrows, 1);
	btn->setPositionX(CCLEFT - 190);
	btn->setPositionY(reference->getPositionY());

	auto btn2 = InfoAlertButton::create("Area Move Trigger", movedist, 1);
	btn2->setPositionY(reference2->getPositionY());
	btn2->setPositionX(reference2->getPositionX() + 35);

	auto array = CCArray::create();
	array->addObject(btn);
	self->addObjectsToPage(array, 1);

	btn->setVisible(reference->isVisible());

	menu->addChild(btn);
	menu->addChild(btn2);

	colorPopup = true;

	return ret;

}

bool(*SetupAreaRotateTriggerPopupO)(SetupAreaRotateTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaRotateTriggerPopupH(SetupAreaRotateTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaRotateTriggerPopupO(self, object, objects);

	colorPopup = false;

	auto layer2 = (CCLayer*) self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*) layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*) menu->getChildren()->objectAtIndex(36);

	auto btn = InfoAlertButton::create("Effect Options", areaArrows, 1);
	btn->setPositionX(CCLEFT - 190);
	btn->setPositionY(reference->getPositionY());

	auto array = CCArray::create();
	array->addObject(btn);
	self->addObjectsToPage(array, 1);

	btn->setVisible(reference->isVisible());
	menu->addChild(btn);

	colorPopup = true;

	return ret;

}

bool(*SetupAreaTransformTriggerPopupO)(SetupAreaTransformTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaTransformTriggerPopupH(SetupAreaTransformTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaTransformTriggerPopupO(self, object, objects);

	colorPopup = false;

	auto layer2 = (CCLayer*) self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*) layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*) menu->getChildren()->objectAtIndex(36);

	auto btn = InfoAlertButton::create("Effect Options", areaArrows, 1);
	btn->setPositionX(CCLEFT - 190);
	btn->setPositionY(reference->getPositionY());

	auto array = CCArray::create();
	array->addObject(btn);
	self->addObjectsToPage(array, 1);

	btn->setVisible(reference->isVisible());
	menu->addChild(btn);

	colorPopup = true;

	return ret;

}

FUNCTIONHOOK(const char*, GameManager_getMGTexture, GameManager* self, int a2) {

	//avoid temp variables
	GM->loadMiddleground(2);
	return CCString::createWithFormat("fg_%02d.png", a2)->getCString();
}

bool(*SetupAreaFadeTriggerPopupO)(SetupAreaFadeTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaFadeTriggerPopupH(SetupAreaFadeTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaFadeTriggerPopupO(self, object, objects);

	colorPopup = false;

	auto layer2 = (CCLayer*) self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*) layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*) menu->getChildren()->objectAtIndex(36);

	auto btn = InfoAlertButton::create("Effect Options", areaArrows, 1);
	btn->setPositionX(CCLEFT - 190);
	btn->setPositionY(reference->getPositionY());

	auto array = CCArray::create();
	array->addObject(btn);
	self->addObjectsToPage(array, 1);

	btn->setVisible(reference->isVisible());
	menu->addChild(btn);

	colorPopup = true;

	return ret;

}

bool(*SetupAreaTintTriggerPopupO)(SetupAreaTintTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaTintTriggerPopupH(SetupAreaTintTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaTintTriggerPopupO(self, object, objects);

	colorPopup = false;

	auto layer2 = (CCLayer*) self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*) layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*) menu->getChildren()->objectAtIndex(36);

	auto btn = InfoAlertButton::create("Effect Options", areaArrows, 1);
	btn->setPositionX(CCLEFT - 190);
	btn->setPositionY(reference->getPositionY());

	auto array = CCArray::create();
	array->addObject(btn);
	self->addObjectsToPage(array, 1);

	btn->setVisible(reference->isVisible());
	menu->addChild(btn);

	colorPopup = true;

	return ret;

}



FLAlertLayer* fl;

void LevelEditorLayerExt::onEnablePlaytest(CCObject* sender) {
	
	GM->setGameVariable("100006", true); 
	auto node = (CCNode*)sender;
	node->setPositionY(10000);
}

bool play;

void(*EditorUI_onPlaytestO)(EditorUI *self, CCObject *a2);
void EditorUI_onPlaytestH(EditorUI *self, CCObject *a2)
{
	//need to add comments because im confusing myself LOL
	//if playtest disabled
	if(GM->getGameVariable("100008") && GM->getGameVariable("100006")) {
		play = true;
		self->runAction(CCCallFuncO::create(self, callfuncO_selector(EditorUI::onPause), self));
		return;
	}
	
	if (!GM->getGameVariable("100006")) {
		
	string desc = "<co>Playtest has a lot of bugs and crashes</c>\n\
				<cr>its use is not recommended!</c>\n\
				<cg>You can enable playtest at your own risk</c>\n\
				<cr>don't complain about it and don't ask for a fix!</c>";
		fl = FLAlertLayer::create(nullptr, "Playtest disabled", desc, "OK", nullptr, 470, false, 200);

		auto menu = fl->_btnMenu();
		auto children = menu->getChildren();
		auto okBtn = reinterpret_cast<CCNode *>(children->objectAtIndex(0));

		auto sprite = ButtonSprite::create("Enable", 80, 30, 30, 5);
		auto btn = CCMenuItemSpriteExtra::create(sprite, sprite, fl, menu_selector(LevelEditorLayerExt::onEnablePlaytest));
		btn->setPositionX(okBtn->getPositionX() + 60);

		okBtn->setPositionX(okBtn->getPositionX() - 40);
		menu->addChild(btn);

		//  fl->m_pLayer->addChild(sprite, 50);
		fl->show();
	}
	
	else {
		return EditorUI_onPlaytestO(self, a2);
	}

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

	if(obj != nullptr) return obj;

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
		
	
	
	if(!GM->_inEditor())
	return triggerShaderO(a1, a2);
	
	if(!GM->getGameVariable("69234"))
	return triggerShaderO(a1, a2);


		
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

		auto middlegroundspr = (CCSprite*)middleground->getChildren()->objectAtIndex(0);
		auto groundspr = (CCSprite*)ground->getChildren()->objectAtIndex(0);
		auto backgroundspr = (CCSprite*)background->getChildren()->objectAtIndex(0);

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
		
		int a = MBO(int, settings, 0x108);
		CCLog("a: %d", a);
		
	#endif

	auto miniToggle = (CCMenuItemToggler*)menu->getChildren()->objectAtIndex(!isStartPos ? 24 : 8);

		
			auto mini = MBO(bool, settings, 0x110);
	
		GDPSHelper::createToggleButton(
		"",
		miniToggle->getPosition(),
		.80, 8,
		self,
		menu_selector(LevelSettingsLayer::onMini),
		menu,
		mini,
		true);
		
		miniToggle->setPositionY(11111111);


	return true;
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
		
	patch* tms = new patch();
	
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
	
	tms->addPatch("libcocos2dcpp.so", countForType, amount + "20"); //GameManager::countForType
	tms->addPatch("libcocos2dcpp.so", updatePlayerFrame, amount + "29"); //PlayerObjet::updatePlayer(gamemode)Frame
	tms->addPatch("libcocos2dcpp.so", baseKey, amount + "34"); // GameManager::calculateBaseKeyForIcons
	
	if(gameMode == 6 || gameMode == 7)
	tms->addPatch("libcocos2dcpp.so", updatePlayerFrameExtra, amount + "29"); // GJRobotSprite::updateFrame
	
	tms->Modify();
	
}


FUNCTIONHOOK(void*, GJItemIcon_Init, int type, int key)
{
	if(type == 11 && key > 17)
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


FUNCTIONHOOK(bool, GJGarageLayer_init, GJGarageLayer* self) {
	
	if(!GJGarageLayer_initO(self))
	return false;
	
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

	
	return true;
}

bool insideToggleSwingMode;

const char *(*CCString_getCStringO)(CCString*);
const char *CCString_getCStringH(CCString *self)
{
	auto ret = CCString_getCStringO(self);

	if (containss(ret, AY_OBFUSCATE("gjp2")) && !passwordTemp.empty())
	{
		auto AM = GJAccountManager::sharedState();
		const char *toAdd;

		if (containss(ret, "userName"))
			toAdd = CCString::createWithFormat("&password=%s&gjp=%s&", passwordTemp.c_str(), FunctionHelper::gjp(passwordTemp).c_str())->getCString();
		else
			toAdd = CCString::createWithFormat("&password=%s&gjp=%s&userName=%s", passwordTemp.c_str(), FunctionHelper::gjp(passwordTemp).c_str(), AM->_username().c_str())->getCString();
		

		//char *s = new char[strlen(ret) + strlen(toAdd) + strlen(toAdd2) + 1];
		char *s = new char[strlen(ret) + strlen(toAdd) + 1];
		strcpy(s, ret);
		strcat(s, toAdd);
		//strcat(s, toAdd2);

		//	CCLog(s);

		ret = s;

	}
	
	if(insideToggleSwingMode) {
	
		int swingKey = MBO(int, GM, 0x224);
		//CCLog("swingKey: %d", swingKey);
		if(swingKey == 1)
		return ret;
		

		if(containss(ret, "swing_01_001.png"))
		return CCString_getCStringO(CCString::createWithFormat("swing_%02d_001.png", swingKey));
	
		if(containss(ret, "swing_01_2_001.png"))
		return CCString_getCStringO(CCString::createWithFormat("swing_%02d_2_001.png", swingKey));
	
		if(containss(ret, "swing_01_glow_001.png"))
		return CCString_getCStringO(CCString::createWithFormat("swing_%02d_glow_001.png", swingKey));
	
		if(containss(ret, "swing_01_extra_001.png"))
		return CCString_getCStringO(CCString::createWithFormat("swing_%02d_extra_001.png", swingKey));

	}
	
	if(containss(ret, "splitscreen") && containss(ret, "shockWaveUV") && containss(ret, "chromaticGlitchUV")) {
		CCLog("enter if");
		auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + "uberShader.fsh";

		std::ifstream ifs( path );

		std::stringstream ss;

		if (ifs.good())
		{
			CCLog("opening file!!!");
			std::string sLine;
			while (getline(ifs, sLine))
			{
				ss << sLine << std::endl;
			}
			ifs.close();
			CCLog("returning updated shader!!!");
			return ss.str().c_str();
		}
		else
		{
			CCLog("file not found");
			ifs.close();
			return ret;
		}		
	}

	return ret;

}

FUNCTIONHOOK(void*, toggleSwingMode, PlayerObject* self,  bool a1, bool a2) {

	insideToggleSwingMode = true;
	
	if(a1)  //bitch icon wont load sometimes
	GM->loadIcon(MBO(int, GM, 0x224), 7, MBO(int, GM, 0xBE0));

	auto ret = toggleSwingModeO(self, a1, a2);
	
	insideToggleSwingMode = false;
	
	return ret;
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
	
	auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + "crash.txt";

    std::ifstream ifs( path );

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

FUNCTIONHOOK(void, PauseLayer_customSetup, PauseLayer* self) {
	
	PauseLayer_customSetupO(self);
	
	auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	sprite->setScale(.7);
	auto optionsBtn = CCMenuItemSpriteExtra::create(sprite, nullptr, self, menu_selector(MenuLayer::onOptions));
	auto bottomMenu2 = CCMenu::createWithItem(optionsBtn);
	reinterpret_cast<CCSprite *>(bottomMenu2)->setPosition({CCRIGHT - 80, CCTOP - 30});
	self->addChild(bottomMenu2);
	
}

/* change swing portal color (not working)
	
FUNCTIONHOOK(CCParticleSystemQuad*, createParticle, GJBaseGameLayer* self, int a1, char const* a2, int a3, void* a4) {
	
	CCLog("a1: %d, a2: %s, a3: %d", a1, a2, a3);
	
	auto ret = createParticleO(self, a1, a2, a3, a4);
	
		ret->CCParticleSystem::setStartColor({255, 0, 0, 100});
		ret->CCParticleSystem::setEndColor({255, 0, 0, 100});
	

	return ret;
	
}

FUNCTIONHOOK(GameObject*, GameObject_createAndAddParticle, GameObject* obj, int a1, char const* a2, int a3, void* a4) {
	
	auto ret = reinterpret_cast<CCParticleSystem*>(GameObject_createAndAddParticleO(obj, a1, a2, a3, a4));

		ret->CCParticleSystem::setStartColor({255, 0, 0, 100});
		ret->CCParticleSystem::setEndColor({255, 0, 0, 100});
		
	return reinterpret_cast<GameObject*>(ret);
}
*/

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

	#ifdef SHADERDEBUG
	DevDebugHooks::ApplyHooks();
	#endif


//	HOOK("_ZN11ShaderLayer4initEv", ShaderLayer_initH, ShaderLayer_initO);
	//HOOK("_ZN11ShaderLayer18triggerColorChangeEfffffffif", triggerColorChangeH, triggerColorChangeO);
	//HOOK("_ZN16LevelEditorLayer14recreateGroupsEv", recreateGroupsH, recreateGroupsO);
//	HOOK("_ZN10GameObject20createAndAddParticleEiPKciN7cocos2d15tCCPositionTypeE", GameObject_createAndAddParticleH, GameObject_createAndAddParticleO);
	//HOOK("_ZN15GJBaseGameLayer14createParticleEiPKciN7cocos2d15tCCPositionTypeE", createParticleH, createParticleO);
		
	HOOK("_ZN22SetupShaderEffectPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayEi", SetupShaderEffectPopup_initH, SetupShaderEffectPopup_initO);

	HOOK("_ZN15GJBaseGameLayer17updateCameraBGArtEN7cocos2d7CCPointE", GJBaseGameLayer_updateCameraBGArtH, GJBaseGameLayer_updateCameraBGArtO);
	
	HOOK("_ZN22SetupShaderEffectPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayEi", SetupShaderEffectPopup_initH, SetupShaderEffectPopup_initO); //this is the longest symbol i have ever seen
	HOOK("_ZN9MenuLayer11onMoreGamesEPN7cocos2d8CCObjectE", onMoreGamesH, onMoreGamesO);
	HOOK("_ZN10PauseLayer11customSetupEv", PauseLayer_customSetupH, PauseLayer_customSetupO);
	HOOK("_ZN12SupportLayer9onRestoreEPN7cocos2d8CCObjectE", SupportLayer_onRestoreH, SupportLayer_onRestoreO);
	HOOK("_ZN23SetupObjectOptionsPopup4initEP10GameObjectPN7cocos2d7CCArrayEP15SetGroupIDLayer", SetupObjectOptionsPopup_initH, SetupObjectOptionsPopup_initO);
	HOOK("_ZN13SongInfoLayer4initESsSsSsSsSsSsi", SongInfoLayer_initH, SongInfoLayer_initO);
	HOOK("_ZN12PlayerObject15toggleSwingModeEbb", toggleSwingModeH, toggleSwingModeO);
	HOOK("_ZN13GJGarageLayer4initEv", GJGarageLayer_initH, GJGarageLayer_initO);
	HOOK("_ZN10GJItemIcon17createBrowserItemE10UnlockTypei", GJItemIcon_InitH, GJItemIcon_InitO);
	//HOOK("_ZN11GameManager8loadIconEiii", someHookToLogStuffH, someHookToLogStuffO);
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
	//HOOK("_ZN15GJBaseGameLayer12addToSectionEP10GameObject", GJBaseGameLayer_addToSectionH, GJBaseGameLayer_addToSectionO);
	//HOOK("_ZN15GJBaseGameLayer23removeObjectFromSectionEP10GameObject", GJBaseGameLayer_removeObjectFromSectionH, GJBaseGameLayer_removeObjectFromSectionO);

	HOOK("_ZN7UILayer4initEv", UILayer_initH, UILayer_initO);
	HOOK("_ZN10PauseLayer6onEditEPN7cocos2d8CCObjectE", PauseLayer_onEditH, PauseLayer_onEditO);
	HOOK("_ZN13EndLevelLayer6onEditEPN7cocos2d8CCObjectE", EndLevelLayer_onEditH, EndLevelLayer_onEditO);
	
	HOOK("_ZN9PlayLayer18togglePracticeModeEb", togglePracticeModeH, togglePracticeModeO);
	HOOK("_ZN8EditorUI10onPlaytestEPN7cocos2d8CCObjectE", EditorUI_onPlaytestH, EditorUI_onPlaytestO);
	HOOK("_ZN25SetupAreaTintTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaTintTriggerPopupH, SetupAreaTintTriggerPopupO);
	HOOK("_ZN25SetupAreaFadeTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaFadeTriggerPopupH, SetupAreaFadeTriggerPopupO);
	HOOK("_ZN30SetupAreaTransformTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaTransformTriggerPopupH, SetupAreaTransformTriggerPopupO);
	HOOK("_ZN27SetupAreaRotateTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaRotateTriggerPopupH, SetupAreaRotateTriggerPopupO);
	HOOK("_ZN25SetupAreaMoveTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaMoveTriggerPopuH, SetupAreaMoveTriggerPopupO);
	HOOK("_ZN16ColorSelectPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayEP11ColorAction", ColorSelectPopupInitH, ColorSelectPopupInitO);
	HOOK("_ZN15InfoAlertButton6createESsSsf", infoButton_hk, infoButton);
	HOOK("_ZN9PlayLayer9addObjectEP10GameObject", PlayLayer_addObjectH, PlayLayer_addObjectO);
	HOOK("_ZN17SetupTriggerPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayEff", SetupTriggerPopupH, SetupTriggerPopupO);
	HOOK("_ZN23SetupPickupTriggerPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayE", SetupPickupTriggerH, SetupPickupTriggerO);
	HOOK("_ZN20AccountRegisterLayer4initEv", AccountRegisterLayer_InitH, AccountRegisterLayer_InitO);
	HOOK("_ZN11GameManager12getMGTextureEi", GameManager_getMGTextureH, GameManager_getMGTextureO);
//	HOOK("_ZN11GameManager18toggleGameVariableEPKc", hook_onToggle, onToggleTrampoline);
	
	HOOK("_ZN13ObjectToolbox13intKeyToFrameEi", keyToFrameH, keyToFrameO);
	HOOK("_ZN8EditorUI4initEP16LevelEditorLayer", EditorUI_InitH, EditorUI_InitO);
	
	HOOK("_ZN8EditorUI13selectObjectsEPN7cocos2d7CCArrayEb", EditorUI_SelectObjectsH, EditorUI_SelectObjectsO);
	HOOK("_ZN8EditorUI12selectObjectEP10GameObjectb", EditorUI_SelectObjectH, EditorUI_SelectObjectO);
	HOOK("_ZN10GameObject13createWithKeyEi", GameObject_createH, GameObject_createO);
	//HOOK("_ZN14LevelInfoLayer4initEP11GJGameLevelb", LevelInfoLayerInitH, LevelInfoLayerInitO);	
	HOOK("_ZN12LoadingLayer4initEb", LoadingLayer_initH, LoadingLayer_initO);

	HOOK("_ZN7cocos2d8CCSprite6createEPKc",
		spriteCreateH, spriteCreateO);
	HOOK("_ZN7cocos2d8CCSprite25createWithSpriteFrameNameEPKc",
		spriteCreateFrameNameH, spriteCreateFrameNameO);
		
	HOOK("_ZN16GameStatsManager14isItemUnlockedE10UnlockTypei",
		isIconUnlockedH, isIconUnlockedO);
	HOOK("_ZN16MoreOptionsLayer9addToggleEPKcS1_S1_",
		addToggle_hk, addToggle_trp);
	HOOK("_ZN12OptionsLayer11customSetupEv",
		OptionsLayerInitH, OptionsLayerInitO);
	HOOK("_ZNK7cocos2d8CCString10getCStringEv",
		CCString_getCStringH, CCString_getCStringO);
		
		
	HOOK("_ZN16GameLevelManager18ProcessHttpRequestESsSsSs10GJHttpType",
		LevelProcessH, LevelProcessO);
	HOOK("_ZN20MusicDownloadManager18ProcessHttpRequestESsSsSs10GJHttpType",
		MusicProcessH, MusicProcessO);
	HOOK("_ZN16GJAccountManager18ProcessHttpRequestESsSsSs10GJHttpType",
		AccountProcessH, AccountProcessO);
		
		
		
		
	HOOK("_ZN12CreatorLayer19canPlayOnlineLevelsEv",
		canPlayOnlineLevelsH, canPlayOnlineLevelsO);
	/*
	HOOK("_ZN16LevelEditorLayer15getTriggerGroupEi",
		getTriggerGroupH, getTriggerGroupO);
		*/

	/*
	HOOK("_ZN16LevelEditorLayer10addToGroupEP10GameObjectib",
		addToGroupH, addToGroupO);
		
		
	/*
	HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, "_ZN11AppDelegate11trySaveGameEb"), (void*) save_hook, (void **) &save_trp);
	HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, "_ZN7cocos2d11CCFileUtils13addSearchPathEPKc"), (void*) CCFileUtils_addSearchPath_hk, (void **) &CCFileUtils_addSearchPath_trp);
	HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, "_ZN11GameManager10dataLoadedEP13DS_Dictionary"), (void*) &GameManager_dataLoaded_hk, (void **) &dataLoaded_trp);
	*/
	
	HOOK("_ZN17AccountLoginLayer8onSubmitEPN7cocos2d8CCObjectE", AccountSubmitH, AccountSubmitO);
	HOOK("_ZN17AccountLoginLayer20loginAccountFinishedEii", LoginFinishedH, LoginFinishedO);
	HOOK("_ZN12LoadingLayer16getLoadingStringEv", getStringH, getStringO);
	
	patch *tms = new patch();
	
	#define NOP4(a, b) a->addPatch("libcocos2dcpp.so", b, "00 BF 00 BF")
	#define NOP2(a, b) a->addPatch("libcocos2dcpp.so", b, "00 BF")

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
//	NOP4(tms, 0x2EDA74); //gauntlets
	
	//https://cdn.discordapp.com/attachments/997593414684135485/1014791808666042438/icons.rar
	
	patchIcons(1, 154); //cube
	patchIcons(2, 169); //ship
	patchIcons(3, 114); //ball
	patchIcons(4, 148); //ufo
	patchIcons(5, 96); //wave
	patchIcons(6, 68); //robot
	patchIcons(7, 69); //spider

	tms->addPatch("libcocos2dcpp.so", 0x2802A6, "2120"); //swing
	
	
	
//	tms->addPatch("libcocos2dcpp.so", 0x81121D, "68 74 74 70 3A 2F 2F 77 77 77 2E 62 6F 6F 6D 6C 69 6E 67 73 2E 63 6F 6D 2F 64 61 74 61 62 61 73 65 2F 67 65 74 47 4A 53 6F 6E 67 49 6E 66 6F 2E 70 68 70");
	
	//tms->addPatch("libcocos2dcpp.so", 0x382266, "4F F0 01 03"); // fix bg
	
	
	


	tms->addPatch("libcocos2dcpp.so", 0x332442, "002D"); // general icon limit bypass
	tms->addPatch("libcocos2dcpp.so", 0x2803D0, "1421"); // general icon limit bypass
	
	
	tms->addPatch("libcocos2dcpp.so", 0x2C437C, "16 29"); // explorers song bypass fix in levelsettingslayer
	tms->addPatch("libcocos2dcpp.so", 0x2C4384, "16 21"); // explorers song bypass fix in levelsettingslayer 2
	tms->addPatch("libcocos2dcpp.so", 0x2C43A6, "16 29"); // explorers song bypass fix in levelsettingslayer 3
	
	tms->addPatch("libcocos2dcpp.so", 0x2EB9EE, "01 21"); // fix level name in pause

	tms->addPatch("libcocos2dcpp.so", 0x2C44EA, "4F F0 02 0A"); // FG
	


	tms->Modify();
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	// init_handle();
	pthread_t t;
	pthread_create(&t, NULL,
		reinterpret_cast< void *(*)(void*) > (loader), NULL);

	return JNI_VERSION_1_6;
}
