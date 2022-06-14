#include <gd.h>
#include <Logger.h>
#include <sstream>
#include "cocos2d.h"
#include "GameManager.h"
#include "rapidjson/rapidjson.h"
#include "patch.h"
#include "AdvancedIconSelect.h"
#include "SimplePlayer.h"
#include "GDPSManager.h"

#define version 0

bool AdvancedIconSelect::init()
{

	this->setKeypadEnabled(true);

	auto dir = CCDirector::sharedDirector();

	auto gm = GameManager::sharedState();

	auto page = CCString::createWithFormat("%i", gm->getIntGameVariable("7000"))->intValue();

	auto swn = gm->m_nPlayerSwing;

	select = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
	select->setScale(1.1);

	auto robtop = CCLabelBMFont::create("Robtop", "goldFont.fnt");
	auto Ighby = CCLabelBMFont::create("Ighby", "goldFont.fnt");
	auto RetroAdvance64 = CCLabelBMFont::create("RetroAdvance64", "goldFont.fnt");
	Flushii = CCLabelBMFont::create("Flushii", "goldFont.fnt");
	Luisillo = CCLabelBMFont::create("Luisillo", "goldFont.fnt");
	Edialex = CCLabelBMFont::create("Edialex", "goldFont.fnt");
	auto Rob_buck = CCLabelBMFont::create("Rob buck", "goldFont.fnt");
	auto Chase97 = CCLabelBMFont::create("Chase", "goldFont.fnt");
	auto Aerma = CCLabelBMFont::create("Aerma", "goldFont.fnt");
	auto Aleiz21 = CCLabelBMFont::create("Aleiz21", "goldFont.fnt");
	auto ML500 = CCLabelBMFont::create("ML500", "goldFont.fnt");
	plus = CCLabelBMFont::create("+", "goldFont.fnt");
	auto NHJ = CCLabelBMFont::create("NHJ", "goldFont.fnt");
	auto Dahila = CCLabelBMFont::create("Dahila", "goldFont.fnt");
	auto PlayDash = CCLabelBMFont::create("Play Dash", "goldFont.fnt");
	auto Henyx = CCLabelBMFont::create("Henyx", "goldFont.fnt");
	auto Falso = CCLabelBMFont::create("Falso", "goldFont.fnt");
	auto GhostPower = CCLabelBMFont::create("Ghost Power", "goldFont.fnt");
	auto SenpaiShika = CCLabelBMFont::create("SenpaiShika", "goldFont.fnt");
	auto Sprint = CCLabelBMFont::create("Sprint", "goldFont.fnt");

	robtopBtn = CCMenuItemSpriteExtra::create(robtop, robtop, this, menu_selector(AdvancedIconSelect::onRobtop));
	RetroAdvance64Btn = CCMenuItemSpriteExtra::create(RetroAdvance64, RetroAdvance64, this, menu_selector(AdvancedIconSelect::onRetroAdvance64));
	IghbyBtn = CCMenuItemSpriteExtra::create(Ighby, Ighby, this, menu_selector(AdvancedIconSelect::onIghby));
	Rob_buckBtn = CCMenuItemSpriteExtra::create(Rob_buck, Rob_buck, this, menu_selector(AdvancedIconSelect::onRob_buck));
	Chase97Btn = CCMenuItemSpriteExtra::create(Chase97, Chase97, this, menu_selector(AdvancedIconSelect::onChase97));
	AermaBtn = CCMenuItemSpriteExtra::create(Aerma, Aerma, this, menu_selector(AdvancedIconSelect::onAerma));
	Aleiz21Btn = CCMenuItemSpriteExtra::create(Aleiz21, Aleiz21, this, menu_selector(AdvancedIconSelect::onAleiz21));
	ML500Btn = CCMenuItemSpriteExtra::create(ML500, ML500, this, menu_selector(AdvancedIconSelect::onML500));
	NHJBtn = CCMenuItemSpriteExtra::create(NHJ, NHJ, this, menu_selector(AdvancedIconSelect::onNHJ));
	DahilaBtn = CCMenuItemSpriteExtra::create(Dahila, Dahila, this, menu_selector(AdvancedIconSelect::onDahila));
	PlayDashBtn = CCMenuItemSpriteExtra::create(PlayDash, PlayDash, this, menu_selector(AdvancedIconSelect::onPlayDash));
	HenyxBtn = CCMenuItemSpriteExtra::create(Henyx, Henyx, this, menu_selector(AdvancedIconSelect::onHenyx));
	FalsoBtn = CCMenuItemSpriteExtra::create(Falso, Falso, this, menu_selector(AdvancedIconSelect::onFalso));
	GhostPowerBtn = CCMenuItemSpriteExtra::create(GhostPower, GhostPower, this, menu_selector(AdvancedIconSelect::onGhostPower));
	SenpaiShikaBtn = CCMenuItemSpriteExtra::create(SenpaiShika, SenpaiShika, this, menu_selector(AdvancedIconSelect::onSenpaiShika));
	SprintBtn = CCMenuItemSpriteExtra::create(Sprint, Sprint, this, menu_selector(AdvancedIconSelect::onSprint));

	// https://www.youtube.com/channel/UCyba8QmmlDzYy2BFbKnV4QA
	// https://www.youtube.com/channel/UCienYQplPSwWfC7G4DHaZFg

	robtop->setScale(.7);
	RetroAdvance64->setScale(.6);
	Ighby->setScale(.7);
	Flushii->setScale(.7);
	Luisillo->setScale(.7);
	Edialex->setScale(.7);
	Rob_buck->setScale(.7);
	Chase97->setScale(.7);
	Aerma->setScale(.7);
	Aleiz21->setScale(.7);
	ML500->setScale(.7);
	plus->setScale(.7);
	NHJ->setScale(.7);
	Dahila->setScale(.5);
	PlayDash->setScale(.7);
	Henyx->setScale(.7);
	Falso->setScale(.7);
	GhostPower->setScale(.7);
	SenpaiShika->setScale(.5);
	Sprint->setScale(.7);

	int pos = gm->m_nPlayerSwing;
	int pos2 = gm->getIntGameVariable("7001");

	/*
								auto formato = CCString::createWithFormat("%i", gm->getIntGameVariable("6969"));
		auto gmtest = CCLabelBMFont::create(formato->getCString(), "goldFont.fnt");
		gmtest->setPosition(dir->getScreenLeft() + 250, dir->getScreenTop() - 30);
		gmtest->setScale(.7);
		addChild(gmtest); */

	auto layer = CCLayer::create();
	auto menuBack = CCMenu::create();

	auto menu = CCMenu::create();

	auto winSize = dir->getWinSize();

	auto bg = CCSprite::create("GJ_gradientBG.png");

	bg->setPosition(CCPoint(0, winSize.height / 2));
	bg->setScaleX(winSize.width);
	bg->setScaleY(winSize.height / bg->getContentSize().height);
	bg->setColor(ccc3(0, 0x66, 0xFF));

	this->addChild(bg, -2);

	auto salir = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	auto salirBtn = CCMenuItemSpriteExtra::create(salir, salir, this, menu_selector(AdvancedIconSelect::onBack));
	menuBack->addChild(salirBtn);
	menuBack->setPosition({dir->getScreenLeft() + 25, dir->getScreenTop() - 25});
	layer->addChild(menuBack, 100);

	layer->addChild(menu, 100);
	float initX = -winSize.width / 2 + 50, initY = winSize.height / 2 - 80;

	select->setPosition(ccp(winSize.width, winSize.height));
	robtopBtn->setPosition(ccp(winSize.width, winSize.height));
	RetroAdvance64Btn->setPosition(ccp(winSize.width, winSize.height));
	IghbyBtn->setPosition(ccp(winSize.width, winSize.height));
	Flushii->setPosition(ccp(winSize.width, winSize.height));
	Luisillo->setPosition(ccp(winSize.width, winSize.height));
	Edialex->setPosition(ccp(winSize.width, winSize.height));
	Rob_buckBtn->setPosition(ccp(winSize.width, winSize.height));
	Chase97Btn->setPosition(ccp(winSize.width, winSize.height));
	AermaBtn->setPosition(ccp(winSize.width, winSize.height));
	Aleiz21Btn->setPosition(ccp(winSize.width, winSize.height));
	ML500Btn->setPosition(ccp(winSize.width, winSize.height));
	plus->setPosition(ccp(winSize.width, winSize.height));
	NHJBtn->setPosition(ccp(winSize.width, winSize.height));
	DahilaBtn->setPosition(ccp(winSize.width, winSize.height));
	PlayDashBtn->setPosition(ccp(winSize.width, winSize.height));
	HenyxBtn->setPosition(ccp(winSize.width, winSize.height));
	FalsoBtn->setPosition(ccp(winSize.width, winSize.height));
	GhostPowerBtn->setPosition(ccp(winSize.width, winSize.height));
	SenpaiShikaBtn->setPosition(ccp(winSize.width, winSize.height));
	SprintBtn->setPosition(ccp(winSize.width, winSize.height));

	// swings
	if (page == 1)
	{

		auto swingText = CCLabelBMFont::create("Select Swing", "goldFont.fnt");
		swingText->setPosition(initX + 80, initY + 40);
		swingText->setScale(.7);
		menu->addChild(swingText, 101);

		auto swingCreator = CCLabelBMFont::create("Swing Creator: ", "goldFont.fnt");
		swingCreator->setPosition(initX + 300, initY + 40);
		swingCreator->setScale(.7);
		menu->addChild(swingCreator, 101);

		auto viewProfile = CCSprite::create("GJ_viewProfileTxt_002.png");
		viewProfile->setPosition(CCPoint(initX + 340, initY + 60));
		menu->addChild(viewProfile);

		if (pos == 1)
		{
			robtopBtn->setPosition(initX + 420, initY + 40);
		}

		float space = 50;
		int totalIcons = 39;

		for (int k = 0; k < totalIcons / 5 + (totalIcons % 5 != 0); k++)
		{
			for (int y = 0; y < 5; y++)
			{

				auto str = CCString::createWithFormat("swing_show_%02d.png", ((k * 5) + y) + 1);
				auto swingIcon = CCSprite::create(str->getCString());
				swingIcon->setScale(.5);
				auto swingIconBtn = CCMenuItemSpriteExtra::create(swingIcon, swingIcon, this, menu_selector(AdvancedIconSelect::onSwingClick));
				swingIconBtn->setPosition(initX + ((float)k * space), initY - (float)y * space);
				swingIconBtn->setTag(((k * 5) + y) + 1);

				menu->addChild(swingIconBtn);

				if (((k * 5) + y) + 1 == pos)
				{
					select->setPosition(ccp(initX + ((float)k * space), initY - (float)y * space));
				}

				if (((k * 5) + y) + 1 >= totalIcons)
					break;
			}
		}

		menu->addChild(select, 101);
		menu->addChild(robtopBtn, 101);
		menu->addChild(RetroAdvance64Btn, 101);
		menu->addChild(IghbyBtn, 101);
		menu->addChild(Flushii, 101);
		menu->addChild(Luisillo, 101);
		menu->addChild(Edialex, 101);
		menu->addChild(Rob_buckBtn, 101);
		menu->addChild(Chase97Btn, 101);
		menu->addChild(AermaBtn, 101);
		menu->addChild(Aleiz21Btn, 101);
		menu->addChild(ML500Btn, 101);
		menu->addChild(plus, 101);
		menu->addChild(NHJBtn, 101);
		menu->addChild(DahilaBtn, 101);
		menu->addChild(HenyxBtn, 101);
		menu->addChild(FalsoBtn, 101);
		menu->addChild(GhostPowerBtn, 101);
		menu->addChild(SenpaiShikaBtn, 101);
		menu->addChild(SprintBtn, 101);

		deleteAll();

		switch (pos)
		{
		case 1:
			robtopBtn->setPosition(initX + 420, initY + 40);
			break;
		case 2:
			IghbyBtn->setPosition(initX + 420, initY + 40);
			break;
		case 3:
		case 4:
		case 5:
			Luisillo->setPosition(initX + 420, initY + 40);
			break;
		case 6:
		case 7:
			Edialex->setPosition(initX + 420, initY + 40);
			break;
		case 8:
			Flushii->setPosition(initX + 420, initY + 40);
			break;
		case 9:
			Rob_buckBtn->setPosition(initX + 420, initY + 40);
			break;
		case 10:
			Chase97Btn->setPosition(initX + 420, initY + 40);
			break;
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 32:
		case 33:
			RetroAdvance64Btn->setPosition(initX + 440, initY + 40);
			break;
		case 26:
		case 27:
			NHJBtn->setPosition(initX + 420, initY + 40);
			break;
		case 28:
			Dahila->setScale(.5);
			SenpaiShika->setScale(.5);
			plus->setPosition(initX + 435, initY + 40);
			SenpaiShikaBtn->setPosition(initX + 480, initY + 40);
			DahilaBtn->setPosition(initX + 405, initY + 40);
			break;
		case 29:
		case 30:
			Aleiz21Btn->setPosition(initX + 420, initY + 40);
			break;
		case 31:
			AermaBtn->setPosition(initX + 405, initY + 40);
			plus->setPosition(initX + 442, initY + 40);
			ML500Btn->setPosition(initX + 480, initY + 40);
			break;
		case 34:
			AermaBtn->setPosition(initX + 405, initY + 40);
			plus->setPosition(initX + 442, initY + 40);
			HenyxBtn->setPosition(initX + 480, initY + 40);
			break;
		case 35:
			AermaBtn->setPosition(initX + 405, initY + 40);
			plus->setPosition(initX + 442, initY + 40);
			IghbyBtn->setPosition(initX + 480, initY + 40);
		case 36:
		case 37:
		case 38:
			SprintBtn->setPosition(initX + 420, initY + 40);
			break;
		case 39:
			FalsoBtn->setPosition(initX + 420, initY + 40);
			break;
		}

		/*
			auto swingCreator = CCLabelBMFont::create("Swing Creator: ", "goldFont.fnt");
		swingCreator->setPosition(initX + 250,initY+40);
		swingCreator->setScale(.7);
		menu->addChild(swingCreator, 101);

	*/
		this->addChild(layer);
	}
	else
	{
		// jetpacks

		auto jetpackText = CCLabelBMFont::create("Select Jetpack", "goldFont.fnt");
		jetpackText->setPosition(initX + 110, initY + 40);
		jetpackText->setScale(.7);
		menu->addChild(jetpackText, 101);

		auto JetpackCreator = CCLabelBMFont::create("Jetpack Creator:", "goldFont.fnt");
		JetpackCreator->setPosition(initX + 285, initY + 40);
		JetpackCreator->setScale(.7);
		menu->addChild(JetpackCreator, 101);

		auto viewProfile = CCSprite::create("GJ_viewProfileTxt_002.png");
		viewProfile->setPosition(CCPoint(initX + 340, initY + 60));
		menu->addChild(viewProfile);

		float space = 50;
		int totalIcons = 4;

		for (int k = 0; k < totalIcons / 5 + (totalIcons % 5 != 0); k++)
		{
			for (int y = 0; y < 5; y++)
			{

				auto str = CCString::createWithFormat("jetpack_show_%02d.png", ((k * 5) + y) + 1);
				auto jetpackIcon = CCSprite::create(str->getCString());
				jetpackIcon->setScale(.5);
				auto jetpackIconBtn = CCMenuItemSpriteExtra::create(jetpackIcon, jetpackIcon, this, menu_selector(AdvancedIconSelect::onJetPackClick));
				jetpackIconBtn->setPosition(initX + ((float)k * space), initY - (float)y * space);
				jetpackIconBtn->setTag(((k * 5) + y) + 1);
				menu->addChild(jetpackIconBtn);

				if (((k * 5) + y) + 1 == pos2)
				{
					select->setPosition(ccp(initX + ((float)k * space), initY - (float)y * space));
				}

				if (((k * 5) + y) + 1 >= totalIcons)
					break;
			}

			menu->addChild(select, 101);
			menu->addChild(robtopBtn, 101);
			menu->addChild(RetroAdvance64Btn, 101);
			menu->addChild(PlayDashBtn, 101);
			menu->addChild(GhostPowerBtn, 101);

			deleteAll();

			switch (pos2)
			{
			case 1:
				robtopBtn->setPosition(initX + 420, initY + 40);
				break;
			case 2:
				PlayDashBtn->setPosition(initX + 420, initY + 40);
				break;
			case 3:
				GhostPowerBtn->setPosition(initX + 440, initY + 40);
				break;
			case 4:
				RetroAdvance64Btn->setPosition(initX + 440, initY + 40);
				break;
			}

			this->addChild(layer);
		}
		menu->addChild(select, 101);
	}

	return true;
}

void AdvancedIconSelect::onSwingClick(CCObject *sender)
{
	CCMenuItemSpriteExtra *obj = (CCMenuItemSpriteExtra *)sender;
	select->setPosition(ccp(obj->getPositionX(), obj->getPositionY()));
	auto tag = obj->getTag();

	auto gm = GameManager::sharedState();
	gm->setIntGameVariable("6969", tag);
	// gm->m_nPlayerSwing = tag;
	gm->setPlayerSwing(tag);
	GDPS->setPlayerSwing(tag);

	extern bool userDataChanged;
	userDataChanged = true;

	extern bool shouldSendDefaultValue;
	shouldSendDefaultValue = false;

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	float initX = -winSize.width / 2 + 50, initY = winSize.height / 2 - 80;

	if (tag == 1)
	{

		deleteAll();
		robtopBtn->setPosition(initX + 420, initY + 40);
	}
	else
	{

		deleteAll();

		switch (tag)
		{

		case 2:
			IghbyBtn->setPosition(initX + 420, initY + 40);
			break;
		case 3:
		case 4:
		case 5:
			Luisillo->setPosition(initX + 420, initY + 40);
			break;
		case 6:
		case 7:
			Edialex->setPosition(initX + 420, initY + 40);
			break;
		case 8:
			Flushii->setPosition(initX + 420, initY + 40);
			break;
		case 9:
			Rob_buckBtn->setPosition(initX + 420, initY + 40);
			break;
		case 10:
			Chase97Btn->setPosition(initX + 420, initY + 40);
			break;
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 32:
		case 33:
			RetroAdvance64Btn->setPosition(initX + 440, initY + 40);
			break;
		case 26:
		case 27:
			NHJBtn->setPosition(initX + 420, initY + 40);
			break;
		case 28:
			plus->setPosition(initX + 435, initY + 40);
			SenpaiShikaBtn->setPosition(initX + 480, initY + 40);
			DahilaBtn->setPosition(initX + 405, initY + 40);
			break;
		case 29:
		case 30:
			Aleiz21Btn->setPosition(initX + 420, initY + 40);
			break;
		case 31:
			AermaBtn->setPosition(initX + 405, initY + 40);
			plus->setPosition(initX + 442, initY + 40);
			ML500Btn->setPosition(initX + 480, initY + 40);
			break;
		case 34:
			AermaBtn->setPosition(initX + 405, initY + 40);
			plus->setPosition(initX + 442, initY + 40);
			HenyxBtn->setPosition(initX + 480, initY + 40);
			break;
		case 35:
			AermaBtn->setPosition(initX + 405, initY + 40);
			plus->setPosition(initX + 442, initY + 40);
			IghbyBtn->setPosition(initX + 480, initY + 40);
			break;
		case 36:
		case 37:
		case 38:
			SprintBtn->setPosition(initX + 420, initY + 40);
			break;
		case 39:
			FalsoBtn->setPosition(initX + 420, initY + 40);
		}
	}
}
void AdvancedIconSelect::onJetPackClick(CCObject *sender)
{
	CCMenuItemSpriteExtra *obj = (CCMenuItemSpriteExtra *)sender;
	select->setPosition(ccp(obj->getPositionX(), obj->getPositionY()));
	auto tag = obj->getTag();

	auto gm = GameManager::sharedState();
	gm->setIntGameVariable("7001", tag);
	GDPS->setPlayerJetpack(tag);

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	float initX = -winSize.width / 2 + 50, initY = winSize.height / 2 - 80;

	if (tag == 1)
	{

		deleteAll();
		robtopBtn->setPosition(initX + 420, initY + 40);
	}
	else
	{

		deleteAll();

		switch (tag)
		{
		case 2:
			PlayDashBtn->setPosition(initX + 420, initY + 40);
			break;
		case 3:
			GhostPowerBtn->setPosition(initX + 440, initY + 40);
			break;
		case 4:
			RetroAdvance64Btn->setPosition(initX + 440, initY + 40);
			break;
		}
	}
}
