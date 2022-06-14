#include <gd.h>
#include <Logger.h>
#include <sstream>
#include "cocos2d.h"
#include "CreditsLayer.h"

#define version 0

bool CreditsLayer::init()
{

	this->setKeypadEnabled(true);

	auto dir = CCDirector::sharedDirector();
	layer = CCLayer::create();
	auto menu = CCMenu::create();
	auto winSize = dir->getWinSize();

	auto bg = CCSprite::create("GJ_gradientBG.png");

	bg->setPosition(CCPoint(0, winSize.height / 2));
	bg->setScaleX(winSize.width);
	bg->setScaleY(winSize.height / bg->getContentSize().height);
	bg->setColor(ccc3(88, 14, 138));

	this->addChild(bg, -2);

	auto Marrow = CCMenu::create();

	auto back = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	auto backBtn = CCMenuItemSpriteExtra::create(back, back, this, menu_selector(CreditsLayer::onBack));
	Marrow->addChild(backBtn);
	Marrow->setPosition({dir->getScreenLeft() + 25, dir->getScreenTop() - 25});
	layer->addChild(Marrow);
	this->addChild(Marrow);

	auto credits = CCLabelBMFont::create("Credits", "bigFont.fnt");
	credits->setPosition({CCRIGHT / 2, CCTOP - 25});
	// credits->setScale(.5);
	layer->addChild(credits);
	layer->addChild(menu, 100);
	// main layer done

	// layer 1
	layer1 = CCLayer::create();

	auto andy = CCSprite::create("andy.png");
	andy->setScale(.65);
	andy->setPosition({CCMIDX - 120, CCTOP - 125});
	layer1->addChild(andy);

	// gj_discordIcon_001

	auto Mandy = CCMenu::create();

	auto AndyYT = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
	auto AndyTW = CCSprite::createWithSpriteFrameName("gj_twIcon_001.png");

	auto AndyYTBtn = CCMenuItemSpriteExtra::create(AndyYT, AndyYT, this, menu_selector(CreditsLayer::onAndyYT));
	auto AndyTWBtn = CCMenuItemSpriteExtra::create(AndyTW, AndyTW, this, menu_selector(CreditsLayer::onAndyTW));

	Mandy->addChild(AndyYTBtn);
	Mandy->addChild(AndyTWBtn);

	Mandy->setPosition({andy->getPositionX(), andy->getPositionY()});

	AndyYTBtn->setPosition(-40, -32);
	AndyTWBtn->setPosition(-75, -32);

	layer1->addChild(Mandy, 100);

	auto blaze = CCSprite::create("blaze.png");
	blaze->setScale(.65);
	blaze->setPosition({CCMIDX + 120, CCTOP - 125});
	layer1->addChild(blaze);

	auto MBlaze = CCMenu::create();

	auto BlazeYT = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
	auto BlazeYTBtn = CCMenuItemSpriteExtra::create(BlazeYT, BlazeYT, this, menu_selector(CreditsLayer::onBlazeYT));
	MBlaze->addChild(BlazeYTBtn);
	MBlaze->setPosition({blaze->getPositionX(), blaze->getPositionY()});
	BlazeYTBtn->setPosition(-75, -32);
	layer1->addChild(MBlaze, 100);

	auto iad = CCSprite::create("iad.png");
	iad->setScale(.65);
	iad->setPosition({CCMIDX - 120, CCTOP - 250});
	layer1->addChild(iad);

	auto MIAD = CCMenu::create();

	auto IADYT = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
	auto IADDI = CCSprite::create("gj_discordIcon_001.png");

	IADYT->setScale(.7);
	IADDI->setScale(.38);

	auto IADYTBtn = CCMenuItemSpriteExtra::create(IADYT, IADYT, this, menu_selector(CreditsLayer::onIADYT));
	auto IADDIBtn = CCMenuItemSpriteExtra::create(IADDI, IADDI, this, menu_selector(CreditsLayer::onIADDI));

	MIAD->addChild(IADYTBtn);
	MIAD->addChild(IADDIBtn);

	MIAD->setPosition({iad->getPositionX(), iad->getPositionY()});

	IADYTBtn->setPosition(-55, -35);
	IADDIBtn->setPosition(-80, -35);

	layer1->addChild(MIAD, 100);

	auto collar = CCSprite::create("collar.png");
	collar->setScale(.65);
	collar->setPosition({CCMIDX + 120, CCTOP - 250});
	layer1->addChild(collar);

	auto MCOLLAR = CCMenu::create();

	auto COLLARDI = CCSprite::create("gj_discordIcon_001.png");

	COLLARDI->setScale(.38);
	auto COLLARDIBtn = CCMenuItemSpriteExtra::create(COLLARDI, COLLARDI, this, menu_selector(CreditsLayer::onCollar));
	MCOLLAR->addChild(COLLARDIBtn);
	MCOLLAR->setPosition({collar->getPositionX(), collar->getPositionY()});
	COLLARDIBtn->setPosition(-80, -38);
	layer1->addChild(MCOLLAR, 100);

	auto Mnext2 = CCMenu::create();

	auto next2 = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	next2->setFlipX(true);

	auto next2Btn = CCMenuItemSpriteExtra::create(next2, next2, this, menu_selector(CreditsLayer::on2));
	Mnext2->setPosition({CCRIGHT - 25, CCTOP / 2});
	Mnext2->addChild(next2Btn);
	layer1->addChild(Mnext2, 100);

	// layer 2

	layer2 = CCLayer::create();

	auto Mnext1 = CCMenu::create();

	auto next1 = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	auto next1Btn = CCMenuItemSpriteExtra::create(next1, next1, this, menu_selector(CreditsLayer::on1));
	Mnext1->addChild(next1Btn);
	Mnext1->setPosition({CCLEFT + 25, CCTOP / 2});
	layer2->addChild(Mnext1, 100);

	auto next3 = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	next3->setFlipX(true);

	auto Mnext3 = CCMenu::create();
	auto next3Btn = CCMenuItemSpriteExtra::create(next3, next3, this, menu_selector(CreditsLayer::on3));
	Mnext3->setPosition({CCRIGHT - 25, CCTOP / 2});
	Mnext3->addChild(next3Btn);
	layer2->addChild(Mnext3, 100);

	auto eitan = CCSprite::create("eitan.png");
	eitan->setScale(.65);
	eitan->setPosition({CCMIDX - 120, CCTOP - 125});
	layer2->addChild(eitan);

	auto MEitan = CCMenu::create();

	auto EitanYT = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
	auto EitanYTBtn = CCMenuItemSpriteExtra::create(EitanYT, EitanYT, this, menu_selector(CreditsLayer::onEitanYT));
	MEitan->addChild(EitanYTBtn);
	MEitan->setPosition({eitan->getPositionX(), eitan->getPositionY()});
	EitanYTBtn->setPosition(-75, -32);
	layer2->addChild(MEitan, 100);

	auto sai = CCSprite::create("sai.png");
	sai->setScale(.65);
	sai->setPosition({CCMIDX + 120, CCTOP - 125});
	layer2->addChild(sai);

	auto MSai = CCMenu::create();

	auto SaiYT = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
	auto SaiDI = CCSprite::create("gj_discordIcon_001.png");
	SaiDI->setScale(.5);

	auto SaiYTBtn = CCMenuItemSpriteExtra::create(SaiYT, SaiYT, this, menu_selector(CreditsLayer::onSaiYT));
	auto SaiDIBtn = CCMenuItemSpriteExtra::create(SaiDI, SaiDI, this, menu_selector(CreditsLayer::onSaiDI));

	MSai->addChild(SaiYTBtn);
	MSai->addChild(SaiDIBtn);

	MSai->setPosition({sai->getPositionX(), sai->getPositionY()});

	SaiYTBtn->setPosition(-40, -32);
	SaiDIBtn->setPosition(-75, -32);

	layer2->addChild(MSai, 100);

	auto robtop = CCSprite::create("robtop.png");
	robtop->setScale(.65);
	robtop->setPosition({CCMIDX - 120, CCTOP - 250});
	layer2->addChild(robtop);

	auto MRobtop = CCMenu::create();

	auto RobtopYT = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
	auto RobtopTW = CCSprite::createWithSpriteFrameName("gj_twIcon_001.png");

	auto RobtopYTBtn = CCMenuItemSpriteExtra::create(RobtopYT, RobtopYT, this, menu_selector(CreditsLayer::onRobtopYT));
	auto RobtopTWBtn = CCMenuItemSpriteExtra::create(RobtopTW, RobtopTW, this, menu_selector(CreditsLayer::onRobtopTW));

	MRobtop->addChild(RobtopYTBtn);
	MRobtop->addChild(RobtopTWBtn);

	MRobtop->setPosition({robtop->getPositionX(), robtop->getPositionY()});

	RobtopYTBtn->setPosition(-40, -32);
	RobtopTWBtn->setPosition(-75, -32);

	layer2->addChild(MRobtop, 100);

	auto retro = CCSprite::create("retro.png");
	retro->setScale(.65);
	retro->setPosition({CCMIDX + 120, CCTOP - 250});
	layer2->addChild(retro);

	auto MRetro = CCMenu::create();

	auto RetroYT = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");
	auto RetroTW = CCSprite::createWithSpriteFrameName("gj_twIcon_001.png");

	RetroYT->setScale(.7);
	RetroTW->setScale(.7);

	auto RetroYTBtn = CCMenuItemSpriteExtra::create(RetroYT, RetroYT, this, menu_selector(CreditsLayer::onRetroYT));
	auto RetroTWBtn = CCMenuItemSpriteExtra::create(RetroTW, RetroTW, this, menu_selector(CreditsLayer::onRetroTW));

	MRetro->addChild(RetroYTBtn);
	MRetro->addChild(RetroTWBtn);

	MRetro->setPosition({retro->getPositionX(), retro->getPositionY()});

	RetroYTBtn->setPosition(-55, -35);
	RetroTWBtn->setPosition(-80, -35);

	layer2->addChild(MRetro, 100);

	// layer 2 done

	// layer 3

	layer3 = CCLayer::create();

	auto Mnext4 = CCMenu::create();

	auto next4 = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	auto next4Btn = CCMenuItemSpriteExtra::create(next4, next4, this, menu_selector(CreditsLayer::on2));
	Mnext4->addChild(next4Btn);
	Mnext4->setPosition({CCLEFT + 25, CCTOP / 2});
	layer3->addChild(Mnext4, 100);

	auto catto = CCSprite::create("catto.png");
	catto->setScale(.65);
	catto->setPosition({CCMIDX - 120, CCTOP - 125});
	layer3->addChild(catto);

	auto MCatto = CCMenu::create();
	auto CattoYT = CCSprite::createWithSpriteFrameName("gj_ytIcon_001.png");

	auto CattoYTBtn = CCMenuItemSpriteExtra::create(CattoYT, CattoYT, this, menu_selector(CreditsLayer::onCattoYT));

	MCatto->addChild(CattoYTBtn);

	MCatto->setPosition({andy->getPositionX(), andy->getPositionY()});

	CattoYTBtn->setPosition(-75, -32);

	layer3->addChild(MCatto, 100);

	layer2->setVisible(false);
	layer3->setVisible(false);

	this->addChild(layer);
	this->addChild(layer1);
	this->addChild(layer2);
	this->addChild(layer3);

	return true;
}
