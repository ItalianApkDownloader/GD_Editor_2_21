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

#include "hooks/MultiplayerLayerExt.h"
#include "hooks/MenuLayerExt.h"
#include "hooks/EditLevelLayerExt.h"
#include "hooks/LevelEditorLayerExt.h"
#include "hooks/EditorPauseLayerExt.h"
#include "hooks/PauseLayerExt.h"
#include "hooks/ShaderDebug.h"

/*
		FLAG USED FOR DEVELOPER MODE DEBUGGING LIKE SHADERS

		!!!!!!!!!!!!!!!!!!!!!!!! COMMENT OUT BEFORE RELEASING APK !!!!!!!!!!!!!!!!!!!!!!!!
*/




string passwordTemp = "";
bool colorPopup = true;
int nFont = 0;

void(*GameManager_tryShowAdO)();
void GameManager_tryShowAdH() {}

bool test = false;

void *(*GameObjectSetOpacityO)(GameObject *self, unsigned char opacity);
void *GameObjectSetOpacityH(GameObject *self, unsigned char opacity)
{ return GameObjectSetOpacityO(self, opacity); }


	CCRect Rmain = {0, 0, 240, 100};
	CCRect Rsep = {0, 0, 240, 100};

	
	bool p1Jumping;
	bool p2Jumping;
	
	// make platformer dpad visible
	#include "CCDrawNode.h"
	
FUNCTIONHOOK(void, GJBaseGameLayer_toggleDual, GJBaseGameLayer* self, void* a1, void* a2, void* a3, void* a4) {
	
  
	GJBaseGameLayer_toggleDualO(self, a1, a2, a3, a4);
	if(GM->_inEditor()) return ;
	
	auto uilayer = MBO(UILayer*, GM->_playLayer(), 0x2CA0);
	bool platformerBtns_visible = self->_isDual();
	
	if(uilayer->_platformer() && uilayer->isTwoPlayer()) {
	
		auto dpad = (CCSprite*)uilayer->getChildByTag(100);
		auto dpad_dwn = (CCSprite*)uilayer->getChildByTag(101);
	
		dpad->CCSprite::setOpacity(platformerBtns_visible ? 255 : 0);
		dpad_dwn->CCSprite::setOpacity(platformerBtns_visible ? 255 : 0);
	}
	
					
	
	
}
FUNCTIONHOOK(bool, UILayer_init, UILayer* self) {
	if(!UILayer_initO(self)) return false;
	

	
	if(!MEMBERBYOFFSET(bool, self, 0x206)) return true;
	
		auto dpad = MEMBERBYOFFSET(CCSprite*, self, 0x1D8);
		bool platformerBtns_visible = !GM->getGameVariable("10007");
		dpad->setVisible(platformerBtns_visible);

		auto node = cocos2d::CCDrawNode::create();
		cocos2d::CCPoint m_points[4];
	
		auto Lmain = self->_Lmain();
		auto Lsep = self->_Lsep();
		
		
	  if(self->isTwoPlayer()) {

		
		auto dpadRight_Dwn = CCSprite::createWithSpriteFrameName("Dpad_Btn_Dwn.png"); //Left default!!
		dpadRight_Dwn->setPositionY(dpad->GPY());
		dpadRight_Dwn->setPositionX(CCRIGHT - (dpad->GPX() - CCLEFT));
		dpadRight_Dwn->setTag(101);
		dpadRight_Dwn->setVisible(platformerBtns_visible);
		self->addChild(dpadRight_Dwn);
		
		auto dpadRight = CCSprite::createWithSpriteFrameName("Dpad_Btn.png");
		dpadRight->setPositionY(dpad->GPY());
		dpadRight->setPositionX(CCRIGHT - (dpad->GPX() - CCLEFT));
		dpadRight->setTag(100);
		dpadRight->setVisible(platformerBtns_visible);
		self->addChild(dpadRight);
		
		

		Rsep = Lsep;
		Rmain = Lmain;
		
		Rsep.origin.x += 525;
		Rmain.origin.x += 470;
				
				
			if(GM->getGameVariable("100009"))
			{
				m_points[0] = ccp(Rsep.getMinX(),Rsep.getMinY());
				m_points[1] = ccp(Rsep.getMaxX(),Rsep.getMinY());
				m_points[2] = ccp(Rsep.getMaxX(),Rsep.getMaxY());
				m_points[3] = ccp(Rsep.getMinX(),Rsep.getMaxY());
				node->drawPolygon(m_points, 4, ccc4f(0, 0, 0, 0), 1, ccc4f(1, 1, 0, 1));
	
				m_points[0] = ccp(Rmain.getMinX(),Rmain.getMinY());
				m_points[1] = ccp(Rmain.getMaxX(),Rmain.getMinY());
				m_points[2] = ccp(Rmain.getMaxX(),Rmain.getMaxY());
				m_points[3] = ccp(Rmain.getMinX(),Rmain.getMaxY());
				node->drawPolygon(m_points, 4, ccc4f(0, 0, 0, 0), 1, ccc4f(1, 0, 0, 1));
			}
		
		}
		
	if(GM->getGameVariable("100009")) 
	{
	  m_points[0] = ccp(Lmain.getMinX(),Lmain.getMinY());
	  m_points[1] = ccp(Lmain.getMaxX(),Lmain.getMinY());
	  m_points[2] = ccp(Lmain.getMaxX(),Lmain.getMaxY());
	  m_points[3] = ccp(Lmain.getMinX(),Lmain.getMaxY());
	  node->drawPolygon(m_points, 4, ccc4f(0, 0, 0, 0), 1, ccc4f(1, 0, 0, 1));
	
	  m_points[0] = ccp(Lsep.getMinX(),Lsep.getMinY());
	  m_points[1] = ccp(Lsep.getMaxX(),Lsep.getMinY());
	  m_points[2] = ccp(Lsep.getMaxX(),Lsep.getMaxY());
	  m_points[3] = ccp(Lsep.getMinX(),Lsep.getMaxY());
	  node->drawPolygon(m_points, 4, ccc4f(0, 0, 0, 0), 1, ccc4f(1, 1, 0, 1));
	
	  self->addChild(node);
	}



	#ifdef SHADERDEBUG
	reinterpret_cast<UILayerDebug*>(self)->doInit();
	#endif

	return true;
}


//take a look at 2p-p bad implementation.cpp i guess 
bool(*UILayer_ccTouchBeganO)(UILayer *self, CCTouch *touch, CCEvent *event);
bool UILayer_ccTouchBeganH(UILayer *self, CCTouch *touch, CCEvent *event)
{
	CCLog("------ BEGAN ------");
	bool ret = UILayer_ccTouchBeganO(self, touch, event);


	bool is_platformer = self->_platformer();
	if (!is_platformer) return ret;
	
	auto &touch_id = self->_touchID();
	if (touch_id != -1) return ret;

	auto p1 = GM->_playLayer()->_player1();
	auto p2 = GM->_playLayer()->_player2();
	auto touch_pos = touch->getLocation();
	


	//touch_pos = self->convertToNodeSpace(touch_pos);

	if (touch_pos.x < 285 && touch_pos.y > 100){
			p1->pushButton(Jump);
	}

	if( touch_pos.x > 285 && touch_pos.y > 100){
			p2->pushButton(Jump);
	} 
	
	if(touch_pos.x < 500 && touch_pos.y < 100){
			p2->pushButton(Left);
	} else if(touch_pos.x > 500 && touch_pos.y < 100){
			p2->pushButton(Right);
	} 
	//normal jump done
	
	


	if(!self->isDual() || !self->isTwoPlayer()) return ret;

	//do platformer 2p stuff
	
	auto Lmain = self->_Lmain();
	auto Lsep = self->_Lsep();
	

		return ret;
}

void(*UILayer_ccTouchEndedO)(UILayer *self, CCTouch *touch, CCEvent *event);
void UILayer_ccTouchEnded(UILayer *self, CCTouch *touch, CCEvent *event)
{
	CCLog("------ END   ------");
	UILayer_ccTouchEndedO(self, touch, event);
	
	auto p2 = GM->_playLayer()->_player2();
	auto p1 = GM->_playLayer()->_player1();

	p2->releaseButton(Jump);
	p2->releaseButton(Right);
	p2->releaseButton(Left);

	p1->releaseButton(Jump);

	if(!self->isDual() || !self->isTwoPlayer()) return;
	
}


void(*UILayer_ccTouchMovedO)(UILayer *self, CCTouch *touch, CCEvent *event);
void UILayer_ccTouchMoved(UILayer *self, CCTouch *touch, CCEvent *event)
{
	
	UILayer_ccTouchMovedO(self, touch, event);

}

int playtest_touchID = -1;

	CCRect right_btn = {100, 0, 240, 100};
	CCRect left_btn = {0, 0, 240, 100};
	
bool(*EditorUI_ccTouchBeganO)(EditorUI *self, CCTouch *touch, CCEvent *event);
bool EditorUI_ccTouchBeganH(EditorUI *self, CCTouch *touch, CCEvent *event)
{
	//bool ret = EditorUI_ccTouchBeganO(self, touch, event);

void* unk = MBO(void*, self->_levelEditor(), 0x344);
bool is_platformer = MBO(bool, self->_levelEditor(), 0x344);


	
	CCLog("enter began");
	if (is_platformer)
	{
	CCLog("enter is Platformer");
		auto &touch_id = playtest_touchID;
		// already a touch going on, ignore multiple
		if (touch_id != -1) return EditorUI_ccTouchBeganO(self, touch, event);;

		auto touch_pos = touch->getLocation();
		touch_pos = self->convertToNodeSpace(touch_pos);

		// dont know if the order is right but doesnt matter
		
		CCLog("x: %f", touch_pos.x);
		CCLog("y: %f", touch_pos.y);
		

				if(right_btn.containsPoint(touch_pos)) {
				CCLog("enter right");

			touch_id = touch->_touchID();
			self->_levelEditor()->queueButton(3, true, false);
			return true;
		}
		
		if (left_btn.containsPoint(touch_pos))
		{
		CCLog("enter left");

			touch_id = touch->_touchID();
			self->_levelEditor()->queueButton(2, true, false);
			return true;
		} 


	}
	CCLog("finish began");
	return EditorUI_ccTouchBeganO(self, touch, event);
}

void(*EditorUI_ccTouchEndedO)(EditorUI *self, CCTouch *touch, CCEvent *event);
void EditorUI_ccTouchEnded(EditorUI *self, CCTouch *touch, CCEvent *event)
{
		CCLog("enter end");


void* unk = MBO(void*, self->_levelEditor(), 0x344);
bool is_platformer = MBO(bool, self->_levelEditor(), 0x344);

auto &touch_id = playtest_touchID;


	if (is_platformer && touch_id == touch->_touchID())
	{
		CCLog("enter platformer end");

		auto touch_pos = touch->getLocation();
		touch_pos = self->convertToNodeSpace(touch_pos);
		
		
		
		
		if (right_btn.containsPoint(touch_pos)) {
			touch_id = -1;
					CCLog("enter right");

			self->_levelEditor()->queueButton(3, false, false);
			return;
		}
		
		if (left_btn.containsPoint(touch_pos))
		{		
					CCLog("enter left");

			touch_id = -1;
			self->_levelEditor()->queueButton(2, false, false);
			return;
			
		}
		
	
	}
		EditorUI_ccTouchEndedO(self, touch, event);


}







bool(*isIconUnlockedO)(void *, int, int);
bool isIconUnlockedH(void *self, int a1, int a2)
{
	return true;

}

CCSprite * (*spriteCreateO)(const char *textureName);
CCSprite* spriteCreateH(const char *textureName)
{
	auto ret = spriteCreateO(textureName);

	if (contains(textureName, "GJ_square05.png"))
		return spriteCreateO("GJ_square04.png");

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
	auto m_sFileName = "password.dat";

	auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_sFileName;

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
	
	doRequest = true;
	GM->setGameVariable("0122", false);
	GM->setGameVariable("0023", false); //smooth fix
	GM->setGameVariable("0074", true);  //show restart button
	return "Italian APK Downloader\nCatto_\niAndy_HD3\nTr1NgleBoss";
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
			addToggle_trp(self, "Playtest as\nSave and Play", "100008", "Playtest button makes the save and play action");
			addToggle_trp(self, "Practice Music", "0125", 0);
			addToggle_trp(self, "Show Platformer Hitbox", "100009", 0);

			isGDPSSettings = false;
		}
	}
	else
	{
		addToggle_trp(self, title, code, desc);
	}
}

bool first = true;
bool(*MenuLayerInitO)(MenuLayer*);
//keeping this here until i start porting stuff from old project
bool MenuLayerInitH(MenuLayer *self)
{
	//CCLog(AY_OBFUSCATE("This apk belongs to TheMilkCat"));
	CCLog("Menu Init!");

	//	CCLog(AY_OBFUSCATE("APK Belongs to DM Group"));

	//string useless = GDPS->itos(1);
	//	self->runAction(CCCallFuncO::create(self, callfuncO_selector(CreatorLayer::onMyLevels), self));

	if (first)
	{
		if (GM->getGameVariable("100003"))
			self->runAction(CCCallFuncO::create(self, callfuncO_selector(CreatorLayer::onMyLevels), self));
		first = false;
	}
/*
	auto dir = CCDirector::sharedDirector();
	
	auto old_menu = CCMenu::create();
	auto oldSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_creatorBtn_001.png");
	//auto oldSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_profileButton_001.png");
	auto old_btn = CCMenuItemSpriteExtra::create(
		oldSprite,
		oldSprite,
		self,
		menu_selector(MenuLayer::onMyProfile));
	old_menu->addChild(old_btn, 100);
	old_btn->setPositionX(dir->getScreenLeft() + 50);
	old_btn->setPositionY(dir->getScreenBottom() + 110);
	old_menu->setPosition({0, 0});
	
	self->addChild(old_menu);
*/

	return MenuLayerInitO(self);

}

CCSprite * (*spriteCreateFrameNameO)(const char *textureName);
CCSprite* spriteCreateFrameNameH(const char *textureName)
{
	auto ret = spriteCreateFrameNameO(textureName);

	if (contains(textureName, "GJ_fullBtn_001.png"))
		return spriteCreateFrameNameO("GJ_creatorBtn_001.png");

	if (ret != nullptr)
		return ret;

	return spriteCreateFrameNameO("GJ_optionsTxt_001.png");
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

const char *(*CCString_getCStringO)(CCString*);
const char *CCString_getCStringH(CCString *self)
{
	auto ret = CCString_getCStringO(self);

	if (contains(ret, AY_OBFUSCATE("gjp2")) && !passwordTemp.empty())
	{
		auto AM = GJAccountManager::sharedState();
		const char *toAdd;

		if (contains(ret, "userName"))
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

	/*
			auto m_sFileName = "password.dat";

		  auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_sFileName;
    std::ifstream infile(path.c_str());
    if (infile.good())
    {
		CCLog("Normal load");
		string myText;
		while (getline (infile, myText))
			CCLog(myText.c_str());
    }
	else
	{ 
		CCLog("file not good");
	}

		*/

	return ret;

}

bool(*canPlayOnlineLevelsO)(CreatorLayer *self);
bool canPlayOnlineLevelsH(CreatorLayer *self)
{
	return true;
}

bool(*UILayerInitO)(UILayer *self);
bool UILayerInitH(UILayer *self)
{
	if(!UILayerInitO(self)) return false;

	if(MEMBERBYOFFSET(bool, self, 0x206)) {
		auto dpad = MEMBERBYOFFSET(CCSprite*, self, 0x1D8);

		dpad->setVisible(!GM->getGameVariable("10007"));
	}

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

	auto ret = addToGroupO(self, a2, a3, a4);

	//	CCLog(typeid(*(CCObject*)ret).name());

	return ret;
}
#include "AccountLoginLayer.h"

void *(*AccountSubmitO)(AccountLoginLayer *self, CCObject *a2, void *a3, void *a4);
void *AccountSubmitH(AccountLoginLayer *self, CCObject *a2, void *a3, void *a4)
{
	passwordTemp = self->_inputPassword()->getString();
	CCLog(passwordTemp.c_str());
	auto ret = AccountSubmitO(self, a2, a3, a4);

	return ret;

}

void *(*LoginFinishedO)(AccountLoginLayer *self, void *a2, void *a3);
void *LoginFinishedH(AccountLoginLayer *self, void *a2, void *a3)
{
	auto m_sFileName = "password.dat";

	auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_sFileName;

	ofstream MyFile(path.c_str());

	MyFile << passwordTemp;

	MyFile.close();

	return LoginFinishedO(self, a2, a3);

}

bool(*LevelInfoLayerInitO)(LevelInfoLayer *, GJGameLevel *, bool);
bool LevelInfoLayerInitH(LevelInfoLayer *self, GJGameLevel *level, bool a3)
{
	auto sprite = CCSprite::createWithSpriteFrameName("communityCreditsBtn_001.png");
	sprite->setScale(2);
	auto btn = CCMenuItemSpriteExtra::create(sprite, sprite, self, menu_selector(LevelInfoLayer::onClone));
	auto menu = CCMenu::create();
	menu->setPositionY(menu->getPositionY() - 50);
	menu->addChild(btn, 500);
	self->addChild(menu, 500);

	auto ret = LevelInfoLayerInitO(self, level, a3);
	return ret;

}

inline long mid_num(const std::string &s)
{
	return std::strtol(&s[s.find('_') + 1], nullptr, 10);
}

GameObject * (*GameObjectCreateO)(int key);
GameObject* GameObjectCreateH(int key)
{
	auto tb = ObjectToolbox::sharedState()->intKeyToFrame(key);
	
	if(key == 2013) return GameObjectCreateO(1);
	
	if(contains(tb, "pixelb"))
		return GameObjectCreateO(1);

	if(contains(tb, "pixel")) {
		
		if(contains(tb, "b_"))
			return GameObjectCreateO(1);

		auto pixelKey = mid_num(tb);
//
		return GameObjectCreateO(pixelKey > 140 ? 1 : key);
	}

	return GameObjectCreateO(key);
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


// moving funny loading text
bool(*LoadingLayer_initO)(LoadingLayer *, bool);
bool LoadingLayer_initH(LoadingLayer *self, bool fromReload)
{
	if (!LoadingLayer_initO(self, fromReload)) return false;
	
	auto text = *reinterpret_cast< CCNode **>(reinterpret_cast<uintptr_t> (self) + 0x144);
	text->setPositionY(text->getPositionY() - 10);

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
			FLAlertLayer::create(nullptr, "DISCLAIMER", "<cg>Pixel blocks</c > are <cr>not official</c > and for that reason levels containing these blocks < cr>will not look good in the official 2.2.</c>\n    <co>(textures will change)</c><cr>Don't use pixel blocks if you want your level to be playable in 2.2.</c>", "OK", nullptr, 500, false, 300)->show();
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
	/*
	if(type == ground) {
		
		//move buttons
		for(int i = 30; i < 34; i++) {
			
		auto btn = (CCNode*)menu->getChildren()->objectAtIndex(i);
		if(i == 30)
			btn->setPositionY(btn->GPY() - 30);
		else
			btn->setPositionX(btn->GPX() + 100);
		}
		
	}
	*/
	
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
		toRemove = 2;
		maxTextures = 2;
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
					frameSprStr = "bgIcon_01_001.png";


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
	
	
	
	
//	GDPSHelper::createLabels(menu, array, {0, 0}, true);
/*
	auto bgSelect = self->_bgSelectMenu();
	bgSelect->getChildren()->removeAllObjects();

	auto bgArray = self->_someArray();
	bgArray->removeAllObjects();

	auto label = CCSprite::createWithSpriteFrameName("bgIcon_03_001.png");
	auto test = CCMenuItemSpriteExtra::create(label, label, self, menu_selector(SelectArtLayer::selectArt));
	bgArray->addObject(test);
	bgSelect->addChild(test);

	auto label2 = CCLabelBMFont::create("Reverse", "bigFont.fnt");
	auto test2 = CCMenuItemSpriteExtra::create(label, label, self, menu_selector(SelectArtLayer::selectArt));
	test2->setPositionY(test2->GPY() + 30);
	bgArray->addObject(test2);
	bgSelect->addChild(test2);
*/
	/*
		int maxTextures = 25;	// not sure if this is right but even if its too much it won't crash

		if (type == SelectArtType::ground)
			maxTextures = 17;

		auto spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();

		for (int i = 0; i < maxTextures; i++)
		{
			auto bgBtn = (CCMenuItemSpriteExtra*)self->_bgSelectMenu()->getChildren()->objectAtIndex(i);

			if (bgBtn != nullptr)
			{
				const char *frameSprStr;

				if (type == SelectArtType::ground)
					frameSprStr = "gIcon_%02d_001.png";
				else
					frameSprStr = "bgIcon_%02d_001.png";

				auto frameStr = CCString::createWithFormat(frameSprStr, i + 1)->getCString();

				auto frameSpr = spriteCache->spriteFrameByName(frameStr);

				if (frameSpr != nullptr) {}
			}
		}

		if (type == SelectArtType::background)
		{
			auto okBtn = (CCMenuItemSpriteExtra*)self->_bgSelectMenu()->getChildren()->objectAtIndex(self->_bgSelectMenu()->getChildrenCount() - 1);

			okBtn->setPositionX(80);
			okBtn->setPositionY(okBtn->getPositionY() + 5);
		}

	*/
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
				desc += "\n- < cp>Follow p follows the players icon rotation. Some modes don't work with this.</c>";
				return infoButton(title, desc, a3);
				break;

			case 2067:	//scale
				desc += "\n- < cp>Scaling a singular object uses  the objects rotated x/y instead of grid x/y.</c>";
				return infoButton(title, desc, a3);
				break;

			case 1585:	//aninamte
				desc += "\n- < cp>Can also animate particle effects and animated object.</c>";
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
				return infoButton("Camera Guide", "Shows the position of the camera if the player was to spawn on the exact position of the Guide Trigger.\n < cg>The green line shows the entire visible area.</c>\n < cy>The yellow lines show where objects start to fade in and out.</c>", a3);
				break;

			case 1934:	//song trigger
				return infoButton(title, "<cy>Sets the time of the song in ms.</c>\n < cr>This means 1000ms = 1 second</c>", a3);
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
				desc += "\n < cg>Uses base and detail color. Does not work with hsv.</c>\n\
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

		if(self->_musicTime() == -1 || fabsf(bgMusicTime - self->_musicTime()) >= 100) {
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
	
	
	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*)layer->getChildren()->objectAtIndex(1);
	
	
	if(!isStartPos) {
		
		auto platformer_toggle = (CCMenuItemToggler*)menu->getChildren()->objectAtIndex(29);
		auto label = CCLabelBMFont::create("Platformer", "goldFont.fnt");
		label->setPosition(platformer_toggle->getPositionX() + 70, platformer_toggle->getPositionY());
		label->setScale(.6);
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
	//	GDPSHelper::createLabels(menu, menu->getChildren(), {0, 0}, true);
		
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
	
		if(font != 10)
		return updateFontLabelO(self, a2);

}


FUNCTIONHOOK(bool, validGroup, GameObject* obj, int group) {
	
	return true;
}

/*

#include <random>


FUNCTIONHOOK(bool, MPL_init, CCLayer* self) {
	
	    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,7); // distribution in range [1, 6]
	
	int n = dist6(rng);
	
	std::string desc = "";
	
	switch(n) {
	
	case 1:
		desc = "What did you expect to happen?";
	break;
	case 2:
	desc = "RubRub won't like this...";
	break;
	case 3:
	desc = "Are you sure this is a good idea?";
	break;
	case 4:
	desc = "Mutliplayer is not available... <cr>(?)</c>";
	break;
	case 5:
	desc = "Nice weather outside";
	break;
	case 6: 
	desc = "Multiplayer does not exist, stop trying!";
	break;
	case 7:
	desc = "Go play with the new editor while you wait for... <cr>(?)</c>";
	break;
	case 8:
	desc = "Test your luck?";

	
	}
	

    std::uniform_int_distribution<std::mt19937::result_type> dist4(1,50); // distribution in range [1, 6]
	
	int z = dist4(rng);
	CCLog("z: %d / n: %d", z, n);
	if(z == 25)
	desc = AY_OBFUSCATE("I might try to add this but don't tell anyone ok?\n :)");
	
	
		FLAlertLayer::create(nullptr, "It's a secret...", desc, "OK", nullptr, 400, false, 200)->show();
		

	return true;

}

//#include "ShaderLayer.h"
FUNCTIONHOOK(void*, triggerColorChange, CCLayer* self, float f1 ,float f2,float f3,float f4,float f5,float f6,float f7,int i1,float f8) {

	CCLog("enter triggerColorChange");
	CCLog("f1: %f", f1);
	CCLog("f2: %f", f2);
	CCLog("f3: %f", f3);
	CCLog("f4: %f", f4);
	CCLog("f5: %f", f5);
	CCLog("f6: %f", f6);
	CCLog("f7: %f", f7);
	CCLog("f8: %f", f8);
	CCLog("i1: %f", i1);
	
	/*
   *((float *)a2 + 0x13B),
    *((float *)a2 + 0x19C),
    *((float *)a2 + 0x19D),
    *((float *)a2 + 0x19B),
    *((float *)a2 + 0x19F),
    *((float *)a2 + 0x1A0),
	*((float *)a2 + 0x1A1),
    *((_DWORD *)a2 + 0x145),
	*((float *)a2 + 0x146));
	
	return triggerColorChangeO(self, f1, f2, f3, f4, f5, f6, f7, i1, f8);
	
	
}

FUNCTIONHOOK(bool, ShaderLayer_init, CCLayer* self) {
	
	CCLog("before init");
		int b = MBO(int, self, 0x41C);
	int c = MBO(int, self, 0x41C + 4);
	
	CCLog("b: %d", b);
	CCLog("c: %d", c);
	
	auto ret = ShaderLayer_initO(self);
	CCLog("init done");
	b = MBO(int, self, 0x41C);
	c = MBO(int, self, 0x41C + 4);
	
	CCLog("b: %d", b);
	CCLog("c: %d", c);
	CCLog("return ret");
	return ret;
	
	

	
	
}
*/
void loader()
{
	auto cocos2d = dlopen(targetLibName != "" ? targetLibName : NULL, RTLD_LAZY);

	//MenuLayerExt::ApplyHooks();
	EditLevelLayerExt::ApplyHooks();
	LevelEditorLayerExt::ApplyHooks();
	EditorPauseLayerExt::ApplyHooks();
	
	#ifdef SHADERDEBUG
	DevDebugHooks::ApplyHooks();
	#endif


//	HOOK("_ZN11ShaderLayer4initEv", ShaderLayer_initH, ShaderLayer_initO);
	//HOOK("_ZN11ShaderLayer18triggerColorChangeEfffffffif", triggerColorChangeH, triggerColorChangeO);
	//HOOK("_ZN16LevelEditorLayer14recreateGroupsEv", recreateGroupsH, recreateGroupsO);
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
	HOOK("_ZN15GJBaseGameLayer12addToSectionEP10GameObject", GJBaseGameLayer_addToSectionH, GJBaseGameLayer_addToSectionO);
	HOOK("_ZN15GJBaseGameLayer23removeObjectFromSectionEP10GameObject", GJBaseGameLayer_removeObjectFromSectionH, GJBaseGameLayer_removeObjectFromSectionO);

	HOOK("_ZN7UILayer4initEv", UILayer_initH, UILayer_initO);
	
	HOOK("_ZN10PauseLayer6onEditEPN7cocos2d8CCObjectE", PauseLayer_onEditH, PauseLayer_onEditO);
	HOOK("_ZN13EndLevelLayer6onEditEPN7cocos2d8CCObjectE", EndLevelLayer_onEditH, EndLevelLayer_onEditO);
	
//	HOOK("_ZN12PlayerObject15spawnDualCircleEv", PlayerObject_spawnDualCircleH, PlayerObject_spawnDualCircleO);
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
//	HOOK("_ZN11GameManager18toggleGameVariableEPKc", hook_onToggle, onToggleTrampoline);
	
	HOOK("_ZN13ObjectToolbox13intKeyToFrameEi", keyToFrameH, keyToFrameO);
	HOOK("_ZN8EditorUI4initEP16LevelEditorLayer", EditorUI_InitH, EditorUI_InitO);
	
	HOOK("_ZN8EditorUI13selectObjectsEPN7cocos2d7CCArrayEb", EditorUI_SelectObjectsH, EditorUI_SelectObjectsO);
	HOOK("_ZN8EditorUI12selectObjectEP10GameObjectb", EditorUI_SelectObjectH, EditorUI_SelectObjectO);
//	HOOK("_ZN10GameObject10setOpacityEh", GameObjectSetOpacityH, GameObjectSetOpacityO);
	HOOK("_ZN10GameObject13createWithKeyEi", GameObjectCreateH, GameObjectCreateO);
	//HOOK("_ZN14LevelInfoLayer4initEP11GJGameLevelb", LevelInfoLayerInitH, LevelInfoLayerInitO);	
	HOOK("_ZN12LoadingLayer4initEb", LoadingLayer_initH, LoadingLayer_initO);
		
	HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, "_ZN7UILayer12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE"),
		(void*) UILayer_ccTouchBeganH,
		(void **) &UILayer_ccTouchBeganO);
	HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, "_ZN7UILayer12ccTouchEndedEPN7cocos2d7CCTouchEPNS0_7CCEventE"),
		(void*) UILayer_ccTouchEnded,
		(void **) &UILayer_ccTouchEndedO);
		
		HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, "_ZN7UILayer12ccTouchMovedEPN7cocos2d7CCTouchEPNS0_7CCEventE"),
		(void*) UILayer_ccTouchMoved,
		(void **) &UILayer_ccTouchMovedO);
		
		HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, "_ZN8EditorUI16playerTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE"),
		(void*) EditorUI_ccTouchBeganH,
		(void **) &EditorUI_ccTouchBeganO);
	HookManager::do_hook(HookManager::getPointerFromSymbol(cocos2d, "_ZN8EditorUI16playerTouchEndedEPN7cocos2d7CCTouchEPNS0_7CCEventE"),
		(void*) EditorUI_ccTouchEnded,
		(void **) &EditorUI_ccTouchEndedO);

	HOOK("_ZN7cocos2d8CCSprite6createEPKc",
		spriteCreateH, spriteCreateO);
	HOOK("_ZN7cocos2d8CCSprite25createWithSpriteFrameNameEPKc",
		spriteCreateFrameNameH, spriteCreateFrameNameO);
		
	HOOK("_ZN16GameStatsManager14isItemUnlockedE10UnlockTypei",
		isIconUnlockedH, isIconUnlockedO);
	HOOK("_ZN9MenuLayer4initEv",
		MenuLayerInitH, MenuLayerInitO);
	HOOK("_ZN16MoreOptionsLayer9addToggleEPKcS1_S1_",
		addToggle_hk, addToggle_trp);
	HOOK("_ZN12OptionsLayer11customSetupEv",
		OptionsLayerInitH, OptionsLayerInitO);
	HOOK("_ZNK7cocos2d8CCString10getCStringEv",
		CCString_getCStringH, CCString_getCStringO);
		
		/*
	HOOK("_ZN16GameLevelManager18ProcessHttpRequestESsSsSs10GJHttpType",
		LevelProcessH, LevelProcessO);
	HOOK("_ZN20MusicDownloadManager18ProcessHttpRequestESsSsSs10GJHttpType",
		MusicProcessH, MusicProcessO);
	HOOK("_ZN16GJAccountManager18ProcessHttpRequestESsSsSs10GJHttpType",
		AccountProcessH, AccountProcessO);
		
		*/
		
		
	HOOK("_ZN12CreatorLayer19canPlayOnlineLevelsEv",
		canPlayOnlineLevelsH, canPlayOnlineLevelsO);
	/*
	HOOK("_ZN16LevelEditorLayer15getTriggerGroupEi",
		getTriggerGroupH, getTriggerGroupO);
		*/
		
//	HOOK("_ZN7UILayer4initEv",
	//	UILayerInitH, UILayerInitO);
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

	patch *tmp = new patch();
	tmp->addPatch("libcocos2dcpp.so", 0x26DB2E, "00 bf 00 bf");

	//playtest
	tmp->addPatch("libcocos2dcpp.so", 0x2B8896, "00 bf 00 bf");
	tmp->addPatch("libcocos2dcpp.so", 0x2B88A4, "00 bf 00 bf");
	tmp->addPatch("libcocos2dcpp.so", 0x2B893C, "00 bf");

	//gradient bypass
	tmp->addPatch("libcocos2dcpp.so", 0x385134, "FF");
	
	
//	tried to fix colors and opacity
    tmp->addPatch("libcocos2dcpp.so", 0x2A8AD2, "00 bf");
    tmp->addPatch("libcocos2dcpp.so", 0x2A8AD8, "00 bf");
    tmp->addPatch("libcocos2dcpp.so", 0x2A88EC, "00 bf 00 bf");
	


	//test fix colors
	tmp->addPatch("libcocos2dcpp.so", 0x2CA80E, "00 bf");
	tmp->addPatch("libcocos2dcpp.so", 0x2CA81A, "00 bf");
	tmp->addPatch("libcocos2dcpp.so", 0x2CA758, "00 bf");

	tmp->addPatch("libcocos2dcpp.so", 0x2CA7D6, "11 e0");
	
	tmp->addPatch("libcocos2dcpp.so", 0x37BB72, "00 bf 00 bf");

	//testing editor shit
	//tmp->addPatch("libcocos2dcpp.so", 0x2C2D84, "00 bf 00 bf");

	//ads
	tmp->addPatch("libcocos2dcpp.so", 0x26E970, "00 BF 00 BF");
	tmp->addPatch("libcocos2dcpp.so", 0x34478C, "00 BF 00 BF");
	tmp->addPatch("libcocos2dcpp.so", 0x346CC4, "00 BF 00 BF");
	tmp->addPatch("libcocos2dcpp.so", 0x26E976, "00 BF 00 BF");
	tmp->addPatch("libcocos2dcpp.so", 0x2E6960, "00 BF 00 BF");
	tmp->addPatch("libcocos2dcpp.so", 0x2E7968, "00 BF 00 BF");
	tmp->addPatch("libcocos2dcpp.so", 0x27BEB0, "00 BF 00 BF");
	tmp->addPatch("libcocos2dcpp.so", 0x27BF7E, "00 BF 00 BF");

	//gauntlets (crash after exiting)
	tmp->addPatch("libcocos2dcpp.so", 0x2E95A8, "00 BF 00 BF");

	// versus button shit
	tmp->addPatch("libcocos2dcpp.so", 0x2E95D2, "00 BF 00 BF");

	//shit
	//	tmp->addPatch("libcocos2dcpp.so", 0x2B8828, "00 BF 00 BF");

	//remove tos popup
	//tmp->addPatch("libcocos2dcpp.so", 0x26C15C, "00 BF 00 BF");

	// make sure playtest background doesn't move
	tmp->addPatch("libcocos2dcpp.so", 0x2BC360, "01 22");

	//tmp->Modify();
	
	
	
	
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
	

	// playtest
	NOP4(tms, 0x2BC876);
	NOP4(tms, 0x2BC884);
	
	
//	NOP4(tms, 0x2EDA9E); //versus
	NOP4(tms, 0x2EDA74); //gauntlets
	
	
	
//	tms->addPatch("libcocos2dcpp.so", 0x81121D, "68 74 74 70 3A 2F 2F 77 77 77 2E 62 6F 6F 6D 6C 69 6E 67 73 2E 63 6F 6D 2F 64 61 74 61 62 61 73 65 2F 67 65 74 47 4A 53 6F 6E 67 49 6E 66 6F 2E 70 68 70");
	
	
	tms->addPatch("libcocos2dcpp.so", 0x2C0384, "01 22"); // fix bg
	
	tms->addPatch("libcocos2dcpp.so", 0x2EB9EE, "01 21"); // fix level name in pause

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

//open with adb
//call adb shell am start -n com.gdpsedi.geometrydashsubzero/com.robtopx.geometryjumplite.GeometryDashLite
