
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
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr


void GDPSHelper::createArrayAndAssign(LevelEditorLayer* p,int offset){
	auto array = CCArray::create();
	*((int *)p + offset) = reinterpret_cast<uintptr_t>(array);
	array->retain();
}


std::string GDPSHelper::format(const std::string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::string(formatted.get());
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
	
	if(!objects) {
		CCLog("nullptr");
		return;
	}
	
	for(int i = 0; i < objects->count(); i++) {
		auto node = (CCNode*)objects->objectAtIndex(i);
		auto name = GDPSHelper::NameForNode(node);
		CCLog("i: %d | %s | count: %d", i, name.c_str(), node->getChildrenCount());

		
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

void GDPSHelper::createLabels(CCNode* layer) {
	GDPSHelper::createLabels(layer, layer->getChildren(), {0, 0}, true);
}
void GDPSHelper::createLabels(CCNode* layer, CCArray* objects, CCPoint pos, bool logNames) {
	int count = objects->count();
	auto menu = CCMenu::create();
	
	menu->setPosition(pos);
	layer->addChild(menu, 100);
	
	for(int i = 0 ; i < count; i++) {
		
		auto node = (CCNode*)objects->objectAtIndex(i);
		
		auto label = CCLabelBMFont::create(FunctionHelper::itos(i).c_str(), "bigFont.fnt");
		label->setPosition(node->getPosition());
		label->setScale(.6);
		menu->addChild(label);
		
		if(logNames) {
			const char* name = GDPSHelper::NameForNode(node).c_str();
			CCLog("i: %d: | name: %s", i, name);
		}
	}
}


std::string GDPSHelper::getRawResponse(extension::CCHttpResponse* response) {
	std::vector<char>* buffer = response->getResponseData();
	return std::string(buffer->begin(), buffer->end());	
}

void GDPSHelper::setHttpResponse(extension::CCHttpResponse* response, std::string str) {
	std::copy(str.begin(), str.end(), std::back_inserter(response->_responseData));
}