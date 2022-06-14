#include <gd.h>
#include "cocos2d.h"
#include "LevelEditorLayerExt.h"

#define _BYTE bool
#define _DWORD int;

bool LevelEditorLayerExt::init_hk(GJGameLevel *level)
{
    LevelEditorLayer *ptr = this;

    /*     if( !dynamic_cast< GJBaseGameLayer* >( ptr )->init( ) )
            return false;
     */
    ptr->quickUpdatePos_ = true;
    ptr->firstThing2 = 1;

    auto gm = GameManager::sharedState();
    gm->inEditor_ = true;

    ptr->smoothFix_ = gm->getGameVariable("0102");

    ptr->ignoreDamage_ = gm->getGameVariable("0009");
    ptr->followPlayer_ = gm->getGameVariable("0001");
    ptr->drawTriggerBoxes_ = gm->getGameVariable("0044");
    ptr->debugDraw_ = gm->getGameVariable("0045");
    ptr->toggleEditorGrid_ = gm->getGameVariable("0038");
    ptr->effectLines_ = gm->getGameVariable("0043");
    ptr->editorGround_ = gm->getGameVariable("0037");
    ptr->toggleEffectDuration = gm->getGameVariable("0058");
    ptr->maxUndo_ = gm->getGameVariable("0013");
    ptr->hasColor_ = gm->getGameVariable("0036");
    ptr->toggleEditorBG_ = gm->getGameVariable("0078");
    ptr->hideGrid_ = gm->getGameVariable("0079");
    ptr->layerLocking_ = gm->getGameVariable("0103");
    ptr->recordOrder_ = gm->getGameVariable("0104");

    ptr->setObjectCount(0);
    GameSoundManager::sharedManager()->stopBackgroundMusic();

    ptr->level = level;
    gm->editorLayer = ptr;

    ptr->level->retain();

    ptr->stickyObjectDict_ = CCDictionary::create();
    ptr->stickyObjectDict_->retain();

    ptr->someArray1 = CCArray::create();
    ptr->someArray1->retain();

    ptr->someArray2 = CCArray::create();
    ptr->someArray2->retain();

    ptr->someArray3 = CCArray::create();
    ptr->someArray3->retain();

    ptr->dict2 = CCDictionary::create();
    ptr->dict2->retain();

    ptr->dict3 = CCDictionary::create();
    ptr->dict3->retain();

    ptr->dict4 = CCDictionary::create();
    ptr->dict4->retain();

    ptr->someArray4 = CCArray::create();
    ptr->someArray4->retain();

    ptr->someArray5 = CCArray::create();
    ptr->someArray5->retain();

    ptr->someArray6 = CCArray::create();
    ptr->someArray6->retain();

    ptr->someArray7 = CCArray::create();
    ptr->someArray7->retain();

    ptr->someArray8 = CCArray::create();
    ptr->someArray8->retain();

    ptr->dict5 = CCDictionary::create();
    ptr->dict5->retain();

    ptr->someArray9 = CCArray::create();
    ptr->someArray9->retain();

    ptr->someArray10 = CCArray::create();
    ptr->someArray10->retain();

    ptr->arrayWithCap = CCArray::createWithCapacity(100);
    ptr->arrayWithCap->retain();

    ptr->dict6 = CCDictionary::create();
    ptr->dict6->retain();

    ptr->someArray11 = CCArray::create();
    ptr->someArray11->retain();

    ptr->someArray12 = CCArray::create();
    ptr->someArray12->retain();

    ptr->dict7 = CCDictionary::create();
    ptr->dict7->retain();

    ptr->someArray13 = CCArray::create();
    ptr->someArray13->retain();

    ptr->redoList_ = CCArray::create();
    ptr->redoList_->retain();

    ptr->crashThing = CCArray::create();
    ptr->crashThing->retain();

    // new 2.2
    ptr->someArray14 = CCArray::create();
    ptr->someArray14->retain();

    ptr->someArray15 = CCArray::create();
    ptr->someArray15->retain();

    ptr->collisionBlocks_ = CCArray::create();
    ptr->collisionBlocks_->retain();

    // vectors :L

    ptr->objectVector_.reserve(9999);
    ptr->nestedArray1.reserve(9999);
    ptr->allGroupsToggled.reserve(9999);
    ptr->lockedLayersList_.reserve(9999);
    ptr->toggledGroups.reserve(9999);

    ptr->other1.reserve(9999);
    ptr->other2.reserve(9999);
    ptr->other4.reserve(9999);
    ptr->other3.reserve(9999);
    ptr->other5.reserve(9999);
    ptr->groupPreviewV.reserve(9999);

    for (int i = 0; i < 9999; i++)
    {
        ptr->objectVector_[i] = nullptr;
        ptr->nestedArray1[i] = nullptr;

        ptr->allGroupsToggled[i] = false;
        ptr->lockedLayersList_[i] = false;
        ptr->toggledGroups[i] = 0;

        ptr->other1[i] = false;
        ptr->other2[i] = false;
        ptr->other3[i] = false;
        ptr->other4[i] = nullptr;
        ptr->other5[i] = nullptr;
        ptr->groupPreviewV[i] = 0.0;
    }

    // continue
    ptr->obb2d_ = OBB2D::create(CCPoint(1, 1), 1.0, 1.0, 0.0);
    ptr->obb2d_->retain();

    ptr->drawNodes_ = new int[0xC80];

    dynamic_cast<GJBaseGameLayer *>(ptr)->setupLayers();

    ptr->drawGridLayer = DrawGridLayer::create(ptr->gameLayer_, ptr);
    ptr->gameLayer_->addChild(ptr->drawGridLayer, gm->getGameVariable("0039") ? 99 : -100);

    auto playwoo = PlayerObject::create(
        gm->playerFrame2 - gm->playerFrame1,
        gm->shipFrame2 - gm->shipFrame1,
        ptr->gameLayer_);

    auto prim_color = gm->colorForIdx(gm->primaryColorID_2 - gm->primaryColorID_1);
    auto secondary_color = gm->colorForIdx(gm->secondaryColorID_2 - gm->secondaryColorID_1);

    // playwoo->__unknown11();
    // playwoo->setColor( prim_color );
    playwoo->setSecondColor(secondary_color);
    playwoo->updateGlowColor();

    ptr->batchNodePlayer_->addChild(playwoo, 10);
    playwoo->field304 = 1;

    playwoo->setVisible(false);

    auto playwoo2 = PlayerObject::create(
        gm->playerFrame2 - gm->playerFrame1,
        gm->shipFrame2 - gm->shipFrame1,
        ptr->gameLayer_);

    // playwoo2->setColor( secondary_color );
    playwoo2->setSecondColor(prim_color);
    playwoo2->updateGlowColor();
    playwoo2->retain();

    // playwoo2->field6f8 = 1;
    // playwoo2->field304 = 2;

    *((int *)playwoo2 + 772) = 2;
    *((int *)playwoo2 + 1784) = 1;

    // playwoo2->setOpacity( 0 );

    ptr->dCross_ = CCSprite::createWithSpriteFrameName("d_cross_01_001.png");

    ptr->batchNode_->addChild(ptr->dCross_, 10);
    // ptr->dCross_->setColor( ccc3(255, 255, 255) );
    ptr->dCross_->setVisible(false); // false
    ptr->dCross_->setScale(0.7);

    //#include "../../include/ZLIB/zlib.h"
    //#include "../../include/Encryption/Encryption.h"

    std::string a = ZipUtils::decompressString(std::string((char *)ptr->level->levelString_), false, 11);
    ptr->oldLevelString_ = new std::string(a);

    ptr->createObjectsFromSetup(a);

    dynamic_cast<GJBaseGameLayer *>(ptr)->createTextLayers();

    if (gm->getGameVariable("0066"))
        dynamic_cast<GJBaseGameLayer *>(ptr)->enableHighCapacityMode();

    CCLog("Mod_Menu: before1! !!!");

    if (!ptr->settingsObject_)
    {
        ptr->settingsObject_ = LevelSettingsObject::create();
        ptr->settingsObject_->gameLevel_ = ptr->level;

        ptr->settingsObject_->retain();
    }

    CCLog("Mod_Menu: before2! !!!");

    auto c_action1 = ptr->settingsObject_->colorManager_->getColorAction(0x3ED);
    c_action1->thing1 = 0;
    c_action1->thing5 = 1;

    auto c_action2 = ptr->settingsObject_->colorManager_->getColorAction(0x3EE);
    c_action2->thing1 = 0;
    c_action2->thing5 = 1;

    CCLog("Mod_Menu: before3! !!!");
    EditorUI::create(ptr);
    CCLog("Mod_Menu: HERE! !!!");

    ptr->editorUI_ = EditorUI::create(ptr);
    CCLog("Mod_Menu: HERE! !!!");
    ptr->addChild(ptr->editorUI_, 100);

    ptr->createGroundLayer();
    ptr->drawGridLayer->updateTimeMarkers();
    ptr->createBackground();

    ptr->editorUI_->updateSlider();
    ptr->updateGroundWidth();

    dynamic_cast<GJBaseGameLayer *>(ptr)->resetGroupCounters(false);
    ptr->sortStickyGroups();

    ptr->levelSettingsUpdated();
    // ptr->updateEditorMode( );

    ptr->schedule(schedule_selector(LevelEditorLayer::updateVisibility), 0.05);
    ptr->schedule(schedule_selector(LevelEditorLayer::updateGround));

    ptr->firstThing2 = 0;

    CCLog("Mod_Menu: END !!!");
    return true;
}
