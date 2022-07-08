
#include "patch.h"
#include <fstream>
#include <gd.h>
#include <CCFileUtils.h>
#include "GDPSHelper.h"
#include <sstream>
#include <typeinfo>
#include <memory>
#include <cxxabi.h>
#include "CCMenuItemToggler.h"
#include "FunctionHelper.h"


void GDPSHelper::createArrayAndAssign(LevelEditorLayer* p,int offset){
	auto array = CCArray::create();
	*((int *)p + offset) = reinterpret_cast<uintptr_t>(array);
	array->retain();
}


std::string GDPSHelper::NameForNode(cocos2d::CCObject* node)
{
	// stolen from mat (thanks mat)
	// and also stolen from https://stackoverflow.com/a/4541470

	auto name = typeid(*node).name();
	auto status = -1;

	std::unique_ptr<char, void (*)(void*)> res {
		abi::__cxa_demangle(name, nullptr, nullptr, &status),
		std::free
	};

	return (status == 0) ? res.get() : name;
}


void GDPSHelper::logNames(CCArray* objects) {
	
	for(int i = 0; i < objects->count(); i++) {
		auto node = (CCNode*)objects->objectAtIndex(i);
		auto name = GDPSHelper::NameForNode(node);
		CCLog("i: %d | %s", i, name.c_str());
		
		if(node->getChildrenCount() > 0) {
			
			auto objects2 = node->getChildren();
			
			for(int j = 0; j < node->getChildrenCount(); j++) {
				
				auto node2 = (CCNode*)objects2->objectAtIndex(i);
				auto name2 = GDPSHelper::NameForNode(node2);
				CCLog("inside of %s | j: %d | %s", name.c_str(), j, name2.c_str());
				
			}
		}
		
	}
	CCLog("Finished");

}

void GDPSHelper::createToggleButton(const char *text, CCPoint position, float boxScale, float labelScale, CCObject *baseclass, cocos2d::SEL_MenuHandler callback, CCMenu *menu, bool toggled, bool enabled)
{
	auto onSpr = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	auto offSpr = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

	onSpr->setScale(boxScale);
	offSpr->setScale(boxScale);

	auto toggle = CCMenuItemToggler::create(
	onSpr,
	offSpr,
	baseclass,
	callback);
	toggle->setSizeMult(1.5);
	toggle->toggle(!toggled);

	auto label = CCLabelBMFont::create(text, "bigFont.fnt");
	label->limitLabelWidth(80, labelScale, 0);
	label->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentLeft);

	toggle->setPosition(position);
	label->setPosition(toggle->getPositionX() + 17.5 + label->getScaledContentSize().width / 2, toggle->getPositionY());

	menu->addChild(toggle);
	menu->addChild(label);

}


void GDPSHelper::createLabels(CCNode* base, CCPoint pos, bool logNames) {
	
	CCArray* objects = base->getChildren();
	int count = objects->count();
	auto menu = CCMenu::create();
	
	menu->setPosition(pos);
	base->addChild(menu, 100);
	
	for(int i = 0 ; i < count; i++) {
		
		auto node = (CCNode*)objects->objectAtIndex(i);
		if(logNames) {
			const char* name = GDPSHelper::NameForNode(node).c_str();
			CCLog("i: %d: | name: %s", i, name);
		}
		
		auto label = CCLabelBMFont::create(FunctionHelper::itos(i).c_str(), "bigFont.fnt");
		label->setPosition(node->getPosition());
		label->setScale(.65);
		menu->addChild(label);
		
	}
	
}