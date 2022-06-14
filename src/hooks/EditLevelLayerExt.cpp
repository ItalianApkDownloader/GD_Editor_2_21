#include <dlfcn.h>
#include "blaze.h"
#include "cocos2d.h"
#include "EditLevelLayerExt.h"
#include "patch.h"

extern bool inEditor;
void EditLevelLayerExt::onBack_hk(CCObject *sender)
{
        inEditor = false;
        onBack_trp(this, sender);

        // patch *tmp = new patch();

        // tmp->addPatch("libcocos2dcpp.so", 0x44D2AE, "D7 F7 B5 FF"); // crash
        // tmp->Modify();
}

void EditLevelLayerExt::onEdit_hk(CCObject *sender)
{
        CCLog("Clicked! onEdit_hk");

        // btnMenu_->setVisible(false);
        /*
        auto scene = CCTransitionFade::create(
                0.5,
                LevelEditorLayer::scene( this->gameLevel_ ) );

        dir->pushScene( scene );
        //GameSoundManager::sharedManager()->playBackgroundMusic( this->gameLevel_->getAudioFileName( ), false, true, false );

        /*
        this->closeTextInputs( );

        this->isPlaying_ = true;

        this->verifyLevelName( );

        GameSoundManager::sharedManager()->playBackgroundMusic( this->gameLevel_->getAudioFileName( ), false, true, false );

        auto dir = CCDirector::sharedDirector( );
        auto scene = CCTransitionFade::create(
                0.5,
                LevelEditorLayer::scene( this->gameLevel_ ) );

        dir->pushScene( scene );
         */

        /*
        if ( !this->isPlaying_ )
        {
            this->closeTextInputs( );

            this->isPlaying_ = true;

            this->verifyLevelName( );

            GameSoundManager::sharedManager()->playBackgroundMusic( this->gameLevel_->getAudioFileName( ), false, true, false );

            auto dir = CCDirector::sharedDirector( );
            auto scene = CCTransitionFade::create(
                    0.5,
                    LevelEditorLayer::scene( this->gameLevel_ ) );

            dir->pushScene( scene );
        }
         */
}
