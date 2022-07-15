#include <gd.h>
#include "cocos2d.h"
#include "LevelEditorLayerExt.h"
#include "hooking.h"

#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

void CreateAndRetainArrayA(uintptr_t obj, int offset) {
	auto array = CCArray::create();
	array->retain();
	*reinterpret_cast<CCArray**>(obj + offset) = array;
}
#define CreateAndRetainArray(class, offset) CreateAndRetainArrayA(reinterpret_cast<uintptr_t>(class), offset);

void CreateAndRetainDictA(uintptr_t obj, int offset) {
	auto dict = CCDictionary::create();
	dict->retain();
	*reinterpret_cast<CCDictionary**>(obj + offset) = dict;
}
#define CreateAndRetainDict(class, offset) CreateAndRetainDictA(reinterpret_cast<uintptr_t>(class), offset);

static inline void (*removeO)(LevelEditorLayer* self,GameObject *,bool);
void LevelEditorLayerExt::removeH(GameObject * obj,bool a1) {
	
	removeO(this, obj, a1);
	obj->removeFromParent();
}


bool (*initO)( LevelEditorLayer* p, GJGameLevel* level );
bool LevelEditorLayerExt::initH(GJGameLevel* level)
{
	if (!dynamic_cast<GJBaseGameLayer*>(this)->init())
		return false;

	auto gm = GameManager::sharedState();
	gm->_inEditor() = true;

	MEMBERBYOFFSET(bool, this, 0x2780) = true;

	this->setObjectCount(0);

	GSM->stopBackgroundMusic();

	//////////////////////
	// updateOptions
	MBO(bool, this, 0x142) = gm->getGameVariable("0009");
	MBO(bool, this, 0x2BC0) = gm->getGameVariable("0001");
	MBO(bool, this, 0x2BC1) = gm->getGameVariable("0044");
	MBO(bool, this, 0x2BC2) = gm->getGameVariable("0045");
	MBO(bool, this, 0x2BC3) = gm->getGameVariable("0038");
	MBO(bool, this, 0x2BC5) = gm->getGameVariable("0043");
	MBO(bool, this, 0x2BC6) = gm->getGameVariable("0037");
	MBO(bool, this, 0x2BC7) = gm->getGameVariable("0058");
	MBO(bool, this, 0x2BC8) = gm->getGameVariable("0013");
	MBO(bool, this, 0x2C91) = gm->getGameVariable("0036");
	MBO(bool, this, 0x2BC9) = gm->getGameVariable("0078");
	MBO(bool, this, 0x2BCA) = gm->getGameVariable("0120");
	MBO(bool, this, 0x2BC4) = gm->getGameVariable("0079");
	MBO(bool, this, 0x2BCB) = gm->getGameVariable("0103");
	MBO(bool, this, 0x2BCC) = gm->getGameVariable("0104");

	//////////////////////

	this->_gameLevel() = level;
	this->_gameLevel()->retain();

	MEMBERBYOFFSET(LevelEditorLayer*, gm, 0x16C) = this;
	this->retain();

	CreateAndRetainArray(this, 0x2C00);
	CreateAndRetainArray(this, 0x2BF8); //here
	CreateAndRetainArray(this, 0x2BF0);
	CreateAndRetainArray(this, 0x2BEC);
	CreateAndRetainArray(this, 0x2BE4);
	CreateAndRetainArray(this, 0x2BE8);
	CreateAndRetainArray(this, 0x2C08);
	CreateAndRetainArray(this, 0x2BE0);
	CreateAndRetainArray(this, 0x354);
	CreateAndRetainArray(this, 0x350);
	CreateAndRetainArray(this, 0x34C);
	CreateAndRetainArray(this, 0x2C3C);
	CreateAndRetainArray(this, 0x2C50);
	CreateAndRetainArray(this, 0x2BFC);
	//CreateAndRetainArray(this, 0x2BF8); again?
	CreateAndRetainArray(this, 0x2BF4);
	CreateAndRetainArray(this, 0x2C34);
	CreateAndRetainArray(this, 0x348);
	CreateAndRetainArray(this, 0x344);
	CreateAndRetainArray(this, 0x2C74);
	CreateAndRetainArray(this, 0x2C78);
	CreateAndRetainArray(this, 0x2D50);

	CreateAndRetainDict(this, 0x46C);
	CreateAndRetainDict(this, 0x43C);
	CreateAndRetainDict(this, 0x2CB4);

	this->_unkVector1().reserve(9999);
	this->_unkVector2().reserve(9999);
	this->_unkVector3().reserve(9999);
	this->_unkVector4().reserve(9999);
	this->_unkVector5().reserve(9999);
	this->_objectsVector().reserve(9999);
	this->_lockedLayers().reserve(9999);
	this->_blendingVec().reserve(9999);
	this->_triggerGroupsVec().reserve(9999);

	this->_something().reserve(9999);

	for ( size_t i = 0; i < 9999; ++i )
	{
		this->_objectsVector()[i] = nullptr;

		this->_unkVector1()[i] = 0;
		this->_unkVector2()[i] = 0;
		this->_unkVector3()[i] = false;
		this->_unkVector4()[i] = 0;
		this->_unkVector5()[i] = false;
		this->_lockedLayers()[i] = false;
		this->_blendingVec()[i] = false;
		this->_triggerGroupsVec()[i] = nullptr;

		this->_something()[i] = 0;
	}

	this->_obb2d() = OBB2D::create( CCPoint( 1, 1 ), 1.0, 1.0, 0.0 );
	this->_obb2d()->retain( );

	this->_drawNodes() = new int[0xC80];
	this->setupLayers();

	this->_gridLayer() = DrawGridLayer::create( this->_gameLayer(), this );

	this->_gameLayer()->addChild( this->_gridLayer(), gm->getGameVariable( "0039" ) ? 99 : -100 );

	this->createPlayer();

	// questionmark
	MBO(bool, this->_player1(), 0x609) = true;
	MBO(bool, this->_player2(), 0x609) = true;

	MBO(bool, this->_player1(), 0x60C) = false;
	MBO(bool, this->_player2(), 0x60C) = false;

	this->_dCross() = CCSprite::createWithSpriteFrameName( "d_cross_01_001.png" );
	this->_batchNode()->addChild( this->_dCross(), 10 );
	this->_dCross()->setColor( ccc3( 255, 0, 0 ) );
	this->_dCross()->setVisible( false );
	this->_dCross()->setScale( 0.7 );

	std::string a = ZipUtils::decompressString( std::string( ( char* ) this->_gameLevel()->_levelString() ), false, 11 );

	this->_levelString() = new std::string(a);

	this->createObjectsFromSetup(a);

	this->createTextLayers();
	if (gm->getGameVariable("0066")) this->enableHighCapacityMode();

	if ( !this->_levelSettings() )
	{
		this->_levelSettings() = LevelSettingsObject::create( );
		this->_levelSettings()->_gameLevel() = this->_gameLevel();

		this->_levelSettings()->retain();
	}

	this->_editorUI() = EditorUI::create(this);
	this->addChild(this->_editorUI(), 100);

	//put the editor in the last position/zoom
	this->_gameLayer()->setPosition(this->_gameLevel()->lastCameraPos_);
	if(this->_gameLevel()->lastEditorZoom != 0.0)
	this->_gameLayer()->setScale(this->_gameLevel()->lastEditorZoom);

	this->_gridLayer()->updateTimeMarkers();

	// create ground, middleground and background
	this->createBackground(MEMBERBYOFFSET(int, this->_levelSettings(), 0x11C));
	this->createMiddleground(MEMBERBYOFFSET(int, this->_levelSettings(), 0x128));
    this->createGroundLayer(
		MEMBERBYOFFSET(int, this->_levelSettings(), 0x120),
		MEMBERBYOFFSET(int, this->_levelSettings(), 0x148)
	);

	// fix background
	MEMBERBYOFFSET(bool, this, 0x2A19) = true;

    this->_editorUI()->updateSlider();

    this->resetGroupCounters(false);

	this->sortStickyGroups();
	this->updateEditorMode();

    this->schedule(schedule_selector(LevelEditorLayer::updateEditor));

	this->updatePreviewAnim();
	this->updatePreviewParticles();

	return true;
}

FUNCTIONHOOK(void, GameObject_setOpacity, GameObject* self, unsigned char opacity) {
	GameObject_setOpacityO(self, opacity);
}

static inline void (*updateVisibilityO)(LevelEditorLayer*,float);
void LevelEditorLayerExt::updateVisibilityH(float a1) {
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
	int limit = (int)(float)(percentage + 1.0);*/

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

								// attempt at preview animation for animated objects
								//if(MBO(bool, obj, 0x47C)) obj->updateSyncedAnimation(MEMBERBYOFFSET(float, this, 0x28B0), -1);
								
								int currentLayer = MBO(int, p, 0x2C50);
								int l1 = MBO(int, obj, 0x450);
								int l2 = MBO(int, obj, 0x454);
								
								//extern void* GameObjectSetOpacityH(GameObject* self, unsigned char opacity);

								// new option "Hide invisible"
								if(MBO(bool, obj, 0x4AF) && GM->getGameVariable("0121")) {
									GameObject_setOpacityH(obj, 0);
								}
								else {
									bool shouldBeVisible = (currentLayer == l1 || (currentLayer == l2 && l2 != 0) || currentLayer == -1);
									GameObject_setOpacityH(obj, shouldBeVisible ? 255 : 70);
								}
								
								if(!MEMBERBYOFFSET(bool, obj, 0x405)) {
									this->_objectsToUpdate()->addObject(obj);
								}                                
							//}
							// GOOFY AHH :trollskullirl:
							/*else{
								obj->deactivateObject(false);
							}*/
						}
					}
				}
			}
		}
	}

	this->updateObjectColors(this->_objectsToUpdate());
	this->_objectsToUpdate()->removeAllObjects();
	
	this->processAreaVisualActions();
	this->sortBatchnodeChildren(0);
}

void LevelEditorLayerExt::ApplyHooks() {
	HOOK_STATIC("_ZN16LevelEditorLayer4initEP11GJGameLevelb",
	LevelEditorLayerExt::initH, LevelEditorLayerExt::initO);
	
	HOOK_STATIC("_ZN16LevelEditorLayer16updateVisibilityEf",
	LevelEditorLayerExt::updateVisibilityH, LevelEditorLayerExt::updateVisibilityO);

	HOOK_STATIC("_ZN10GameObject10setOpacityEh", GameObject_setOpacityH, GameObject_setOpacityO);

	/*
	HOOK_STATIC("_ZN16LevelEditorLayer12removeObjectEP10GameObjectb", 
	LevelEditorLayerExt::removeH, LevelEditorLayerExt::removeO);
	*/
}