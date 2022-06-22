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

#define targetLibName ("libcocos2dcpp.so")
#define contains(x, y) strstr(x, y) != NULL

#include <typeinfo>
#include <memory>
#include <cxxabi.h>

string passwordTemp = "";
bool shouldAdd = true;

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}


//https://discord.com/channels/646101505417674758/651480005536383009/988422971993501726
//thanks cvolton

std::string gjp(std::string password) {
	
const char* key = "37526";
const size_t keyLen = 5;
size_t gjpCurrent = 0;
	for(auto& character : password) {
		character ^= key[gjpCurrent];
		gjpCurrent = (gjpCurrent + 1) % keyLen;
	}
	return base64_encode((const unsigned char *)password.c_str(), password.length());
}





std::string get_name_for_node(cocos2d::CCObject* node)
{
    // stolen from mat (thanks mat)
    // and also stolen from https://stackoverflow.com/a/4541470

    auto name = typeid(*node).name();
    auto status = -1;

    std::unique_ptr<char, void (*)(void*)> res {
            abi::__cxa_demangle(name, nullptr, nullptr, &status),
            std::free
    };

    return (status == 0) ? res.get() : name;
}


void *getPointerFromSymbol(void *handle, const char *symbol)
{
    return reinterpret_cast<void *>(dlsym(handle, symbol));
}

bool (*EditLevelLayer_initO)(EditLevelLayer* self,GJGameLevel * level);
bool EditLevelLayer_initH(EditLevelLayer* ptr,GJGameLevel * level){
    auto ret = EditLevelLayer_initO(ptr, level);

    auto dir = cocos2d::CCDirector::sharedDirector();
    if (dir->getScheduler()->_fTimeScale != 1)
    {
        dir->getScheduler()->_fTimeScale = 1;
    }

    auto menu = ptr->_btnMenu();
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
    return ret;
}
class events : public EditLevelLayer {
public:
    void onCLick(){
        LOGD("ENTER! onClick!");

        this->closeTextInputs();
        *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(GameManager::sharedState()) + 0x1BC) = 3;
        this->verifyLevelName();

        auto dir = cocos2d::CCDirector::sharedDirector( );
        auto layer = LevelEditorLayer::create( this->gameLevel_ ,false);
        auto sc = CCScene::create();
        sc->addChild(layer);

        auto scene = cocos2d::CCTransitionFade::create(
                0.5,sc
        );

        dir->replaceScene( scene );


    }
};

void createArrayAndAssign(LevelEditorLayer* p,int offset){
    auto array = CCArray::create();
    *((int *)p + offset) = reinterpret_cast<int>(array);
    array->retain();
}

bool (*editor_callback_trp)( LevelEditorLayer* p, GJGameLevel* level );
bool editor_callback( LevelEditorLayer* p, GJGameLevel* level )
{
    if ( !dynamic_cast< GJBaseGameLayer* >( p )->init( ) )
        return false;

    auto gm = GameManager::sharedState( );
    *((bool *)gm + 442) = true;
    p->ignoreDamage_ = gm->getGameVariable( "0009");
    p->unk_bool_01 = gm->getGameVariable( "0001");
    p->unk_bool_02 = false; //orbs
    p->unk_bool_03 = gm->getGameVariable( "0045");
    p->unk_bool_04 = gm->getGameVariable( "0038");
    p->unk_bool_06 = gm->getGameVariable( "0043");
    p->unk_bool_07 = gm->getGameVariable( "0037");
    p->unk_bool_08 = gm->getGameVariable( "0058");
    p->unk_bool_09 = gm->getGameVariable( "0013");
    p->unk_bool_10 = gm->getGameVariable( "0036");
    p->unk_bool_10 = gm->getGameVariable( "0078");
    p->unk_bool_11 = gm->getGameVariable( "0120");
    p->unk_bool_05 = gm->getGameVariable( "0079");
    p->unk_bool_12 = gm->getGameVariable( "0103");
    p->unk_bool_13 = gm->getGameVariable( "0104");

    p->setObjectCount( 0 );

    GSM->stopBackgroundMusic( );

    p->_level = level;
    p->_level->retain();

    gm->playLayer_ = p;
    p->retain();
    p->arr_03 = CCArray::create();
    p->arr_03->retain();
    p->arr_01 = CCArray::create();
    p->arr_01->retain();
    p->arr_02 = CCArray::create();
    p->arr_02->retain();




    p->arr_2848 = CCArray::create();
    p->arr_2848->retain();
    p->arr_2832 = CCArray::create();
    p->arr_2832->retain();
    p->arr_2833 = CCArray::create();
    p->arr_2833->retain();
    p->arr_2802 = CCArray::create();
    p->arr_2802->retain();
    p->arr_2803 = CCArray::create();
    p->arr_2803->retain();
    p->arr_2863 = CCArray::create();
    p->arr_2863->retain();
    p->arr_2816 = CCArray::create();
    p->arr_2816->retain();
    p->arr_2795 = CCArray::create();
    p->arr_2795->retain();
    p->arr_2805 = CCArray::create();
    p->arr_2805->retain();
    p->arr_2799 = CCArray::create();
    p->arr_2799->retain();
    p->arr_2800 = CCArray::create();
    p->arr_2800->retain();
    p->arr_2797 = CCArray::create();
    p->arr_2797->retain();
    p->arr_2796 = CCArray::create();
    p->arr_2796->retain();
    p->arr_2798 = CCArray::create();
    p->arr_2798->retain();
    p->arr_2801 = CCArray::create();
    p->arr_2801->retain();
    p->arr_2818 = CCArray::create();
    p->arr_2818->retain();
    p->arr_2807 = CCArray::create();
    p->arr_2807->retain();
    p->arr_2821 = CCArray::create();
    p->arr_2821->retain();
	
    p->_crashArray1() = CCArray::create();
    p->_crashArray1()->retain();
	
	p->_stickyGroupsDict() = CCDictionary::create();
    p->_stickyGroupsDict()->retain();
	


    *((bool *)p + 316) = *((bool *)level + 393);


    p->vector_2868.reserve( 9999 );
    p->vector_2858.reserve( 9999 );
    p->vector_2878.reserve( 9999 );
    p->vector_2853.reserve( 9999 );
    p->vector_2873.reserve( 9999 );
    p->vector_2881.reserve( 9999 );
    p->_something().reserve( 9999 );
    p->objectsVector_.reserve( 9999 );


    //p->objectVector_.reserve( 9999 );


    for ( size_t i = 0; i < 9999; ++i )
    {
        p->objectsVector_[ i ] = nullptr;


        p->vector_2853[ i ] = 0;
        p->vector_2873[ i ] = 0;
        p->vector_2868[ i ] = false;
        p->vector_2858[ i ] = false;
        //*(bool *)(*((int *)p + 2878) + i) = 0;
        p->vector_2878[ i ] = 0;
        p->vector_2881[ i ] = 0;
        p->_something()[i] = 0;
    }


    p->obb2d2_ = OBB2D::create( CCPoint( 1, 1 ), 1.0, 1.0, 0.0 );
    p->obb2d2_->retain( );
    p->weirdAlloc = new int[ 0xC80 ];
    p->setupLayers( );

    p->gridLayer_ = DrawGridLayer::create( p->gameLayer_, p );

    p->gameLayer_->addChild( p->gridLayer_, gm->getGameVariable( "0039" ) ? 99 : -100 );

    p->createPlayer();
    *(bool *)(*((int *)p + 204) + 1556) = false;
    *(bool *)(*((int *)p + 205) + 1556) = false;

    if(GM->getGameVariable("0114")){
        *(bool *)(*((int *)p + 204) + 3008) = true;
        *(bool *)(*((int *)p + 205) + 3008) = true;
    }

    p->unk_arr_209 = CCArray::create();
    p->unk_arr_209->retain();
    p->unk_arr_211 = CCArray::create();
    p->unk_arr_211->retain();
    p->arr_260 = CCArray::create();
    p->arr_260->retain();


    p->dCross_ = CCSprite::createWithSpriteFrameName( "d_cross_01_001.png" );
    p->batchNode_->addChild( p->dCross_, 10 );
    p->dCross_->setColor( ccc3( 255, 0, 0 ) );
    p->dCross_->setVisible( false );
    p->dCross_->setScale( 0.7 );


    std::string a = ZipUtils::decompressString( std::string( ( char* ) p->_level->levelString_ ), false, 11 );

    p->levelString_ = new std::string( a );

    p->createObjectsFromSetup( a );

    p->createTextLayers();
    if ( GameManager::sharedState()->getGameVariable( "0066") == 1 )
        p->enableHighCapacityMode();

    if ( !p->levelSettings_ )
    {
        p->levelSettings_ = LevelSettingsObject::create( );
        p->levelSettings_->gameLevel_ = p->_level;

        p->levelSettings_->retain( );
    }
    p->editorUI_ = EditorUI::create( p );
    p->addChild( p->editorUI_, 100 );


    //put the editor in the last position/zoom
    p->gameLayer_->setPosition(p->_level->lastCameraPos_);
    if(p->_level->lastEditorZoom != 0.0)
        p->gameLayer_->setScale(p->_level->lastEditorZoom);


    p->editorUI_->updateSlider();
    p->createGroundLayer(true,true);
    p->gridLayer_->updateTimeMarkers();
    p->createBackground(0);
    p->createBackground(1);
    p->editorUI_->updateSlider( );

    p->resetGroupCounters(false);
	p->sortStickyGroups();
    p->updateEditorMode();

    p->schedule( schedule_selector( LevelEditorLayer::updateEditor ));

    //fix to the glitched background with updateEditor
    p->editorUI_->onStopPlaytest(nullptr);

    return true;
}


bool (*setUpLevelInfo)(EditLevelLayer *, GJGameLevel *);
bool setUpLevelInfo_hk(EditLevelLayer *ptr, GJGameLevel *level)
{

    auto ret = setUpLevelInfo(ptr, level);


//*(_DWORD *)(GameManager::sharedState(v40) + 360

    auto menu = ptr->_btnMenu();

    // creator btn

    auto editBtn = (CCMenuItemSpriteExtra *)menu->getChildren()->objectAtIndex(0);

    auto editBtnCustom = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png"), nullptr, ptr,
                                                       menu_selector(events::onCLick));
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

    return ret;
};



void (*GameManager_tryShowAdO)();
void GameManager_tryShowAdH(){

}
bool test = false;
void (*LevelEditorLayer_updateVisibilityO)(LevelEditorLayer*,float);
void LevelEditorLayer_updateVisibilityH(LevelEditorLayer* p,float a1){

	
    //((PlayLayer*)p)->updateVisibility(a1);
	
if(!GM->getGameVariable("100004")) {

    auto node = *((CCNode **)p + 284);
    auto position = node->getPosition();
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto scale = node->getScale();
    position.x = -(position.x * (1 / scale));
    position.y = -(position.y * (1 / scale));


    auto rect = CCRect(
            position.x,
            position.y,
            (float)(winSize.width / scale) + 30.0f,
            (float)(winSize.height / scale) + 30.0f);


    float centerX = rect.origin.x;
    int startIndex = (int)(float)(floorf(centerX / 100.0) - 1.0);
    float percentage = (float)(centerX + rect.size.width) / 100.0;
    int limit = (int)(float)(percentage + 1.0);
    auto sections = p->sections;

    for(int i = startIndex;  i <= limit; i++ ){
        if(i >= 0 && i < sections->count()){
            auto section = sections->objectAtIndex(i);
            if(section){
                auto sect = reinterpret_cast<CCArray*>(section);

                if (sect) {
					p->updateObjectColors(sect); 
                    if (sect->count() > 0) {
                        for (int k = 0; k < sect->count(); k++) {
                            GameObject *obj = dynamic_cast<GameObject *>(sect->objectAtIndex(k));
                            auto objectPos = obj->getPosition();
                            if(rect.containsPoint(objectPos)){
								auto selected = p->editorUI_->getSelectedObjects();
								
								for (int i = 0; i < selected->count(); i++)
								{
									auto object = (GameObject*)selected->objectAtIndex(i);	
									object->updateMainColor({0, 255, 0});
								}
								
                                obj->addMainSpriteToParent(false);
                                if(obj->hasSecondaryColor()) obj->addColorSpriteToParent(true);
                                // UNFINISHED!!!!!!!!!!
                            }
                            /*else{
                                if(obj->isVisible()){
                                    obj->setVisible(false);
                                    obj->retain();
                                    p->gameLayer_->removeChild(obj);
                                }
                            }*/
                        }
                    }
                }
            }

        }
    }
    p->processAreaVisualActions();
    p->sortBatchnodeChildren(0.0);
}
}
void (*CCSpriteFrameCache_spriteFrameByNameO)(CCSpriteFrameCache* self,const char* a1);
void CCSpriteFrameCache_spriteFrameByNameH(CCSpriteFrameCache* self,const char* a1){
    auto v2 = a1;
    if(strstr(v2,"pixel"))
        v2 = "d_spikes_01_001.png";
    return CCSpriteFrameCache_spriteFrameByNameO(self,v2);
}

void (*EditorPauseLayer_onResumeO)(EditorPauseLayer* self,CCObject* a1);
void EditorPauseLayer_onResumeH(EditorPauseLayer* p,CCObject* a1){
    auto gm = GameManager::sharedState();
    *((bool *)p + 770) = gm->getGameVariable( "0009");
    *((bool *)p + 11600) = gm->getGameVariable( "0001");
  //  *((bool *)p + 11601) = gm->getGameVariable( "0044");
    *((bool *)p + 11602) = gm->getGameVariable( "0045");
    *((bool *)p + 11603) = gm->getGameVariable( "0038");
    *((bool *)p + 11605) = gm->getGameVariable( "0043");
    *((bool *)p + 11606) = gm->getGameVariable( "0037");
    *((bool *)p + 11607) = gm->getGameVariable( "0058");
    *((bool *)p + 11608) = gm->getGameVariable( "0013");
    *((bool *)p + 11809) = gm->getGameVariable( "0036");
    *((bool *)p + 11609) = gm->getGameVariable( "0078");
    *((bool *)p + 11610) = gm->getGameVariable( "0120");
    *((bool *)p + 11604) = gm->getGameVariable( "0079");
    *((bool *)p + 11611) = gm->getGameVariable( "0103");
    *((bool *)p + 11612) = gm->getGameVariable( "0104");

    if(p->_editorLayer()->editorUI_) p->_editorLayer()->editorUI_->toggleStickyControls(gm->getGameVariable("0097"));

    p->removeFromParentAndCleanup(true);
    //(*(int (__fastcall **)(EditorPauseLayer *, int))(p + 256))(p, 1);
}

void (*EditorPauseLayer_removeO)(LevelEditorLayer* self,GameObject *,bool);
void EditorPauseLayer_removeH(LevelEditorLayer* p,GameObject * obj,bool a1){
    EditorPauseLayer_removeO(p,obj,a1);
    obj->removeFromParent();
}





void (*GJBaseGameLayer_addToSectionO)(GJBaseGameLayer* self,GameObject* obj);
void GJBaseGameLayer_addToSectionH(GJBaseGameLayer* self,GameObject* obj){
    if(obj)
        GJBaseGameLayer_addToSectionO(self,obj);
}

void (*GJBaseGameLayer_removeObjectFromSectionO)(GJBaseGameLayer* self,GameObject* obj);
void GJBaseGameLayer_removeObjectFromSectionH(GJBaseGameLayer* self,GameObject* obj){
	if(obj) {
        GJBaseGameLayer_removeObjectFromSectionO(self, obj);
	    if(*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(GameManager::sharedState()) + 442)) obj->removeFromParent();
    }
}

bool (*UILayer_ccTouchBeganO)(UILayer* self, CCTouch* touch, CCEvent* event);
bool UILayer_ccTouchBeganH(UILayer* self, CCTouch* touch, CCEvent* event) {
	
	bool ret = UILayer_ccTouchBeganO(self, touch, event);

	// println("touch {} {}", touch->getID(), from<int>(touch, 0x30));
	
	bool is_platformer = self->_platformer();
	if (is_platformer) {
		auto& touch_id = self->_touchID();
		// already a touch going on, ignore multiple
		if (touch_id != -1) return ret;

		auto touch_pos = touch->getLocation();
		touch_pos = self->convertToNodeSpace(touch_pos);

		// dont know if the order is right but doesnt matter
		auto left_btn = self->_leftBtn();
		auto right_btn = self->_rightBtn();
		if (!left_btn.containsPoint(touch_pos) && !right_btn.containsPoint(touch_pos)) {
			touch_id = touch->_touchID();
			GameManager::sharedState()->_playLayer()->queueButton(1, true, false);
		}
	}

	return ret;
}



void (*UILayer_ccTouchEndedO)(UILayer* self, CCTouch* touch, CCEvent* event);
void UILayer_ccTouchEnded(UILayer* self, CCTouch* touch, CCEvent* event) {
	
	UILayer_ccTouchEndedO(self, touch, event);
	
	bool is_platformer = self->_platformer();
	auto& touch_id = self->_touchID();
	if (is_platformer && touch_id == touch->_touchID() ) {
		touch_id = -1;
		GameManager::sharedState()->_playLayer()->queueButton(1, false, false);
	}
}

bool (*isIconUnlockedO)(void*, int, int);
bool isIconUnlockedH(void* self, int a1, int a2) {
	
	return true;

}

CCSprite* (*spriteCreateO)(const char* textureName);
CCSprite* spriteCreateH(const char* textureName) {

	auto ret = spriteCreateO(textureName);
	
	if(contains(textureName,"GJ_square05.png"))
	return spriteCreateO("GJ_square04.png");
	
	if(ret != nullptr) 
	return ret;



	return spriteCreateO("pixel.png");

}

bool isGDPSSettings;



void (*OptionsLayerInitO)(OptionsLayer* self);
void OptionsLayerInitH(OptionsLayer* self) {
	
	
		auto dir = CCDirector::sharedDirector();
		auto midx = dir->getScreenRight() / 2;
		auto midy = dir->getScreenTop() / 2;
	auto old_menu = CCMenu::create();
	auto layer = reinterpret_cast<CCLayer*>(self->getChildren()->objectAtIndex(0));

	auto oldSprite = cocos2d::CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
	auto old_btn = CCMenuItemSpriteExtra::create(
		oldSprite,
		oldSprite,
		self,
		menu_selector(OptionsLayer::onGDPSSettings));
	old_menu->addChild(old_btn, 100);
	old_btn->setPositionX(midx - 110);
	old_btn->setPositionY(midy);
	old_menu->setPosition({0, 0});
	layer->addChild(old_menu, 100);
	
	return OptionsLayerInitO(self);
}


const char* (*getStringO)(LoadingLayer* self);
const char* getStringH(LoadingLayer* self) {
	
		auto m_sFileName = "password.dat";
	

    auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_sFileName;
	
	  std::ifstream infile(path.c_str());
    if (infile.good())
    {
		string myText;
		while (getline (infile, myText))
			passwordTemp = myText;
		CCLog("password done");
    }
	else
	{ 
		passwordTemp = "0";
		CCLog("no file found");
	}
	
	GM->setGameVariable("0122", false);
	return "Catto_\nItalian APK Downloader\niAndy_HD3";
}
	
	

void (*addToggle_trp)(MoreOptionsLayer * self,const char * title, const char * code, const char * desc);
void addToggle_hk(MoreOptionsLayer* self, const char * title, const char * code, const char * desc) {
	
	if (isGDPSSettings)
    {
        if (strcmp(title, "Show Orb Guide") == 0)
        {
            addToggle_trp(self, "Robtop Servers", "100001", "Change servers to robtop servers");
            addToggle_trp(self, "Force Platformer", "100002", "All levels are platformer mode");
			addToggle_trp(self, "Instantly open editor\nat startup", "100003", 0);
			addToggle_trp(self, "Disable editor\nobject visibility", "100004", 0);
            isGDPSSettings = false;
        }
    }
    else
    {
        addToggle_trp(self, title, code, desc);
    }
}
bool first = true;
bool (*MenuLayerInitO)(MenuLayer*);
bool MenuLayerInitH(MenuLayer* self) {
	
	//CCLog(AY_OBFUSCATE("This apk belongs to TheMilkCat"));
	CCLog("Menu Init!");
	//string useless = GDPS->itos(1);
		//	self->runAction(CCCallFuncO::create(self, callfuncO_selector(CreatorLayer::onMyLevels), self));
			
	if(first) {
		if(GM->getGameVariable("100003"))
			self->runAction(CCCallFuncO::create(self, callfuncO_selector(CreatorLayer::onMyLevels), self));
	first = false;
	}
		

	auto dir = CCDirector::sharedDirector();
	auto old_menu = CCMenu::create();
	auto oldSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_profileButton_001.png");
	auto old_btn = CCMenuItemSpriteExtra::create(
		oldSprite,
		oldSprite,
		self,
		menu_selector(MenuLayer::onMyProfile));
	old_menu->addChild(old_btn, 100);
	old_btn->setPositionX(dir->getScreenLeft() + 50);
	old_btn->setPositionY(dir->getScreenBottom() + 110);
	old_menu->setPosition({0, 0});
	
	/*
	auto sprite = CCSprite::createWithSpriteFrameName("communityCreditsBtn_001.png");
	sprite->setScale(2);
	auto btn = CCMenuItemSpriteExtra::create(sprite, sprite, self, menu_selector(CreatorLayer::onMyLevels));
	auto menu = CCMenu::create();
	menu->setPositionY(menu->getPositionY() - 50);
	menu->addChild(btn, 500);
	self->addChild(menu, 500);
	*/
	
	
	self->addChild(old_menu, 900);
	
return MenuLayerInitO(self);

}

CCSprite* (*spriteCreateFrameNameO)(const char* textureName);
CCSprite* spriteCreateFrameNameH(const char* textureName) {
	
		// CCLog(textureName);

	
	auto ret = spriteCreateFrameNameO(textureName);
	
	if(contains(textureName, "GJ_fullBtn_001.png"))
	return spriteCreateFrameNameO("GJ_creatorBtn_001.png");

	if(ret != nullptr)
	return ret;



	return spriteCreateFrameNameO("GJ_optionsTxt_001.png");

}


#include "cocos2dx/extensions/network/HttpClient.h"
#include "cocos2dx/extensions/network/HttpRequest.h"
#include "cocos2dx/extensions/network/HttpResponse.h"
#include "obfuscate.h"

const char* gdpseditor = AY_OBFUSCATE("http://game.gdpseditor.com/server");
const char* boomlings = AY_OBFUSCATE("http://www.boomlings.com/database");

//epic servers obfuscate moment

inline string replaceServers(std::string original) {
			
		for(int i = 0; i < strlen(gdpseditor); i++)
          original[i] = gdpseditor[i];

      return original;
    }
	
	
void* (*LevelProcessO)(GameManager* gm, string a1, string a2, string a3, int a4);
void* LevelProcessH(GameManager* gm, string a1, string a2, string a3, int a4)
{	
		
	return LevelProcessO(gm, replaceServers(a1), a2, a3, a4);
}

void* (*MusicProcessO)(void* idk, string a1, string a2, string a3, int a4);
void* MusicProcessH(void* idk, string a1, string a2, string a3, int a4)
{	
	return MusicProcessO(idk, replaceServers(a1), a2, a3, a4);
}

void* (*AccountProcessO)(void* idk, string a1, string a2, string a3, int a4);
void* AccountProcessH(void* idk, string a1, string a2, string a3, int a4)
{	
	return AccountProcessO(idk, replaceServers(a1), a2, a3, a4);
}

const char *(*CCString_getCStringO)(CCString *);
const char *CCString_getCStringH(CCString *self)
{
	auto ret = CCString_getCStringO(self);
	
		if(contains(ret, AY_OBFUSCATE("gjp2")) && !passwordTemp.empty() && shouldAdd) {
			
			

			
			auto AM = GJAccountManager::sharedState();
			const char* valid = AY_OBFUSCATE("2022");
			const char *toAdd;


		if(AM->_username().empty() || contains(ret, AY_OBFUSCATE("userName")))
		toAdd = CCString::createWithFormat("&password=%s&gjp=%s&", passwordTemp.c_str(), gjp(passwordTemp).c_str())->getCString();
		else
		toAdd = CCString::createWithFormat("&password=%s&gjp=%s&userName=%s", passwordTemp.c_str(), gjp(passwordTemp).c_str(), AM->_username().c_str())->getCString();
	
		//char *s = new char[strlen(ret) + strlen(toAdd) + strlen(toAdd2) + 1];
		char *s = new char[strlen(ret) + strlen(toAdd) + 1];
		strcpy(s, ret);
		strcat(s, toAdd);
		//strcat(s, toAdd2);
		
		CCLog(s);

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



bool (*canPlayOnlineLevelsO)(CreatorLayer* self);
bool canPlayOnlineLevelsH(CreatorLayer* self){
	return true;
}

bool (*UILayerInitO)(UILayer* self);
bool UILayerInitH(UILayer* self){
	
	return UILayerInitO(self);
}

LevelSettingsObject* (*objectfromDictO)(LevelSettingsObject*, CCDictionary* keys);
LevelSettingsObject* objectfromDictH(LevelSettingsObject* a1, CCDictionary* keys) {
	
	if(GM->getGameVariable("100002")) {
		auto ret = objectfromDictO(a1, keys);
		*((bool *)ret + 275) = true; 	//platformer - 0x113
		return ret;
	}
	auto ret = objectfromDictO(a1, keys);
	return ret;
}

CCArray* (*getTriggerGroupO)(LevelEditorLayer* self, int a1);
CCArray* getTriggerGroupH(LevelEditorLayer* self, int a1) {
	
	//	CCLog("Trigger group func %d", a1);

	//return getTriggerGroupO(self, a1);

	CCArray* a = CCArray::create();
	return a;
	
	
	/*
	auto ret = getTriggerGroupO(self, a1);
		CCLog("Trigger group func 2 %d", a1);

	CCLog(typeid(ret).name());
	return getTriggerGroupO(self, a1);
	*/
	
}

void* (*addToGroupO)(GJBaseGameLayer* self, GameObject* a2, int a3,  bool a4);
void* addToGroupH(GJBaseGameLayer* self, GameObject* a2, int a3,  bool a4) {
	
	*((bool *)a2 + 1157) = false;
	
	auto ret = addToGroupO(self, a2, a3, a4);

	//	CCLog(typeid(*(CCObject*)ret).name());

	return ret;
}


void *(*buildHelperO)(EditorUI *self, bool a2);
void *buildHelperH(EditorUI *self, bool a2)
{
    auto ret = buildHelperO(self, a2);

    auto selected = self->getSelectedObjects();
    CCLog("enter loop");
    for (int i = 0; selected->count() > i; i++)
    {
		CCNode* node = reinterpret_cast<CCNode*>(selected->objectAtIndex(i));
		const char* type = typeid(*(CCObject*)node).name();
		CCLog(type);
        if(contains(type, "Effect")) {
			CCLog("true");
			node->removeFromParent();
			
		}
		
    }

    return ret;

}
#include "AccountLoginLayer.h"

void *(*AccountSubmitO)(AccountLoginLayer* self, CCObject* a2, void* a3, void* a4);
void *AccountSubmitH(AccountLoginLayer* self, CCObject* a2, void* a3, void* a4)
{
	
	passwordTemp = self->_inputPassword()->getString();
	CCLog(passwordTemp.c_str());
    auto ret = AccountSubmitO(self, a2, a3, a4);


    return ret;

}

void *(*LoginFinishedO)(AccountLoginLayer* self, void* a2, void* a3);
void *LoginFinishedH(AccountLoginLayer* self, void* a2, void* a3) 
{
		
	auto m_sFileName = "password.dat";
	
	    auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_sFileName;

		ofstream MyFile(path.c_str());

		MyFile << passwordTemp;

		MyFile.close();

    return LoginFinishedO(self, a2, a3);

}

bool (*LevelInfoLayerInitO)(LevelInfoLayer*, GJGameLevel*, bool);
bool LevelInfoLayerInitH(LevelInfoLayer* self, GJGameLevel* level, bool a3) {
	
	
	
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

GameObject* (*GameObjectCreateO)(int key);
GameObject *GameObjectCreateH(int key)
{
	auto tb = ObjectToolbox::sharedState()->intKeyToFrame(key);

    // fix missing textures
    if(std::string(tb).length() < 1) return GameObjectCreateO(1);

	CCLog("GAMEOBJECT CREATED: %s", tb);
	/*if (strstr(tb, "pixelart") != NULL && strstr(tb, "b_001") == NULL)
	{
		auto pixelKey = mid_num(tb);
		// LOGD("ART: %i", pixelKey);

		return GameObjectCreateO(pixelKey > 140 ? 1 : key);
	}

	if (!strcmp(tb, "pixelb_03_01_001.png"))
		return GameObjectCreateO(1);

	if (!strcmp(tb, "pixelb_03_02_001.png"))
		return GameObjectCreateO(1);
    */
	return GameObjectCreateO(key);
}

void (*downloadLevelO)(LevelInfoLayer* self);
void downloadLevelH(LevelInfoLayer* self) {
	
	shouldAdd = false;
	downloadLevelO(self);
	shouldAdd = true;
}

/*
void (*dict1)(CCDictionary *, CCObject *, int);
void dict_hk1(cocos2d::CCDictionary *d, CCObject *obj, int key)
{

	// LOGD("%s", obj, key);

	switch (key)
	{

	case 0x4EB:
        return dict1( d, CCString::create( "edit_eCounterBtn_001" ), key );
        break;

	case 0x732:
        return dict1( d, CCString::create( "edit_eCounterBtn_001" ), key );
        break;
		
	case 0xBCE:
        return dict1( d, CCString::create( "edit_eCounterBtn_001" ), key );
        break;
		
	case 0xB53:
        return dict1( d, CCString::create( "edit_eCounterBtn_001" ), key );
        break;
		default:

		dict1(d, obj, key);
		break;
	}
}

*/

void (*PauseLayer_onEditO)(PauseLayer*, CCObject*);
void PauseLayer_onEditH(PauseLayer* self, CCObject*) {
    auto gm = GameManager::sharedState();

    gm->_playLayer()->stopAllActions();
    gm->_playLayer()->unscheduleAllSelectors();

    GameSoundManager::sharedManager()->stopBackgroundMusic();

    gm->_playLayer()->removeAllObjects();

    self->runAction(
        CCSequence::create(
            CCDelayTime::create(0),
            CCCallFunc::create(self, callfunc_selector(PauseLayer::goEditFix)),
            nullptr
        )
    );
}

void PauseLayer::goEditFix() {
    auto dir = CCDirector::sharedDirector();
    
    auto level = GameManager::sharedState()->_playLayer()->_level();

    auto layer = LevelEditorLayer::create(level, false);
    auto scene = CCScene::create();
    scene->addChild(layer);

    dir->replaceScene(
        CCTransitionFade::create(.5, scene)
    );
}

#define HOOK(a, b, c) HookManager::do_hook(getPointerFromSymbol(cocos2d, a), (void*)b, (void**)&c);

void loader()
{
    auto cocos2d = dlopen(targetLibName != "" ? targetLibName : NULL, RTLD_LAZY);
	
//	HOOK("_ZN7cocos2d12CCDictionary9setObjectEPNS_8CCObjectEi", dict_hk1, dict1);
	HOOK("_ZN10GameObject13createWithKeyEi", GameObjectCreateH, GameObjectCreateO);
	HOOK("_ZN14LevelInfoLayer4initEP11GJGameLevelb", LevelInfoLayerInitH, LevelInfoLayerInitO);	
    HOOK("_ZN10PauseLayer6onEditEPN7cocos2d8CCObjectE", PauseLayer_onEditH, PauseLayer_onEditO);
	
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN9AdToolbox16showInterstitialEv"),
                         (void *)GameManager_tryShowAdH,
                         (void **)&GameManager_tryShowAdO);
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN14EditLevelLayer4initEP11GJGameLevel"),
                         (void *)setUpLevelInfo_hk,
                         (void **)&setUpLevelInfo);
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16LevelEditorLayer4initEP11GJGameLevelb"),
                         (void *)editor_callback,
                         (void **)&editor_callback_trp);
   HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16LevelEditorLayer16updateVisibilityEf"),
                        (void *)LevelEditorLayer_updateVisibilityH,
                         (void **)&LevelEditorLayer_updateVisibilityO);
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN7cocos2d18CCSpriteFrameCache17spriteFrameByNameEPKc"),
                         (void *)CCSpriteFrameCache_spriteFrameByNameH,
                         (void **)&CCSpriteFrameCache_spriteFrameByNameO);
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16EditorPauseLayer8onResumeEPN7cocos2d8CCObjectE"),
                         (void *)EditorPauseLayer_onResumeH,
                         (void **)&EditorPauseLayer_onResumeO);
						 
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN16LevelEditorLayer12removeObjectEP10GameObjectb"),
                         (void *)EditorPauseLayer_removeH,
                         (void **)&EditorPauseLayer_removeO);
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN15GJBaseGameLayer12addToSectionEP10GameObject"),
                         (void *) GJBaseGameLayer_addToSectionH,
                         (void **)&GJBaseGameLayer_addToSectionO);
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN15GJBaseGameLayer23removeObjectFromSectionEP10GameObject"),
                         (void *) GJBaseGameLayer_removeObjectFromSectionH,
                         (void **)&GJBaseGameLayer_removeObjectFromSectionO);	 
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN7UILayer12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE"),
                         (void *)UILayer_ccTouchBeganH,
                         (void **)&UILayer_ccTouchBeganO);
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN7UILayer12ccTouchEndedEPN7cocos2d7CCTouchEPNS0_7CCEventE"),
                         (void *)UILayer_ccTouchEnded,
                         (void **)&UILayer_ccTouchEndedO);
						 
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
	//HOOK("_ZN7cocos2d9extension12CCHttpClient4sendEPNS0_13CCHttpRequestE",
	//HttpSendH, HttpSendO);
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
	HOOK("_ZN19LevelSettingsObject14objectFromDictEPN7cocos2d12CCDictionaryE",
	objectfromDictH, objectfromDictO);
	HOOK("_ZN16LevelEditorLayer15getTriggerGroupEi",
	getTriggerGroupH, getTriggerGroupO);
	HOOK("_ZN7UILayer4initEv",
	UILayerInitH, UILayerInitO);
	HOOK("_ZN16LevelEditorLayer10addToGroupEP10GameObjectib",
	addToGroupH, addToGroupO);
	HOOK("_ZN8EditorUI18dynamicGroupUpdateEb",
	buildHelperH, buildHelperO);
	/*
		HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11AppDelegate11trySaveGameEb"), (void*) save_hook, (void **) &save_trp);

	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN7cocos2d11CCFileUtils13addSearchPathEPKc"), (void*) CCFileUtils_addSearchPath_hk, (void **) &CCFileUtils_addSearchPath_trp);
	HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11GameManager10dataLoadedEP13DS_Dictionary"), (void*) &GameManager_dataLoaded_hk, (void **) &dataLoaded_trp);
	*/
	HOOK("_ZN17AccountLoginLayer8onSubmitEPN7cocos2d8CCObjectE", AccountSubmitH, AccountSubmitO);
	HOOK("_ZN17AccountLoginLayer20loginAccountFinishedEii", LoginFinishedH, LoginFinishedO);
	HOOK("_ZN12LoadingLayer16getLoadingStringEv",getStringH, getStringO);
	HOOK("_ZN14LevelInfoLayer13downloadLevelEv", downloadLevelH, downloadLevelO);





    patch *tmp = new patch();
    tmp->addPatch("libcocos2dcpp.so", 0x26DB2E, "00 bf 00 bf");
	
	


    //playtest
    tmp->addPatch("libcocos2dcpp.so", 0x2B8896, "00 bf 00 bf");
    tmp->addPatch("libcocos2dcpp.so", 0x2B88A4, "00 bf 00 bf");
    tmp->addPatch("libcocos2dcpp.so", 0x2B893C, "00 bf");
	
	/*
	tried to fix colors and opacity
    tmp->addPatch("libcocos2dcpp.so", 0x2A8AD2, "00 bf");
    tmp->addPatch("libcocos2dcpp.so", 0x2A8AD8, "00 bf");
    tmp->addPatch("libcocos2dcpp.so", 0x2A88EC, "00 bf 00 bf");
*/
    //test fix colors
    /*
    tmp->addPatch("libcocos2dcpp.so", 0x2CA80E, "00 bf");
    tmp->addPatch("libcocos2dcpp.so", 0x2CA81A, "00 bf");
    tmp->addPatch("libcocos2dcpp.so", 0x2CA758, "00 bf");

    tmp->addPatch("libcocos2dcpp.so", 0x2CA7D6, "11 e0");
    */
	    //tmp->addPatch("libcocos2dcpp.so", 0x37BB72, "00 bf 00 bf");

	
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
	
	//shit
//	tmp->addPatch("libcocos2dcpp.so", 0x2B8828, "00 BF 00 BF");
	
	//remove tos popup
//	tmp->addPatch("libcocos2dcpp.so", 0x26C15C, "00 BF 00 BF");

	
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
