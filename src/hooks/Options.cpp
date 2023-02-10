#include "Options.h"
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"
#include "patch.h"
#include "GDPSHelper.h"
#include "GDPSManager.h"
#include "FunctionHelper.h"
#include "gd.h"
#include "../../include/cocos2dx/extensions/CCScale9Sprite.h"

constexpr std::uint8_t GDPSSettings{ 0b0000'0001 };
constexpr std::uint8_t HacksSettings{ 0b0000'0010 };
constexpr std::uint8_t normalSettings{ 0b0000'0100 };
constexpr std::uint8_t insideSafe{ 0b0000'1000 };

static std::uint8_t flags {0b0000'0000};
static inline bool isGDPSSettings() { return flags & GDPSSettings; }
static inline bool isHacksSettings() { return flags & HacksSettings; }
static inline bool isnormalSettings() { return flags & normalSettings; }
static inline bool isinsideSafe() { return flags & insideSafe; }

void Options::onGDPSSettings(CCObject*) {
//	normalSettings = false;
//	GDPSSettings = true;
//	HacksSettings = false;
	flags |= GDPSSettings;
	flags &= ~(normalSettings | HacksSettings);
	MoreOptionsLayer::create()->show();
}
void Options::onHacks(CCObject*) {
	//normalSettings = false;
	//GDPSSettings = false;
	//HacksSettings = true;
	flags |= HacksSettings;
	flags &= ~(normalSettings | GDPSSettings);
	MoreOptionsLayer::create()->show();
}
void Options::onNormalSettings(CCObject*) {
	//normalSettings = true;
	//GDPSSettings = false;
	//HacksSettings = false;
	flags |= normalSettings;
	flags &= ~(GDPSSettings | HacksSettings);
	MoreOptionsLayer::create()->show();
}

class EditorOptionsLayer : public FLAlertLayer {
	public:
	static EditorOptionsLayer* create();
};
	
void Options::onEditorSettings(CCObject*) {
//	normalSettings = false;
//	GDPSSettings = false;
//	HacksSettings = false;
	flags &= ~(normalSettings | GDPSSettings | HacksSettings);
	EditorOptionsLayer::create()->show();
}
	
	
FUNCTIONHOOK(void, addToggle, MoreOptionsLayer *self, const char *title, const char *code, const char *desc)
{
	//if is not custom settings return like normal
	if(isnormalSettings() || !isGDPSSettings() && !isHacksSettings()) return addToggleO(self, title, code, desc);

	//if call is ours
	if (isinsideSafe()) return addToggleO(self, title, code, desc);
	
	//dont add our own settings until its not last call
	if(strcmp(title, "Show Orb Guide") != 0) return;
	
	if(isGDPSSettings()) 
	{
		flags |= insideSafe;
		addToggleO(self, "Open editor at startup", "100003", 0);
		//addToggleO(self, "Pixel blocks in editor", "100005", 0);
		addToggleO(self, "Sneak Peek Death Effect", "100014", 0);
		addToggleO(self, "Swap Platf. jump sides", "100011", 0);
		addToggleO(self, "Practice Music", "0125", 0);
		addToggleO(self, "Disable arrow trigger fix", "1000010", 0);
		addToggleO(self, "Speedrun Timer", "1000011", "<cr>Red</c> means that the time is <cr>invalid</c>.\n Before starting a speedrun <cr>make sure to die atleast 1 time</c> because the first attempt time takes the enter transition into account and that will always be slower");
		addToggleO(self, "Show FPS", "0115", 0);
		addToggleO(self, "Hide pause button", "1000012", 0);
		addToggleO(self, "Disable Abbreviated Labels", "1000013", "Download and like labels in level search results");
		addToggleO(self, "No Transition", "200004", 0);
		flags &= ~insideSafe;
	}
	else if(isHacksSettings())
	{
		flags |= insideSafe;
		addToggleO(self, "Safe Noclip", "200001", "Make the player invincible\nbut it doesnt save level progress.");
		addToggleO(self, "Safe Mode", "200002", "Does not save level progress.");
		addToggleO(self, "Disable Shaders", "200003", "Does not save level progress.");
		addToggleO(self, "Disable Hack Alert", "200005", 0);
		flags &= ~insideSafe;
	}

}


	

void Options::platformOpacityLeft(CCNode* sender) {
	auto value = reinterpret_cast<SliderThumb*>(sender)->getValue();
	float newscale = 255 * value;
	GDPSManager::sharedState()->opacityLeft = newscale;
	
	auto spr = (CCSprite*)this->getChildByTag(1938);
	if(spr)
	spr->CCSprite::setOpacity(newscale);
}
void Options::platformOpacityRight(CCObject* sender) {
	auto value = reinterpret_cast<SliderThumb*>(sender)->getValue();
	float newscale = 255 * value;
	GDPSManager::sharedState()->opacityRight = newscale;
	
	auto spr = (CCSprite*)this->getChildByTag(1945);
	if(spr)
	spr->CCSprite::setOpacity(newscale);
}

FUNCTIONHOOK(bool, MoreOptionsLayer_init, MoreOptionsLayer* self) {
	flags &= ~insideSafe;
	if(!MoreOptionsLayer_initO(self)) return false;
	if(isnormalSettings() || isGDPSSettings() || isHacksSettings())
		return true;
	
	self->setTag(3);
	self->removeAllChildren();
	MBO(CCMenu*, self, 0x1B4) = CCMenu::create();
	auto menu = MBO(CCMenu*, self, 0x1B4);
	
	auto closebtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("OK"), nullptr, self, menu_selector(MoreOptionsLayer::onClose));
	closebtn->setPositionY(-100);
	menu->addChild(closebtn);

	auto square = extension::CCScale9Sprite::create("GJ_square01.png", {0.0f, 0.0f, 80.0f, 80.0f});
	square->setPosition(CCMIDX, CCMIDY);
	square->setContentSize({400, 250});
	self->addChild(square);
	
	auto gdpsbtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("GDPS", 100, 100, 120, false), nullptr, self, menu_selector(Options::onGDPSSettings));
	auto hackbtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("Hacks", 100, 100, 120, false), nullptr, self, menu_selector(Options::onHacks));
	auto optionsbtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("Options", 100, 100, 120, false), nullptr, self, menu_selector(Options::onNormalSettings));
	auto editorbtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("Editor", 100, 100, 120, false), nullptr, self, menu_selector(Options::onEditorSettings));
	
	constexpr float sepY = 25;
	constexpr float sepX = 65;
	
	constexpr float y = 70;
	
	gdpsbtn->setPosition(-sepX, +sepY + y);
	hackbtn->setPosition(+sepX, +sepY + y);
	
	optionsbtn->setPosition(-sepX, -sepY + y);
	editorbtn->setPosition(+sepX, -sepY + y);
	
	menu->addChild(gdpsbtn);
	if(!GM->_playLayer())
		menu->addChild(hackbtn);
	menu->addChild(optionsbtn);
	menu->addChild(editorbtn);

	

	
	
	constexpr float yy = 75;
	
	auto sl = Slider::create(menu,menu_selector(Options::platformOpacityLeft),1);
	sl->setPositionY(sl->getPositionY() + 60 - yy);
	
	float value = (float)GDPS->opacityLeft / (float)255;
	sl->setValue(value);
	menu->addChild(sl);
	
	auto sl2 = Slider::create(menu,menu_selector(Options::platformOpacityRight),1);
	sl2->setPositionY(sl2->getPositionY() + 5 - yy);

	float value2 = (float)GDPS->opacityRight / (float)255;
	sl2->setValue(value2);
	menu->addChild(sl2);
	
	auto txt = CCLabelBMFont::create("Platformer Opacity Left", "bigFont.fnt");
	txt->setScale(0.5);
	txt->setPositionY(txt->getPositionY() + 85 - yy);
	menu->addChild(txt);
	
	auto txt2 = CCLabelBMFont::create("Platformer Opacity Right", "bigFont.fnt");
	txt2->setScale(0.5);
	txt2->setPositionY(txt2->getPositionY() + 30 - yy);
	menu->addChild(txt2);
	
	
	auto dpadleft = CCSprite::createWithSpriteFrameName("Dpad_Btn.png");
	dpadleft->CCSprite::setOpacity(GDPS->opacityLeft);
	dpadleft->setTag(1938);
	dpadleft->setScale(0.5);
	dpadleft->setOpacity(GDPS->opacityLeft);
	dpadleft->setPosition({sl->getPositionX() - 150, sl->getPositionY()});
	menu->addChild(dpadleft);
	
	auto dpadright = CCSprite::createWithSpriteFrameName("Dpad_Btn.png");
	dpadright->CCSprite::setOpacity(GDPS->opacityRight);
	dpadright->setTag(1945);
	dpadright->setScale(0.5);
	dpadright->setOpacity(GDPS->opacityLeft);
	dpadright->setPosition({sl2->getPositionX() - 150, sl2->getPositionY()});
	menu->addChild(dpadright);
	
	
	self->addChild(menu);
	
	return true;
}

FUNCTIONHOOK(void, OptionsLayerInit_onOptions, void* self, void* sender) {
	MoreOptionsLayer::create()->show();
}

FUNCTIONHOOK(void, MoreOptionsLayer_onClose, void* self, void* sender) {
	flags &= ~(normalSettings | GDPSSettings | HacksSettings | insideSafe);
	MoreOptionsLayer_onCloseO(self, sender);
}
void Options::ApplyHooks() 
{
	HOOK("_ZN16MoreOptionsLayer4initEv", MoreOptionsLayer_initH, MoreOptionsLayer_initO);
	HOOK("_ZN16MoreOptionsLayer9addToggleEPKcS1_S1_", addToggleH, addToggleO);
	HOOK("_ZN16MoreOptionsLayer7onCloseEPN7cocos2d8CCObjectE", MoreOptionsLayer_onCloseH, MoreOptionsLayer_onCloseO);
}