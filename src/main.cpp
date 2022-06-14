#include <jni.h>
#include <dlfcn.h>
#include <cstdio>
#include <pthread.h>
#include <Logger.h>
#include "cocos2d.h"
#include "gd.h"
#include "hooking.h"

#define targetLibName ("libcocos2dcpp.so")

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

        auto dir = cocos2d::CCDirector::sharedDirector( );
        auto layer = LevelEditorLayer::create( this->gameLevel_ ,false);
        auto sc = CCScene::create();
        sc->addChild(layer);

        auto scene = cocos2d::CCTransitionFade::create(
                0.5,sc
        );

        dir->pushScene( scene );


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


    LOGE("Member address = %d",(int*)&p->vector_2873 - (int*)p);

    auto gm = GameManager::sharedState( );
    *((bool *)gm + 442) = true;
    p->ignoreDamage_ = gm->getGameVariable( "0009");
    p->unk_bool_01 = gm->getGameVariable( "0001");
    p->unk_bool_02 = gm->getGameVariable( "0044");
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


    p->vector_2868.reserve( 9999 );
    p->vector_2858.reserve( 9999 );
    p->vector_2878.reserve( 9999 );
    p->vector_2853.reserve( 9999 );
    p->vector_2873.reserve( 9999 );
    p->vector_2881.reserve( 9999 );
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


    p->dCross_ = CCSprite::createWithSpriteFrameName( "d_cross_01_001.png" );
    p->batchNode_->addChild( p->dCross_, 10 );
    p->dCross_->setColor( ccc3( 255, 255, 255 ) );
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
    p->createGroundLayer(true,true);
    p->gridLayer_->updateTimeMarkers();
    p->createBackground(0);
    p->createBackground(1);
    p->editorUI_->updateSlider( );

    p->resetGroupCounters(false);
    p->sortStickyGroups();
    p->updateEditorMode();
    p->schedule( schedule_selector( LevelEditorLayer::updateVisibility ));

    return true;
}

bool (*setUpLevelInfo)(EditLevelLayer *, GJGameLevel *);
bool setUpLevelInfo_hk(EditLevelLayer *ptr, GJGameLevel *level)
{

    auto ret = setUpLevelInfo(ptr, level);




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
    p->sortBatchnodeChildren(0.0);

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
                    if (sect->count() > 0) {
                        for (int k = 0; k < sect->count(); k++) {
                            GameObject *obj = dynamic_cast<GameObject *>(sect->objectAtIndex(k));
                            auto objectPos = obj->getPosition();
                            if(rect.containsPoint(objectPos)){
                                if(!obj->getParent()){
                                    p->gridLayer_->addChild(obj);
                                }
                                obj->setVisible(true);
                            }else{
                                if(obj->isVisible()){
                                    obj->setVisible(false);
                                    obj->retain();
                                    p->gridLayer_->removeChild(obj);
                                }
                            }
                        }
                    }
                }
            }

        }
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
    *((bool *)p + 11601) = gm->getGameVariable( "0044");
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

    p->removeFromParentAndCleanup(true);
    //(*(int (__fastcall **)(EditorPauseLayer *, int))(p + 256))(p, 1);
}

void (*EditorPauseLayer_removeO)(LevelEditorLayer* self,GameObject *,bool);
void EditorPauseLayer_removeH(LevelEditorLayer* p,GameObject * obj,bool a1){
    EditorPauseLayer_removeO(p,obj,a1);
    obj->removeFromParent();
}



void loader()
{
	auto cocos2d = dlopen(targetLibName != "" ? targetLibName : NULL, RTLD_LAZY);
    HookManager::do_hook(getPointerFromSymbol(cocos2d, "_ZN11GameManager19tryShowInterstitialEiii"),
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
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	// init_handle();
	pthread_t t;
	pthread_create(&t, NULL,
				   reinterpret_cast<void *(*)(void *)>(loader), NULL);





	return JNI_VERSION_1_6;
}
