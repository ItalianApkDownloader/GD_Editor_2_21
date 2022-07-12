#include <gd.h>
#include "cocos2d.h"
#include "LevelEditorLayerExt.h"
#include "hooking.h"

#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

#define CreateAndRetainArray(name, class, offset) \
	auto name = *reinterpret_cast<CCArray**>(reinterpret_cast<uintptr_t>(class) + offset); \
	name = CCArray::create(); \
	name->retain();

#define CreateAndRetainDict(name, class, offset) \
	auto name = *reinterpret_cast<CCDictionary**>(reinterpret_cast<uintptr_t>(class) + offset); \
	name = CCDictionary::create(); \
	name->retain();

static inline void (*removeO)(LevelEditorLayer* self,GameObject *,bool);
void LevelEditorLayerExt::removeH(GameObject * obj,bool a1) {
	
	removeO(this, obj, a1);
	obj->removeFromParent();
}


bool (*initO)( LevelEditorLayer* p, GJGameLevel* level );
bool LevelEditorLayerExt::initH(GJGameLevel* level)
{
	if ( !dynamic_cast< GJBaseGameLayer* >( this )->init() )
		return false;

	auto gm = GameManager::sharedState( );
	MEMBERBYOFFSET(bool, gm, 0x299) = true;
	/*this->ignoreDamage_ = gm->getGameVariable( "0009");
	this->unk_bool_01 = gm->getGameVariable( "0001");
	this->unk_bool_02 = false; //orbs
	this->unk_bool_03 = gm->getGameVariable( "0045");
	this->unk_bool_04 = gm->getGameVariable( "0038");
	this->unk_bool_06 = gm->getGameVariable( "0043");
	this->unk_bool_07 = gm->getGameVariable( "0037");
	this->unk_bool_08 = gm->getGameVariable( "0058");
	this->unk_bool_09 = gm->getGameVariable( "0013");
	this->unk_bool_10 = gm->getGameVariable( "0036");
	this->unk_bool_10 = gm->getGameVariable( "0078");
	this->unk_bool_11 = gm->getGameVariable( "0120");
	this->unk_bool_05 = gm->getGameVariable( "0079");
	this->unk_bool_12 = true; //layer locking
	this->unk_bool_13 = gm->getGameVariable( "0104");*/

	this->setObjectCount(0);

	GSM->stopBackgroundMusic();

	this->_gameLevel() = level;
	this->_gameLevel()->retain();

	MEMBERBYOFFSET(LevelEditorLayer*, gm, 0x168) = this;
	MEMBERBYOFFSET(LevelEditorLayer*, gm, 0x16C) = this;
	this->retain();
	/*this->arr_03 = CCArray::create();
	this->arr_03->retain();
	this->arr_01 = CCArray::create();
	this->arr_01->retain();
	this->arr_02 = CCArray::create();
	this->arr_02->retain();


CCLog("ddd");

	this->arr_2848 = CCArray::create();
	this->arr_2848->retain();
	this->arr_2832 = CCArray::create();
	this->arr_2832->retain();
	this->arr_2833 = CCArray::create();
	this->arr_2833->retain();
	this->arr_2802 = CCArray::create();
	this->arr_2802->retain();
	this->arr_2803 = CCArray::create();
	this->arr_2803->retain();
	this->arr_2863 = CCArray::create();
	this->arr_2863->retain();
	this->arr_2816 = CCArray::create();
	this->arr_2816->retain();
	this->arr_2795 = CCArray::create();
	this->arr_2795->retain();
	this->arr_2805 = CCArray::create();
	this->arr_2805->retain();
	this->arr_2799 = CCArray::create();
	this->arr_2799->retain();
	this->arr_2800 = CCArray::create();
	this->arr_2800->retain();
	this->arr_2797 = CCArray::create();
	this->arr_2797->retain();
	this->arr_2796 = CCArray::create();
	this->arr_2796->retain();
	this->arr_2798 = CCArray::create();
	this->arr_2798->retain();
	this->arr_2801 = CCArray::create();
	this->arr_2801->retain();
	this->arr_2818 = CCArray::create();
	this->arr_2818->retain();
	this->arr_2807 = CCArray::create();
	this->arr_2807->retain();
	this->arr_2821 = CCArray::create();
	this->arr_2821->retain();*/

	this->_array1() = CCArray::create();
	this->_array1()->retain();

	this->_array2() = CCArray::create();
	this->_array2()->retain();

	this->_array3() = CCArray::create();
	this->_array3()->retain();

	this->_allObjects() = CCArray::create();
	this->_allObjects()->retain();

	this->_unkArray1() = CCArray::create();
	this->_unkArray1()->retain();

	CreateAndRetainArray(objArr1, this, 0x2C00);
	CreateAndRetainArray(objArr2, this, 0x2BF8);
	CreateAndRetainArray(objArr3, this, 0x2BF0);
	CreateAndRetainArray(objArr4, this, 0x2BEC);
	CreateAndRetainArray(objArr5, this, 0x2BE4);
	CreateAndRetainArray(objArr6, this, 0x2BE8);
	CreateAndRetainArray(objArr7, this, 0x2C34);
	CreateAndRetainArray(objArr8, this, 0x2C08);
	CreateAndRetainArray(objArr9, this, 0x2BE0);
	CreateAndRetainArray(objArr10, this, 0x354);
	CreateAndRetainArray(objArr11, this, 0x350);
	CreateAndRetainArray(objArr12, this, 0x34C);
	CreateAndRetainArray(objArr13, this, 0x2C3C);
	CreateAndRetainArray(objArr14, this, 0x2C50);

	CreateAndRetainDict(objDict1, this, 0x46C);
	CreateAndRetainDict(objDict2, this, 0x43C);
	CreateAndRetainArray(objDict3, this, 0x2CB4);

	//this->_crashArray1() = CCArray::create();
	//this->_crashArray1()->retain();

	
	//this->_stickyGroupsDict() = CCDictionary::create();
	//this->_stickyGroupsDict()->retain();

	//this->_objectsToUpdate() = CCArray::create();
	//this->_objectsToUpdate()->retain();

	//*((bool *)this + 316) = *((bool *)level + 393);

	this->_unkVector1().reserve(9999);
	this->_unkVector2().reserve(9999);
	this->_objectsVector().reserve(9999);
	this->_lockedLayers().reserve(9999);

	for ( size_t i = 0; i < 9999; ++i )
	{
		//CCLog("loop: %d", i);
		//this->objectsVector_[ i ] = nullptr;
		this->_objectsVector()[i] = nullptr;

		this->_unkVector1()[i] = 0;
		this->_unkVector2()[i] = false;
		this->_lockedLayers()[i] = false;
		/*CCLog("loop stage 2");

		this->vector_2853[ i ] = 0;
		CCLog("loop stage 3");
		this->vector_2873[ i ] = 0;
		CCLog("loop stage 4");
		this->vector_2868[ i ] = false;
		CCLog("loop stage 5");
		this->vector_2858[ i ] = false;
		CCLog("loop stage 6");
		//*(bool *)(*((int *)p + 2878) + i) = 0;
		this->vector_2878[ i ] = 0;
		CCLog("loop stage 7");
		this->vector_2881[ i ] = 0;
		CCLog("loop stage 8");
		this->_something()[i] = 0;
		CCLog("loop stage 9");*/
	}

	this->_obb2d() = OBB2D::create( CCPoint( 1, 1 ), 1.0, 1.0, 0.0 );
	this->_obb2d()->retain( );

	this->_drawNodes() = new int[ 0xC80 ];
	this->setupLayers();

	this->_gridLayer() = DrawGridLayer::create( this->_gameLayer(), this );

	this->_gameLayer()->addChild( this->_gridLayer(), gm->getGameVariable( "0039" ) ? 99 : -100 );

	this->createPlayer();

	/**(bool *)(*((int *)this + 204) + 1556) = false;
	*(bool *)(*((int *)this + 205) + 1556) = false;

	if(GM->getGameVariable("0114")){
		*(bool *)(*((int *)this + 204) + 3008) = true;
		*(bool *)(*((int *)this + 205) + 3008) = true;
	}*/


	/*this->unk_arr_209 = CCArray::create();
	this->unk_arr_209->retain();
	this->unk_arr_211 = CCArray::create();
	this->unk_arr_211->retain();
	this->arr_260 = CCArray::create();
	this->arr_260->retain();*/

	this->_dCross() = CCSprite::createWithSpriteFrameName( "d_cross_01_001.png" );
	this->_batchNode()->addChild( this->_dCross(), 10 );
	this->_dCross()->setColor( ccc3( 255, 0, 0 ) );
	this->_dCross()->setVisible( false );
	this->_dCross()->setScale( 0.7 );

	std::string a = ZipUtils::decompressString( std::string( ( char* ) this->_gameLevel()->_levelString() ), false, 11 );

	this->_levelString() = new std::string(a);

	this->createObjectsFromSetup(a);

	this->createTextLayers();
	if ( GameManager::sharedState()->getGameVariable( "0066") == 1 )
		this->enableHighCapacityMode();

	if ( !this->_levelSettings() )
	{
		this->_levelSettings() = LevelSettingsObject::create( );
		this->_levelSettings()->_gameLevel() = this->_gameLevel();

		this->_levelSettings()->retain();
	}

	this->_editorUI() = EditorUI::create(this);
	this->addChild(this->_editorUI(), 100);

	//put the editor in the last position/zoom
	//this->gameLayer_->setPosition(this->_level->lastCameraPos_);
	//if(this->_level->lastEditorZoom != 0.0)
	//this->gameLayer_->setScale(this->_level->lastEditorZoom);

	this->_gridLayer()->updateTimeMarkers();

	// create ground, middleground and background
	//this->createBackground(MEMBERBYOFFSET(int, this->levelSettings_, 0x11C));
	//this->createMiddleground(MEMBERBYOFFSET(int, this->levelSettings_, 0x128));
    //this->createGroundLayer(
	//	MEMBERBYOFFSET(int, this->levelSettings_, 0x120),
	//	MEMBERBYOFFSET(int, this->levelSettings_, 0x148)
	//);

	this->createBackground(1);
	this->createGroundLayer(false, false);

    this->_editorUI()->updateSlider();

    this->resetGroupCounters(false);
	//this->sortStickyGroups();

    //this->updateEditorMode();

    //this->schedule(schedule_selector(LevelEditorLayer::updateEditor));

    // fix to the glitched background with updateEditor
    //MEMBERBYOFFSET(bool, this, 0x29E9) = true;

	// fix playtest pause
	//MEMBERBYOFFSET(int, this, 0x2C58) = 0;
	//CCLog("6666");


	return true;
}

static inline void (*updateVisibilityO)(LevelEditorLayer*,float);
void LevelEditorLayerExt::updateVisibilityH(float a1){

	auto p = this;
	
	/*int cl = MBO(int, p, 0x2C1C);
	
	
	if(p->isLayerLocked(cl)) {
		p->toggleLockActiveLayer();
		p->_editorUI()->updateGroupIDLabel();
	}*/
	
	/*auto node = MEMBERBYOFFSET(CCNode*, this, 0x11C);
	
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
	auto sections = MEMBERBYOFFSET(CCArray*, this, 0x348);*/

	/*
		UPDATING OBJECTS
	*/

	auto sections = MEMBERBYOFFSET(CCArray*, this, 0x348);

	for(int i = 0;  i <= 9999; i++){
		if(i >= 0 && i < sections->count()){
			auto section = sections->objectAtIndex(i);
			if(section){
				auto sect = reinterpret_cast<CCArray*>(section);
				if (sect) {
					if (sect->count() > 0) {
						for (int k = 0; k < sect->count(); k++) {
							GameObject *obj = dynamic_cast<GameObject *>(sect->objectAtIndex(k));
							auto objectPos = obj->getPosition();
							//if(rect.containsPoint(objectPos)){
								obj->addMainSpriteToParent(false);
								if(obj->hasSecondaryColor()) obj->addColorSpriteToParent(true);
								obj->activateObject();
								/*
								int currentLayer = MBO(int, p, 0x2C1C);
								int l1 = MBO(int, obj, 0x450);
								int l2 = MBO(int, obj, 0x454);
								
								extern void* GameObjectSetOpacityH(GameObject* self, unsigned char opacity);

								// new option "Hide invisible"
								if(MBO(bool, obj, 0x4AF) && GM->getGameVariable("0121")) {
									GameObjectSetOpacityH(obj, 0);
								}
								else {
									bool shouldBeVisible = (currentLayer == l1 || (currentLayer == l2 && l2 != 0) || currentLayer == -1);
									GameObjectSetOpacityH(obj, shouldBeVisible ? 255 : 70);
								}*/
								
								/*if(!MEMBERBYOFFSET(bool, obj, 0x405)) {
									//p->_objectsToUpdate()->addObject(obj);
								}        */                        
							/*}
							// GOOFY AHH :trollskullirl:
							else{
								obj->deactivateObject(false);
							}*/

						}
					}
				}
			}
		}
	}

	//p->updateObjectColors(p->_objectsToUpdate());
	//p->_objectsToUpdate()->removeAllObjects();
	
	p->processAreaVisualActions();
	p->sortBatchnodeChildren(0.0);
}

FUNCTIONHOOK(void, GJBaseGameLayer_updateCameraBGArt, GJBaseGameLayer* self, CCPoint pos) {
	// fix playtest bg
	MEMBERBYOFFSET(bool, self, 0x2A19) = true;

	GJBaseGameLayer_updateCameraBGArtO(self, pos);
}

void LevelEditorLayerExt::ApplyHooks() {
	
	HOOK_STATIC("_ZN16LevelEditorLayer4initEP11GJGameLevelb",
	LevelEditorLayerExt::initH, LevelEditorLayerExt::initO);
	
	//HOOK_STATIC("_ZN16LevelEditorLayer16updateVisibilityEf",
	//LevelEditorLayerExt::updateVisibilityH, LevelEditorLayerExt::updateVisibilityO);

	//HOOK_STATIC("_ZN15GJBaseGameLayer17updateCameraBGArtEN7cocos2d7CCPointE", GJBaseGameLayer_updateCameraBGArtH, GJBaseGameLayer_updateCameraBGArtO);

	/*
	HOOK_STATIC("_ZN16LevelEditorLayer12removeObjectEP10GameObjectb", 
	LevelEditorLayerExt::removeH, LevelEditorLayerExt::removeO);
	*/
}