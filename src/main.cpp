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

#define MEMBERBYOFFSET(type, class, offset) *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(class) + offset)
#define MBO MEMBERBYOFFSET

string passwordTemp = "";
bool shouldAdd = true;
bool layersfix;
bool colorPopup = true;






template <typename T>
std::string itos(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

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


void logNames(CCArray* objects) {
	
	for(int i = 0; i < objects->count(); i++) {
		auto node = (CCNode*)objects->objectAtIndex(i);
		auto name = get_name_for_node(node);
		CCLog("i: %d | %s", i, name.c_str());
		
		if(node->getChildrenCount() > 0) {
			
			auto objects2 = node->getChildren();
			
			for(int j = 0; j < node->getChildrenCount(); j++) {
				
			auto node2 = (CCNode*)objects2->objectAtIndex(i);
			auto name2 = get_name_for_node(node2);
			CCLog("inside of %s | j: %d | %s", name.c_str(), j, name2.c_str());
			
			}
		}
		
	}
		CCLog("Finished");

}


void *getPointerFromSymbol(void *handle, const char *symbol)
{
    return reinterpret_cast<void *>(dlsym(handle, symbol));
}




#include "CCMenuItemToggler.h"
void createToggleButton(const char *text, CCPoint position, float boxScale, float labelScale, CCObject *baseclass, cocos2d::SEL_MenuHandler callback, CCMenu *menu, bool toggled, bool enabled)
{
    auto onSpr = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto offSpr = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

    onSpr->setScale(boxScale);
    offSpr->setScale(boxScale);

    auto toggle = CCMenuItemToggler::create(
        onSpr,
        offSpr,
        baseclass,
        callback);
    toggle->setSizeMult(1.5);
    toggle->toggle(!toggled);

    auto label = CCLabelBMFont::create(text, "bigFont.fnt");
    label->limitLabelWidth(80, labelScale, 0);
    label->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentLeft);

    toggle->setPosition(position);
    label->setPosition(toggle->getPositionX() + 17.5 + label->getScaledContentSize().width / 2, toggle->getPositionY());

    menu->addChild(toggle);
    menu->addChild(label);

}


void createLabels(CCNode* base, CCPoint pos, bool logNames) {
	
	CCArray* objects = base->getChildren();
	int count = objects->count();
	auto menu = CCMenu::create();
	
	menu->setPosition(pos);
	base->addChild(menu, 100);
	
	for(int i = 0 ; i < count; i++) {
	
	auto node = (CCNode*)objects->objectAtIndex(i);
	if(logNames) {
		const char* name = get_name_for_node(node).c_str();
		CCLog("i: %d: | name: %s", i, name);
	}
	
	auto label = CCLabelBMFont::create(itos(i).c_str(), "bigFont.fnt");
	label->setPosition(node->getPosition());
	label->setScale(.65);
	menu->addChild(label);
		
	}
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
	
	
	//createLabels(ptr, {0,0}, true);
	
	
	
	
	
	
	
    return ret;
}
class events : public EditLevelLayer {
public:
    void onCLick(){
        LOGD("ENTER! onClick!");
		/*
		if(!GM->getGameVariable("100099")) {
			
			FLAlertLayer::create(nullptr, "DISCLAIMER", "This version of the editor is\n<cr>unstable and contains bugs and crashes that can't be fixed</c>.\nYour game will crash or freeze, <cg>save your game and levels often</c>.\n<co>If you don't want an unstable editor wait for official 2.2 and don't complain about it</c>.", "OK", nullptr, 500, false, 300)->show();
			GM->setGameVariable("100099", true);
		}
		else {
			*/
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
	//	}
		

    }
};

void createArrayAndAssign(LevelEditorLayer* p,int offset){
    auto array = CCArray::create();
    *((int *)p + offset) = reinterpret_cast<uintptr_t>(array);
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
    p->unk_bool_12 = true; //layer locking
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

    p->_objectsToUpdate() = CCArray::create();
    p->_objectsToUpdate()->retain();

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
   // p->schedule( schedule_selector( LevelEditorLayer::updateVisibility ));
    p->schedule( schedule_selector( LevelEditorLayer::updateEditor ));

    //fix to the glitched background with updateEditor
    p->editorUI_->onStopPlaytest(nullptr);
	layersfix = true;
	


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
	
//		createLabels(ptr->_btnMenu(), {0,0}, true);


    return ret;
};



void (*GameManager_tryShowAdO)();
void GameManager_tryShowAdH(){

}
bool test = false;

void* (*GameObjectSetOpacityO)(GameObject* self, unsigned char opacity);
void* GameObjectSetOpacityH(GameObject* self, unsigned char opacity){
	

	//CCLog("GameObjectSetOpacityH");
	
	return GameObjectSetOpacityO(self, opacity);

}
bool onit = false;

void (*UnlockAllLayersO)(EditorPauseLayer* self, CCObject* a2);
void UnlockAllLayersH(EditorPauseLayer* self, CCObject* a2){
	
	auto editor = self->_editorLayer();
	auto ui = editor->editorUI_;

	onit = true;
	for(int i = 0; i < 1000; i++) {
		CCLog("class member : %d", editor->_currentLayer());
		
		editor->_currentLayer()++;

		if(editor->isLayerLocked(editor->_currentLayer()))
		editor->toggleLockActiveLayer();
	}
	
	CCLog("finish loop");
	editor->_currentLayer() = 0xFFFFFFFF;
	CCLog("reseting");
	onit = false;
	
}

void (*LevelEditorLayer_updateVisibilityO)(LevelEditorLayer*,float);
void LevelEditorLayer_updateVisibilityH(LevelEditorLayer* p,float a1){

	//int a = MBO(int, p, 0x2CBC);
	//CCLog("layertest: %d", a);
	
	//((PlayLayer*)p)->updateVisibility(a1);
	
	int cl = MBO(int, p, 0x2C1C);
	
	if(p->isLayerLocked(cl)) {
		p->toggleLockActiveLayer();
		p->editorUI_->updateGroupIDLabel();
	}
	
	
//Uncomment this to see the wierdest bug you've ever seen
//****************************************************
/*
	if(p->isLayerLocked(3))	
	CCLog("Current Layer: %d | Layer 3 : LOCKED", cl);
	else
	CCLog("Current Layer: %d | Layer 3 : UNLOCKED", cl);
*/
//****************************************************


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

	/*
		UPDATING OBJECTS
	*/
	for(int i = startIndex;  i <= limit; i++){
		if(i >= 0 && i < sections->count()){
			auto section = sections->objectAtIndex(i);
			if(section){
				auto sect = reinterpret_cast<CCArray*>(section);
				


				if (sect) {
					if (sect->count() > 0) {
						for (int k = 0; k < sect->count(); k++) {
							GameObject *obj = dynamic_cast<GameObject *>(sect->objectAtIndex(k));
							

							
							auto objectPos = obj->getPosition();
							if(rect.containsPoint(objectPos)){
								obj->addMainSpriteToParent(false);
								if(obj->hasSecondaryColor()) obj->addColorSpriteToParent(true);
								obj->activateObject();
	
									int currentLayer = MBO(int, p, 0x2C1C);
									int l1 = MBO(int, obj, 0x450);
									int l2 = MBO(int, obj, 0x454);
								
								
									bool shouldBeVisible = (currentLayer == l1 || (currentLayer == l2 && l2 != 0) || currentLayer == -1);
			
									GameObjectSetOpacityH(obj, shouldBeVisible ? 255 : 70);
								
								
								if(!MEMBERBYOFFSET(bool, obj, 0x405)) {
									p->_objectsToUpdate()->addObject(obj);
								}                                
							}
							// GOOFY AHH :trollskullirl:
							else{
								obj->deactivateObject(false);
							}

						}
					}
				}
			}

		}
	}

	p->updateObjectColors(p->_objectsToUpdate());
	p->_objectsToUpdate()->removeAllObjects();
	
	p->processAreaVisualActions();
	p->sortBatchnodeChildren(0.0);
	

}
void (*CCSpriteFrameCache_spriteFrameByNameO)(CCSpriteFrameCache* self,const char* a1);
void CCSpriteFrameCache_spriteFrameByNameH(CCSpriteFrameCache* self,const char* a1){
 //   auto v2 = a1;
  //  if(strstr(v2,"pixelart") || strstr(v2, "pixelb"))
   //     v2 = "d_spikes_01_001.png";
    return CCSpriteFrameCache_spriteFrameByNameO(self,a1);
}

void (*EditorPauseLayer_onResumeO)(EditorPauseLayer* self,CCObject* a1);
void EditorPauseLayer_onResumeH(EditorPauseLayer* p,CCObject* a1){
    auto gm = GameManager::sharedState();

    auto editor = MEMBERBYOFFSET(LevelEditorLayer*, p, 0x1C4);

    MEMBERBYOFFSET(bool, editor, 0x13E) = gm->getGameVariable("0009");
    MEMBERBYOFFSET(bool, editor, 0x2B8C) = gm->getGameVariable("0001");
    //MEMBERBYOFFSET(bool, editor, 0x2B8D) = gm->getGameVariable("0044"); // orbs and shit
    MEMBERBYOFFSET(bool, editor, 0x2B8E) = gm->getGameVariable("0045");
    MEMBERBYOFFSET(bool, editor, 0x2B8F) = gm->getGameVariable("0038");
    MEMBERBYOFFSET(bool, editor, 0x2B91) = gm->getGameVariable("0043");
    MEMBERBYOFFSET(bool, editor, 0x2B92) = gm->getGameVariable("0037");
    MEMBERBYOFFSET(bool, editor, 0x2B93) = gm->getGameVariable("0058");
    MEMBERBYOFFSET(bool, editor, 0x2B94) = gm->getGameVariable("0013");
    MEMBERBYOFFSET(bool, editor, 0x2C5D) = gm->getGameVariable("0036");
    MEMBERBYOFFSET(bool, editor, 0x2B95) = gm->getGameVariable("0078");
    MEMBERBYOFFSET(bool, editor, 0x2B96) = gm->getGameVariable("0120");
    MEMBERBYOFFSET(bool, editor, 0x2B90) = gm->getGameVariable("0079");
    MEMBERBYOFFSET(bool, editor, 0x2B97) = true; //layer locking
    MEMBERBYOFFSET(bool, editor, 0x2B98) = gm->getGameVariable("0104");

    if(editor->editorUI_) editor->editorUI_->updateGroupIDLabel();

    /**((bool *)p + 770) = gm->getGameVariable("0009");
    *((bool *)p + 11600) = gm->getGameVariable("0001");
    //*((bool *)p + 11601) = gm->getGameVariable( "0044");
    *((bool *)p + 11602) = gm->getGameVariable("0045");
    *((bool *)p + 11603) = gm->getGameVariable("0038");
    *((bool *)p + 11605) = gm->getGameVariable("0043");
    *((bool *)p + 11606) = gm->getGameVariable("0037");
    *((bool *)p + 11607) = gm->getGameVariable("0058");
    *((bool *)p + 11608) = gm->getGameVariable("0013");
    *((bool *)p + 11809) = gm->getGameVariable("0036");
    *((bool *)p + 11609) = gm->getGameVariable("0078");
    *((bool *)p + 11610) = gm->getGameVariable("0120");
    *((bool *)p + 11604) = gm->getGameVariable("0079");
    *((bool *)p + 11611) = gm->getGameVariable("0103");
    *((bool *)p + 11612) = gm->getGameVariable("0104");*/

    p->removeFromParentAndCleanup(true);
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
        GJBaseGameLayer_removeObjectFromSectionO(self,obj);
		if(GM->_inEditor())
		obj->removeFromParent();
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
	
	return OptionsLayerInitO(self);}

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
	//	CCLog("password done");
    }
	else
	{ 
		passwordTemp = "0";
	//	CCLog("no file found");
	}
	
	GM->setGameVariable("0122", false);
	CCLog("Loading Init");
	return "Italian APK Downloader\nCatto_\niAndy_HD3\nTr1NgleBoss";
}
	
	

void (*addToggle_trp)(MoreOptionsLayer * self,const char * title, const char * code, const char * desc);
void addToggle_hk(MoreOptionsLayer* self, const char * title, const char * code, const char * desc) {
	
	if (isGDPSSettings)
    {
        if (strcmp(title, "Show Orb Guide") == 0)
        {
         //   addToggle_trp(self, "Robtop Servers", "100001", "Change servers to robtop servers");
         //   addToggle_trp(self, "Force Platformer", "100002", "All levels are platformer mode");
			addToggle_trp(self, "Instantly open editor\nat startup", "100003", 0);
			//addToggle_trp(self, "Disable editor\nobject visibility", "100004", 0);
			addToggle_trp(self, "Enable pixel blocks\nin the editor", "100005", 0);
			addToggle_trp(self, "Disable Playtest", "100006", 0);
			
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
	
//	CCLog(AY_OBFUSCATE("APK Belongs to DM Group"));
	
	//string useless = GDPS->itos(1);
		//	self->runAction(CCCallFuncO::create(self, callfuncO_selector(CreatorLayer::onMyLevels), self));
			
	if(first) {
		if(GM->getGameVariable("100003"))
			self->runAction(CCCallFuncO::create(self, callfuncO_selector(CreatorLayer::onMyLevels), self));
	first = false;
	}
		

	auto dir = CCDirector::sharedDirector();
	/*auto old_menu = CCMenu::create();
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
	
	
	
	self->addChild(old_menu, 900);*/
	
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



bool (*canPlayOnlineLevelsO)(CreatorLayer* self);
bool canPlayOnlineLevelsH(CreatorLayer* self){
	return true;
}

bool (*UILayerInitO)(UILayer* self);
bool UILayerInitH(UILayer* self){
	
	return UILayerInitO(self);
}
/*
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
*/
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


#include "AccountLoginLayer.h"

void *(*AccountSubmitO)(AccountLoginLayer* self, CCObject* a2, void* a3, void* a4);
void *AccountSubmitH(AccountLoginLayer* self, CCObject* a2, void* a3, void* a4)
{
	
	passwordTemp = self->_inputPassword()->getString();
	//CCLog(passwordTemp.c_str());
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
//	CCLog("name: %s | key: %d", tb, key);
	/*
	switch(key) {
		
		case 2013:
		case 2223:
		return GameObjectCreateO(1);
		
	}
	*/
	if(contains(tb, "pixelb"))
	return GameObjectCreateO(2142);

	if(contains(tb, "pixel")) {
	
		if(contains(tb, "b_"))
		return GameObjectCreateO(2142);

		auto pixelKey = mid_num(tb);

		return GameObjectCreateO(pixelKey > 140 ? 2142 : key);
	}
	
	return GameObjectCreateO(key);

}

	
const char* (*keyToFrameO)(ObjectToolbox* self, int key);
const char* keyToFrameH(ObjectToolbox* self, int key) {
	
//	auto ret = keyToFrameO(self, key);
//	CCLog("%d | | %s", key, ret);
	
	if(key >= 2925) {
		
		switch(key) {

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
	}}
//7DA3E7



void* (*PlayLayer_addObjectO)(PlayLayer* self, GameObject* obj);
void* PlayLayer_addObjectH(PlayLayer* self, GameObject* obj) {
	
	
	int id = obj->_objectID();
//	CCLog("Object ID: %d", id);
	
	if(id == 1331) {
		
		patch *s = new patch();
		string spider = "70 6F 72 74 61 6C 5F 31 37 5F 62 61 63 6B 5F 30 30 31 2E 70 6E 67";
		s->addPatch("libcocos2dcpp.so", 0x7DA3E7, spider);
		s->Modify();
	}
	
	
	if(id == 1933) {
		
		patch *p = new patch();
		string swing = "70 6F 72 74 61 6C 5F 31 38 5F 62 61 63 6B 5F 30 30 31 2E 70 6E 67";
		p->addPatch("libcocos2dcpp.so", 0x7DA3E7, swing);
		p->Modify();
		
	}
	
	return PlayLayer_addObjectO(self, obj);

	
}
	
	

void (*downloadLevelO)(LevelInfoLayer* self);
void downloadLevelH(LevelInfoLayer* self) {
	
	shouldAdd = false;
	downloadLevelO(self);
	shouldAdd = true;
}

// moving funny loading text
bool (*LoadingLayer_initO)(LoadingLayer*, bool);
bool LoadingLayer_initH(LoadingLayer* self, bool fromReload) {
    if(!LoadingLayer_initO(self, fromReload)) return false;

    auto text = *reinterpret_cast<CCNode**>(reinterpret_cast<uintptr_t>(self) + 0x140);
    text->setPositionY(text->getPositionY() - 10);
    
    return true;
}

// fix funky profile button
void (*MenuLayer_updateUserProfileButtonO)(MenuLayer*);
void MenuLayer_updateUserProfileButtonH(MenuLayer* self) {
    auto accountManager = GJAccountManager::sharedState();

    int accountID1 = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(accountManager) + 0x10C);
    int accountID2 = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(accountManager) + 0x110);

    bool loggedIn = (accountID1 - accountID2) > 0;

    self->_playerUsernameLabel()->setVisible(loggedIn);
    self->_profileBtn()->setVisible(loggedIn);

    if(loggedIn) {
        // ---------- MOTHERFUCKER WHY DOES THIS CRASH THE OFFSET IS RIGHT AND EVERYTHING
        //self->_playerUsernameLabel()->setString(accountManager->_username().c_str());
        //self->_playerUsernameLabel()->limitLabelWidth(70, .7, 0);
    }
}

void (*MenuLayer_showTOSO)(MenuLayer*);
void MenuLayer_showTOSH(MenuLayer* self) {
    *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(self) + 316) = false;
}

void* (*EditorUI_SelectObjectsO)(EditorUI* self, CCArray* objects, bool a3);
void* EditorUI_SelectObjectsH(EditorUI* self, CCArray* objects, bool a3) {
	

   	int cl = MBO(int, self->_levelEditor(), 0x2C1C);
	if(cl != -1) {
		int count = objects->count();
		auto toDelete = CCArray::create();
		
		for(int i = 0; i < count; i++) {
		//	CCLog("loop: %d", i);
			auto obj = (GameObject*)objects->objectAtIndex(i);
			int l1 = MBO(int, obj, 0x450);
			int l2 = MBO(int, obj, 0x454);
		//	CCLog("CL: %d | L1: %d | L2: %d", cl, l1, l2);
			if(cl != l1 && l2 == 0) {
				toDelete->addObject(obj);
			//	CCLog("object %d removed from array", i);
			}
		}
		objects->removeObjectsInArray(toDelete);
	}
   
   return EditorUI_SelectObjectsO(self, objects, a3);
}





bool (*EditorUI_InitO)(EditorUI* self, LevelEditorLayer* editor);
bool EditorUI_InitH(EditorUI* self, LevelEditorLayer* editor) {
	
	if(!EditorUI_InitO(self, editor))
		return false;
	
	
	if(!GM->getGameVariable("100005")) {
		
		auto menu = self->_objectMenu();
		auto btns = menu->getChildren();
		
		auto pixelblocksBtn = (CCMenuItemSpriteExtra*)btns->objectAtIndex(8);
		btns->removeObject(pixelblocksBtn, false);
		menu->alignItemsHorizontallyWithPadding(-2);
	}
	
	return true;

	
}

int (*onToggleTrampoline)(void *pthis, const char *val);
void hook_onToggle(void *pthis, const char *val) {

	int v = atoi(val);

	onToggleTrampoline(pthis, val);

	if (v > 100000)
	{
		if(v == 100005 && GM->getGameVariable("100005")) {
			FLAlertLayer::create(nullptr, "DISCLAIMER", "<cg>Pixel blocks</c> are <cr>not official</c> and for that reason levels containing these blocks <cr>will not look good in the official 2.2.</c>\n     <co>(textures will change)</c> <cr>Don't use pixel blocks if you want your level to be playable in 2.2.</c>", "OK", nullptr, 500, false, 300)->show();
		}
	}
}


bool (*SelectArtLayer_initO)(SelectArtLayer *, SelectArtType);
bool SelectArtLayer_initH(SelectArtLayer *self, SelectArtType type)
{
	if (!SelectArtLayer_initO(self, type))
		return false;
	
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
	
	
/*
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
				
			}
		}
	}


	if (type == SelectArtType::background)
	{
		auto okBtn = (CCMenuItemSpriteExtra *)self->_bgSelectMenu()->getChildren()->objectAtIndex(self->_bgSelectMenu()->getChildrenCount() - 1);

		okBtn->setPositionX(80);
		okBtn->setPositionY(okBtn->getPositionY() + 5);
	}
*/
	return true;
}


#include "AccountRegisterLayer.h"
bool (*AccountRegisterLayer_InitO)(AccountRegisterLayer*);
bool AccountRegisterLayer_InitH(AccountRegisterLayer*) {
	
	cocos2d::CCApplication::sharedApplication()->openURL("http://game.gdpseditor.com/server/tools/account/registerAccount.php");
	
	
}
#include "SetupTriggerPopup.h"
#include "SetupPickupTriggerPopup.h"

bool (*SetupPickupTriggerO)(SetupPickupTriggerPopup *, EffectGameObject *, cocos2d::CCArray *, float, float);
bool SetupPickupTriggerH(SetupPickupTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects, float a, float b)
{
	auto ret = SetupPickupTriggerO(self, object, objects, a, b);
//pickup
	
	EffectGameObject* obj = MBO(EffectGameObject*, self, 0x1F4);
	bool overrideCount = MBO(bool, obj, 0x5C5);
	CCMenu* menu = self->_menu();
	

	auto multitrigger = (CCMenuItemToggler*)menu->getChildren()->objectAtIndex(9);
	
		createToggleButton("Override\nCount",
		{55, 0},
		.7,
		0.5,
		self,
		menu_selector(SetupPickupTriggerPopup::onOverrideCount),
		menu, overrideCount, true);
		
		

		
		multitrigger->setPositionY(multitrigger->getPositionY() + 36);
		multitrigger->setPositionX(multitrigger->getPositionX() - 23);
		
		auto layer2 = (CCLayer*)self->getChildren()->objectAtIndex(0);
		auto label = (CCLabelBMFont*)layer2->getChildren()->objectAtIndex(9);
		
		label->setPositionY(label->getPositionY() + 36);
		label->setPositionX(label->getPositionX() - 23);
		
		//createLabels(layer2, {0,0}, true);
	//	createLabels(menu, {0,0}, true);


	return ret;

}


int lastTriggerObjectID = 0;


bool (*SetupTriggerPopupO)(SetupTriggerPopup *, EffectGameObject *, cocos2d::CCArray *, float, float);
bool SetupTriggerPopupH(SetupTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects, float x, float y)
{
	auto ret = SetupTriggerPopupO(self, object, objects, x, y);
	
	if(objects == nullptr) 
	return ret;

	if(objects->count() > 1)
	return ret;
	
	if(colorPopup) {
		int id = object->_objectID();
		lastTriggerObjectID = id;
	}
	
	return ret;

}

#include "SetupAreaMoveTriggerPopup.h"
#include "InfoAlertButton.h"

	
	

#include "ColorSelectPopup.h"

void* (*ColorSelectPopupInitO)(ColorSelectPopup *, EffectGameObject *, cocos2d::CCArray *, ColorAction*);
void* ColorSelectPopupInitH(ColorSelectPopup *self, EffectGameObject *object, cocos2d::CCArray *objects, ColorAction* a4)
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



bool (*infoButton)(string, string, float);
bool infoButton_hk(string title, string desc, float a3)
{

	auto ret = infoButton(title, desc, a3);

	if(GM->_inEditor() && colorPopup) {
		


	switch(lastTriggerObjectID) {
		
			case 901: //move
			desc += "\n- <cp>x/y modifier 4 locks to the camera if lock to camera is enabled</c>";
			return infoButton(title, desc, a3);
			break;
			
			case 1346: //rotate 
			desc += "\n- <cp>Follow p follows the players icon rotation. Some modes don't work with this.</c>";
			return infoButton(title, desc, a3);
			break;
			
			case 2067: //scale
			desc += "\n- <cp>Scaling a singular object uses  the objects rotated x/y instead of grid x/y.</c>";
			return infoButton(title, desc, a3);
			break;
			
			case 1585: //aninamte
			desc += "\n- <cp>Can also animate particle effects and animated object.</c>";
			return infoButton(title, desc, a3);
			break;
			
			case 2062: //camera edge
			return infoButton("Camera Edge Trigger", "Stops the camera on a target ID.\n\
			Direction is based on the players directions.\n\
			Target id 0 unlocks camera egdes.\n\
			This only works when having a direction selected", a3);
			break;
			
			case 1914: // static camera
			return infoButton("Static Camera", "Locks the camera to a specific object.\nOnly X and Y will will lock the camera to only scroll horizontally and vertically respectfully.\nExit Static moves the camera back to the player. This does not require a group.\nfollow ignores move time. Set move time to 0 to avoid the camera to instantly jump to the target.", a3);
			break;
			
			case 1817: //pickup trigger
			return infoButton("Pickup Trigger", "Adds or subtracts a value from a counter.\nOverwrite count sets the value of the counter to that of the pickup trigger.", a3);
			break;
			
			case 1814: //follow player Y
			return infoButton("Follow Player Y", "makes objects follow the Y position of the player\nOffset controls how much higher or lower the object should follow. \nDelay controls how much time passes until the object goes to the player Y position.\nMax Speed controls how fast the object should follow the players Y position\nSpeed multiplies the Max Speed.", a3);
			break;
			
			case 1916: //camera offset
			return infoButton("Camera Offset", "Offsets the camera by X or Y.\nThe movement will be based on the original camera position, not the current one. This means offsetting the camera twice won't double the offset", a3);	
			break;
			
			case 2016: //camera guide
			return infoButton("Camera Guide", "Shows the position of the camera if the player was to spawn on the exact position of the Guide Trigger.\n<cg>The green line shows the entire visible area.</c>\n<cy>The yellow lines show where objects start to fade in and out.</c>", a3);
			break;
			
			case 1934: //song trigger
			return infoButton(title, "<cy>Sets the time of the song in ms.</c>\n<cr>This means 1000ms = 1 second</c>", a3);
			break;
			
			case 1935: //timewarp
			return infoButton("Timewarp", "Changes the speed of the game like a speedhack", a3);
			break;
			
			case 1811: //instant count
			return infoButton("Instant Count", "Only checks for the number count of an item ID channel one time instead of continuously checking until being stopped. Equals checks for if the item ID count is the exact same as the set number, smaller and larger check for if the number is smaller or larger.", a3);
			break;
			
			case 2925: //camera mode
			return infoButton("Camera Mode", "Enabled free mode for gamemodes with locked camera.\n\
			Padding determines when the camera moves with the player on y.\n\
			Easing eases the camera. (Who could've expected that!)", a3);
			break;
			
			case 2999: //Middle Ground Trigger
			return infoButton("Middle Ground Trigger", "It moves the middle ground on Y.", a3);
			break;
			
			case 2899: //options trigger
			return infoButton("Options Trigger", "Changes how the game behaves and what it should render.", a3);
			break;
			
			case 2903: //gradient trigger
			desc += "\n<cg>Uses base and detail color. Does not work with hsv.</c>\n\
<cb>You can choose on what layer the gradient should be.</c>\n\
<co>Rotating the gradient trigger also rotates the gradient.</c>\n\
<cp>Vertex mode uses math instead of textures to create the gradient. U D L R (BL BR TL TR in vertex mode) places the egdes of the gradient on center ids.</c>\n\
<cr>Change the ID if you use more gradients.\n\
The maximum amount of ids is 255.</c>";
			return infoButton("Gradient Trigger", desc, a3);
			break;
			

			case 3006: //Area Move 
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
			
			case 3007: { //rotate
			
			
						string aR1 = "<cg>Length: 30 = 1 gridspace.</c>\n\
<cy>Offset: offsets the area effect</c> ";

		string aR2 = "\n<co>Easing/ease out: determines the shape of the dropoff of the area effect</c>\n\
		<cr>OffsetY: Offset of the area effect in the Y axis.</c>\n\
		<cp>ModFront/Back: modifier for easing. </c>\n\
		<cl>Deadzone: the range at which the area effect is completly applied from the egde of the area effect to the center.</c>";

		
			

			string aR3 = "\n<cg>Rotation: the amount of degrees the objects will spin. Ignores locked rotation objects but also doesnt change the hitbox.</c>";
						
			string des1 = aR1 + aR2 + aR3;

			return infoButton("Area Rotate Trigger", des1, a3);
			
			break;
			}
			
			case 3008: {
				
				
						string aR1 = "<cg>Length: 30 = 1 gridspace.</c>\n\
<cy>Offset: offsets the area effect</c> ";

		string aR2 = "\n<co>Easing/ease out: determines the shape of the dropoff of the area effect</c>\n\
		<cr>OffsetY: Offset of the area effect in the Y axis.</c>\n\
		<cp>ModFront/Back: modifier for easing. </c>\n\
		<cl>Deadzone: the range at which the area effect is completly applied from the egde of the area effect to the center.</c>";

		
			

			string aR3 = "\n<cg>Scalex/y: Scales objects by their relative x/y unless a p group is used.</c>";
								string des1 = aR1 + aR2 + aR3;
	
			return infoButton("Area Scale Trigger", des1, a3);
	
			}
			break;
			
						case 3009: {
				
				
						string aR1 = "<cg>Length: 30 = 1 gridspace.</c>\n\
<cy>Offset: offsets the area effect</c>";

			string aR2 = "\n<cr>OffsetY: Offset of the area effect in the Y axis.</c>\n\
		<cp>ModFront/Back: modifier for easing. </c>\n\
		<cl>Deadzone: the range at which the area effect is completly applied from the egde of the area effect to the center.</c>";

			string aR3 = "\n<co>Opacity: the opacity the object should be set to.</c>";
								string des1 = aR1 + aR2 + aR3;
	
			return infoButton("Area Fade Trigger", des1, a3);
	
			}
			break;
			
									case 3010: {
				
				
						string aR1 = "<cg>Length: 30 = 1 gridspace.</c>\n\
<cy>Offset: offsets the area effect</c> ";

		string aR2 = "\n<cr>OffsetY: Offset of the area effect in the Y axis.</c>\n\
		<cp>ModFront/Back: modifier for easing. </c>\n\
		<cl>Deadzone: the range at which the area effect is completly applied from the egde of the area effect to the center.</c>";

			string aR3 = "\n<co>Color channel: to what colors the objects should change</c>\n\
				<cr>%: the intensity of the effect.</c>\n\
			<cg>Hsv: uses hsv instead of a color channel.</c>";
			
								string des1 = aR1 + aR2 + aR3;
	
			return infoButton("Area Tint Trigger", des1, a3);
	
			}
			break;
			
			case 3011: //All Area Triggers
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
	


bool (*SetupAreaMoveTriggerPopupO)(SetupAreaMoveTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaMoveTriggerPopuH(SetupAreaMoveTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaMoveTriggerPopupO(self, object, objects);
	
	colorPopup = false;
	
	auto layer2 = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*)layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(36);
	auto reference2 = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(1);
	
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



bool (*SetupAreaRotateTriggerPopupO)(SetupAreaRotateTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaRotateTriggerPopupH(SetupAreaRotateTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaRotateTriggerPopupO(self, object, objects);
	
	colorPopup = false;
	
	auto layer2 = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*)layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(36);
	
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



bool (*SetupAreaTransformTriggerPopupO)(SetupAreaTransformTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaTransformTriggerPopupH(SetupAreaTransformTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaTransformTriggerPopupO(self, object, objects);
	
	colorPopup = false;
	
	auto layer2 = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*)layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(36);
	
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

bool (*SetupAreaFadeTriggerPopupO)(SetupAreaFadeTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaFadeTriggerPopupH(SetupAreaFadeTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaFadeTriggerPopupO(self, object, objects);
	
	colorPopup = false;
	
	auto layer2 = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*)layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(36);
	
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

bool (*SetupAreaTintTriggerPopupO)(SetupAreaTintTriggerPopup *, EffectGameObject *, cocos2d::CCArray *);
bool SetupAreaTintTriggerPopupH(SetupAreaTintTriggerPopup *self, EffectGameObject *object, cocos2d::CCArray *objects)
{
	auto ret = SetupAreaTintTriggerPopupO(self, object, objects);
	
	colorPopup = false;
	
	auto layer2 = (CCLayer*)self->getChildren()->objectAtIndex(0);
	auto menu = (CCMenu*)layer2->getChildren()->objectAtIndex(1);
	auto reference = (CCMenuItemSpriteExtra*)menu->getChildren()->objectAtIndex(36);
	
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



void (*CreateParticlePopup_onPasteSettingsO)(SetupTriggerPopup* self, CCObject* a2);
void CreateParticlePopup_onPasteSettingsH(SetupTriggerPopup* self, CCObject* a2){

}

void (*EditorUI_onPlaytestO)(EditorUI* self, CCObject* a2);
void EditorUI_onPlaytestH(EditorUI* self, CCObject* a2){
	
	if(!GM->getGameVariable("100006"))
		return EditorUI_onPlaytestO(self, a2);

}




void loader()
{
	
	#define HOOK(a, b, c) HookManager::do_hook(getPointerFromSymbol(cocos2d, a), (void*)b, (void**)&c);
    auto cocos2d = dlopen(targetLibName != "" ? targetLibName : NULL, RTLD_LAZY);
	
	

	HOOK("_ZN8EditorUI10onPlaytestEPN7cocos2d8CCObjectE", EditorUI_onPlaytestH, EditorUI_onPlaytestO);
	HOOK("_ZN19CreateParticlePopup14onCopySettingsEPN7cocos2d8CCObjectE", CreateParticlePopup_onPasteSettingsH, CreateParticlePopup_onPasteSettingsO);
	HOOK("_ZN19CreateParticlePopup15onPasteSettingsEPN7cocos2d8CCObjectE", CreateParticlePopup_onPasteSettingsH, CreateParticlePopup_onPasteSettingsO);
	HOOK("_ZN25SetupAreaTintTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaTintTriggerPopupH, SetupAreaTintTriggerPopupO);
	HOOK("_ZN25SetupAreaFadeTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaFadeTriggerPopupH, SetupAreaFadeTriggerPopupO);
	HOOK("_ZN30SetupAreaTransformTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaTransformTriggerPopupH, SetupAreaTransformTriggerPopupO);
	HOOK("_ZN27SetupAreaRotateTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaRotateTriggerPopupH, SetupAreaRotateTriggerPopupO);
	HOOK("_ZN25SetupAreaMoveTriggerPopup4initEP17EnterEffectObjectPN7cocos2d7CCArrayE", SetupAreaMoveTriggerPopuH, SetupAreaMoveTriggerPopupO);
	HOOK("_ZN16ColorSelectPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayEP11ColorAction", ColorSelectPopupInitH, ColorSelectPopupInitO);
	HOOK("_ZN15InfoAlertButton6createESsSsf", infoButton_hk, infoButton);
	HOOK("_ZN9PlayLayer9addObjectEP10GameObject", PlayLayer_addObjectH, PlayLayer_addObjectO);
	HOOK("_ZN17SetupTriggerPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayEff", SetupTriggerPopupH, SetupTriggerPopupO );
	HOOK("_ZN23SetupPickupTriggerPopup4initEP16EffectGameObjectPN7cocos2d7CCArrayE", SetupPickupTriggerH, SetupPickupTriggerO);
	HOOK("_ZN20AccountRegisterLayer4initEv", AccountRegisterLayer_InitH, AccountRegisterLayer_InitO);
	//HOOK("_ZN14SelectArtLayer4initE13SelectArtType", SelectArtLayer_initH, SelectArtLayer_initO)
	HOOK("_ZN11GameManager18toggleGameVariableEPKc", hook_onToggle, onToggleTrampoline);
	HOOK("_ZN13ObjectToolbox13intKeyToFrameEi", keyToFrameH, keyToFrameO);
	HOOK("_ZN8EditorUI4initEP16LevelEditorLayer", EditorUI_InitH, EditorUI_InitO);
	HOOK("_ZN8EditorUI13selectObjectsEPN7cocos2d7CCArrayEb", EditorUI_SelectObjectsH, EditorUI_SelectObjectsO);
	HOOK("_ZN16EditorPauseLayer17onUnlockAllLayersEPN7cocos2d8CCObjectE", UnlockAllLayersH, UnlockAllLayersO);
	HOOK("_ZN10GameObject10setOpacityEh", GameObjectSetOpacityH, GameObjectSetOpacityO);
	HOOK("_ZN10GameObject13createWithKeyEi", GameObjectCreateH, GameObjectCreateO);
	HOOK("_ZN14LevelInfoLayer4initEP11GJGameLevelb", LevelInfoLayerInitH, LevelInfoLayerInitO);	
    HOOK("_ZN12LoadingLayer4initEb", LoadingLayer_initH, LoadingLayer_initO);
    HOOK("_ZN9MenuLayer23updateUserProfileButtonEv", MenuLayer_updateUserProfileButtonH, MenuLayer_updateUserProfileButtonO);
    HOOK("_ZN9MenuLayer7showTOSEv", MenuLayer_showTOSH, MenuLayer_showTOSO);

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
	HOOK("_ZN16LevelEditorLayer15getTriggerGroupEi",
	getTriggerGroupH, getTriggerGroupO);
	HOOK("_ZN7UILayer4initEv",
	UILayerInitH, UILayerInitO);
	HOOK("_ZN16LevelEditorLayer10addToGroupEP10GameObjectib",
	addToGroupH, addToGroupO);
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
	
	//gradient bypass
    tmp->addPatch("libcocos2dcpp.so", 0x385134, "FF");
	
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
