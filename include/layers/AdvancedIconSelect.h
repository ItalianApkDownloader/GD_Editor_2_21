#pragma once
#ifndef GDPSEDITOR_ADVANCEDICONSELECT_H
#define GDPSEDITOR_ADVANCEDICONSELECT_H

#include "cocos2d.h"
#include "gd.h"


class AdvancedIconSelect : public CCLayer {
	
	//swings

    CCSprite* select;
	// CCLabelBMFont* robtop;
	// CCLabelBMFont* RetroAdvance64;
	// CCLabelBMFont* Ighby;
	CCLabelBMFont* Flushii;
	CCLabelBMFont* Luisillo;
	CCLabelBMFont* Edialex;
	// CCLabelBMFont* Rob_buck;
	// CCLabelBMFont* Chase97;
	// CCLabelBMFont* Aerma;
	// CCLabelBMFont* Aleiz21;
	// CCLabelBMFont* ML500;
	CCLabelBMFont* plus;
	// CCLabelBMFont* NHJ;
	// CCLabelBMFont* Dahila;
	
	CCMenuItemSpriteExtra* robtopBtn;
	CCMenuItemSpriteExtra* RetroAdvance64Btn;
	CCMenuItemSpriteExtra* Rob_buckBtn;
	CCMenuItemSpriteExtra* Chase97Btn;
	CCMenuItemSpriteExtra* AermaBtn;
	CCMenuItemSpriteExtra* Aleiz21Btn;
	CCMenuItemSpriteExtra* ML500Btn;
	CCMenuItemSpriteExtra* IghbyBtn;
	CCMenuItemSpriteExtra* NHJBtn;
	CCMenuItemSpriteExtra* DahilaBtn;
	// CCMenuItemSpriteExtra* FlushiiBtn;
	// CCMenuItemSpriteExtra* LuisilloBtn;
	// CCMenuItemSpriteExtra* plusBtn;
	CCMenuItemSpriteExtra* PlayDashBtn;
	CCMenuItemSpriteExtra* HenyxBtn;
	CCMenuItemSpriteExtra* FalsoBtn;
	CCMenuItemSpriteExtra* GhostPowerBtn;
	CCMenuItemSpriteExtra* SenpaiShikaBtn;
	CCMenuItemSpriteExtra* SprintBtn;

	

public:
    bool init();
	
	    virtual void keyBackClicked() { this->onBack(nullptr); }

	
	void deleteAll() {
		
		robtopBtn->setPosition(0,100000);
	 RetroAdvance64Btn->setPosition(0,100000);
		 IghbyBtn->setPosition(0,100000);
		 // FlushiiBtn->setPosition(0,100000);
		// LuisilloBtn->setPosition(0,100000);
		// EdialexBtn->setPosition(0,100000);
		
				 Flushii->setPosition(0,100000);
		Luisillo->setPosition(0,100000);
		 Edialex->setPosition(0,100000);
		
		 Rob_buckBtn->setPosition(0,100000);
		 Chase97Btn->setPosition(0,100000);
		 AermaBtn->setPosition(0,100000);
		 Aleiz21Btn->setPosition(0,100000);
		 ML500Btn->setPosition(0,100000);
		//plusBtn->setPosition(0,100000);
		plus->setPosition(0,100000);
		
		 NHJBtn->setPosition(0,100000);
		 DahilaBtn->setPosition(0,100000);
		PlayDashBtn->setPosition(0,100000);
		HenyxBtn->setPosition(0,100000);
		FalsoBtn->setPosition(0,100000);
		GhostPowerBtn->setPosition(0,100000);
		SenpaiShikaBtn->setPosition(0,100000);
		SprintBtn->setPosition(0,100000);
		
	}
	
	void onEmpty(CCObject* sender) {}
	
	//swings
	
	void onRobtop(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/RobTopGames");

	app->openURL(url);
	}
	
		void onRetroAdvance64(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/RetroAdvance64");

	app->openURL(url);
	}
	
		void onIghby(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/Ighby_Guy");

	app->openURL(url);
	}
	
		void onRob_buck(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://www.youtube.com/channel/UCienYQplPSwWfC7G4DHaZFg");

	app->openURL(url);
	}
	
		void onChase97(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://www.youtube.com/channel/UCyba8QmmlDzYy2BFbKnV4QA");

	app->openURL(url);
	}
	
		void onAerma(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://www.youtube.com/c/Aerma");

	app->openURL(url);
	}
	
		void onAleiz21(CCObject* sender) {
	
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/Aleiz21");

	app->openURL(url);
	}
	
			void onML500(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/MasterL500");

	app->openURL(url);
	}
	
			void onNHJ(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/NHJ__NHJ");

	app->openURL(url);
	}
	
			void onDahila(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/MiyuSuou");

	app->openURL(url);
	}
		

			void onPlayDash(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/ItzTheDash");

	app->openURL(url);
	}
	
				void onHenyx(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://www.youtube.com/channel/UCgPZAAsnYpPfzpil55Hta0w");

	app->openURL(url);
	}
	
					void onFalso(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/FalsoMashups");

	app->openURL(url);
	}
	
						void onGhostPower(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://www.youtube.com/channel/UCC_S4bHTIGVJiCfZsKU15Gw");

	app->openURL(url);
	}						void onSenpaiShika(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/SenpaiShika");

	app->openURL(url);
	}
	
	void onSprint(CCObject* sender) {
		
		        auto app = cocos2d::CCApplication::sharedApplication( );
        auto url = ("https://twitter.com/SenpaiShika");

	app->openURL(url);
	}
		
		
    void onSwingClick(CCObject * sender);
	void onJetPackClick(CCObject * sender);

    void onSwing(CCObject*)
    {
		    GameManager::sharedState()->setIntGameVariable("7000", 1);
			
        auto scene = CCScene::create();
        scene->addChild(AdvancedIconSelect::create(), 100);

        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
    }

    void onJetpack(CCObject*)
    {

    GameManager::sharedState()->setIntGameVariable("7000", 2);
				
                auto scene = CCScene::create();
        scene->addChild(AdvancedIconSelect::create(), 100);

        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
    }

    void onOldIcons(CCObject*)
    {
        FLAlertLayer::create(nullptr, "Original Icons", "You can change to original icons in GDPS Settings\n<cr>Options -> GDPS Settings -> Original Textures</c>", "OK", nullptr, 400, false, 200)->show();

        //this will be used for a guide, saving here xd
        //FLAlertLayer::create(nullptr, "The Leak", "Geometry Dash is a semi-platformer game published by RobTop Games in 2013. It was initially released on iOS and Android, then published to Steam in 2014. The game consists of the player controlling a square in a horizontal platformer along with multiple game modes, icons and more. The most notable feature is the editor, which allows sharing user made levels onto the servers.\n\nIn the years since there have been multiple updates and several free games that have been published. The three side games that have been published are: World, Meltdown, and SubZero. The game also has a lite version which can also be downloaded for free.\n\nThe latest update to the game was 2.1, which was published in 2017. In the multiple years that have passed since, the next update, 2.2, has not released (as of July 9th, 2021). However, the side games do contain extra functionality leaked (whether intentional or not), however the latest dates back to 2017.", "OK", nullptr, 400, true, 300)->show();
    }
    void onBack(CCObject*)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GJGarageLayer::scene()));
    }
	
	void onJetpackComingSoon(CCObject*)
    {
        FLAlertLayer::create(nullptr, "Jetpack", "Coming soon!", "OK", nullptr)->show();

        //this will be used for a guide, saving here xd
        //FLAlertLayer::create(nullptr, "The Leak", "Geometry Dash is a semi-platformer game published by RobTop Games in 2013. It was initially released on iOS and Android, then published to Steam in 2014. The game consists of the player controlling a square in a horizontal platformer along with multiple game modes, icons and more. The most notable feature is the editor, which allows sharing user made levels onto the servers.\n\nIn the years since there have been multiple updates and several free games that have been published. The three side games that have been published are: World, Meltdown, and SubZero. The game also has a lite version which can also be downloaded for free.\n\nThe latest update to the game was 2.1, which was published in 2017. In the multiple years that have passed since, the next update, 2.2, has not released (as of July 9th, 2021). However, the side games do contain extra functionality leaked (whether intentional or not), however the latest dates back to 2017.", "OK", nullptr, 400, true, 300)->show();
    }

    CREATE_FUNC(AdvancedIconSelect)
};

#endif //GDPSEDITOR_COLOURSPALETTE_H
