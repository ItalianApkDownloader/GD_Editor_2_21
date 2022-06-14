#include <gd.h>
#include "cocos2d.h"
#include "onPlaytestExt.h"
#include "patch.h"
//#include "../../include/hooks/MenuLayerExt.h"

void onPlaytestExt::playtest_hk(int a2, int a3, int a4)
{

	if (GM->getGameVariable("100001"))
	{
		return playtest(this, a2, a3, a4);
	}
	else
	{

		fl = FLAlertLayer::create(nullptr, "Playtest disabled", "Playtest <cr>often crashes</c> so it's disabled by default\nYou can enable playtest (at your own risk) in GDPS Settings\n<cg>Options -> GDPS Settings -> Playtest</c>", "OK", nullptr, 470, false, 200);

		auto menu = fl->m_pButtonMenu;
		auto children = menu->getChildren();
		auto okBtn = reinterpret_cast<CCNode *>(children->objectAtIndex(0));

		auto sprite = ButtonSprite::create("Enable", 80, 30, 30, 5);
		auto btn = CCMenuItemSpriteExtra::create(sprite, sprite, this, menu_selector(onPlaytestExt::onEnablePlaytest));
		btn->setPositionX(okBtn->getPositionX() + 60);

		okBtn->setPositionX(okBtn->getPositionX() - 40);
		menu->addChild(btn);

		//  fl->m_pLayer->addChild(sprite, 50);
		fl->show();
	}
}
