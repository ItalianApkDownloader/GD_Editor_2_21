#include <gd.h>
#include "cocos2d.h"
#include "LevelEditorLayerExt.h"
#include "hooking.h"

static inline void (*removeO)(LevelEditorLayer* self,GameObject *,bool);
void LevelEditorLayerExt::removeH(GameObject * obj,bool a1) {
	
	removeO(this, obj, a1);
	obj->removeFromParent();
}


bool (*initO)( LevelEditorLayer* p, GJGameLevel* level );
bool LevelEditorLayerExt::initH(GJGameLevel* level)
{
	if ( !dynamic_cast< GJBaseGameLayer* >( this )->init( ) )
	return false;






	auto gm = GameManager::sharedState( );
	*((bool *)gm + 442) = true;
	this->ignoreDamage_ = gm->getGameVariable( "0009");
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
	this->unk_bool_13 = gm->getGameVariable( "0104");

	this->setObjectCount( 0 );

	GSM->stopBackgroundMusic( );

	this->_level = level;
	this->_level->retain();

	gm->playLayer_ = this;
	this->retain();
	this->arr_03 = CCArray::create();
	this->arr_03->retain();
	this->arr_01 = CCArray::create();
	this->arr_01->retain();
	this->arr_02 = CCArray::create();
	this->arr_02->retain();




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
	this->arr_2821->retain();
	
	
	this->_crashArray1() = CCArray::create();
	this->_crashArray1()->retain();
	
	this->_stickyGroupsDict() = CCDictionary::create();
	this->_stickyGroupsDict()->retain();

	this->_objectsToUpdate() = CCArray::create();
	this->_objectsToUpdate()->retain();

	*((bool *)this + 316) = *((bool *)level + 393);


	this->vector_2868.reserve( 9999 );
	this->vector_2858.reserve( 9999 );
	this->vector_2878.reserve( 9999 );
	this->vector_2853.reserve( 9999 );
	this->vector_2873.reserve( 9999 );
	this->vector_2881.reserve( 9999 );
	this->_something().reserve( 9999 );
	this->objectsVector_.reserve( 9999 );


	//this->objectVector_.reserve( 9999 );


	for ( size_t i = 0; i < 9999; ++i )
	{
		this->objectsVector_[ i ] = nullptr;


		this->vector_2853[ i ] = 0;
		this->vector_2873[ i ] = 0;
		this->vector_2868[ i ] = false;
		this->vector_2858[ i ] = false;
		//*(bool *)(*((int *)p + 2878) + i) = 0;
		this->vector_2878[ i ] = 0;
		this->vector_2881[ i ] = 0;
		this->_something()[i] = 0;
	}


	this->obb2d2_ = OBB2D::create( CCPoint( 1, 1 ), 1.0, 1.0, 0.0 );
	this->obb2d2_->retain( );
	this->weirdAlloc = new int[ 0xC80 ];
	this->setupLayers( );

	this->gridLayer_ = DrawGridLayer::create( this->gameLayer_, this );

	this->gameLayer_->addChild( this->gridLayer_, gm->getGameVariable( "0039" ) ? 99 : -100 );

	this->createPlayer();
	*(bool *)(*((int *)this + 204) + 1556) = false;
	*(bool *)(*((int *)this + 205) + 1556) = false;

	if(GM->getGameVariable("0114")){
		*(bool *)(*((int *)this + 204) + 3008) = true;
		*(bool *)(*((int *)this + 205) + 3008) = true;
	}

	this->unk_arr_209 = CCArray::create();
	this->unk_arr_209->retain();
	this->unk_arr_211 = CCArray::create();
	this->unk_arr_211->retain();
	this->arr_260 = CCArray::create();
	this->arr_260->retain();


	this->dCross_ = CCSprite::createWithSpriteFrameName( "d_cross_01_001.png" );
	this->batchNode_->addChild( this->dCross_, 10 );
	this->dCross_->setColor( ccc3( 255, 0, 0 ) );
	this->dCross_->setVisible( false );
	this->dCross_->setScale( 0.7 );


	std::string a = ZipUtils::decompressString( std::string( ( char* ) this->_level->levelString_ ), false, 11 );

	this->levelString_ = new std::string( a );

	this->createObjectsFromSetup( a );

	this->createTextLayers();
	if ( GameManager::sharedState()->getGameVariable( "0066") == 1 )
	this->enableHighCapacityMode();

	if ( !this->levelSettings_ )
	{
		this->levelSettings_ = LevelSettingsObject::create( );
		this->levelSettings_->gameLevel_ = this->_level;

		this->levelSettings_->retain( );
	}
	this->editorUI_ = EditorUI::create( this );
	this->addChild( this->editorUI_, 100 );


	//put the editor in the last position/zoom
	this->gameLayer_->setPosition(this->_level->lastCameraPos_);
	if(this->_level->lastEditorZoom != 0.0)
	this->gameLayer_->setScale(this->_level->lastEditorZoom);


	this->editorUI_->updateSlider();
	this->createGroundLayer(true,true);
	this->gridLayer_->updateTimeMarkers();
	this->createBackground(0);
	this->createBackground(1);
	this->editorUI_->updateSlider( );

	this->resetGroupCounters(false);
	this->sortStickyGroups();
	this->updateEditorMode();
	// this->schedule( schedule_selector( LevelEditorLayer::updateVisibility ));
	this->schedule( schedule_selector( LevelEditorLayer::updateEditor ));

	//fix to the glitched background with updateEditor
	this->editorUI_->onStopPlaytest(nullptr);
	


	return true;
}

static inline void (*updateVisibilityO)(LevelEditorLayer*,float);
void LevelEditorLayerExt::updateVisibilityH(float a1){

	auto p = this;
	
	int cl = MBO(int, p, 0x2C1C);
	
	
	if(p->isLayerLocked(cl)) {
		p->toggleLockActiveLayer();
		p->editorUI_->updateGroupIDLabel();
	}
	
	
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
								
								extern void* GameObjectSetOpacityH(GameObject* self, unsigned char opacity);
								GameObjectSetOpacityH(obj, shouldBeVisible ? 255 : 70);
								//for later GameObjectExt::setOpacityH(obj, shouldBeVisible ? 255 : 70);
								
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

void LevelEditorLayerExt::ApplyHooks() {
	
	HOOK_STATIC("_ZN16LevelEditorLayer4initEP11GJGameLevelb",
	LevelEditorLayerExt::initH, LevelEditorLayerExt::initO);
	
	HOOK_STATIC("_ZN16LevelEditorLayer16updateVisibilityEf",
	LevelEditorLayerExt::updateVisibilityH, LevelEditorLayerExt::updateVisibilityO);
	
	HOOK_STATIC("_ZN16LevelEditorLayer12removeObjectEP10GameObjectb", 
	LevelEditorLayerExt::removeH, LevelEditorLayerExt::removeO);
	
}