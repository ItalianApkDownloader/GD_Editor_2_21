#pragma once

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

namespace GDPSHelper {

	void createArrayAndAssign(LevelEditorLayer* p,int offset);
	std::string NameForNode(cocos2d::CCObject* node);
	void logNames(CCArray* objects);
	vector<string> getDebugLogVector();
	std::string format(std::string, ...);
	

	void createToggleButton(
		const char *text, CCPoint position, float boxScale, float labelScale, CCObject *baseclass, 
		cocos2d::SEL_MenuHandler callback, CCMenu *menu, bool toggled, bool enabled
	);
	
	void createLabels(CCNode*, CCArray*, CCPoint pos, bool logNames);
	void createLabels(CCNode*);
	std::string getRawResponse(extension::CCHttpResponse* response);
	void setHttpResponse(extension::CCHttpResponse* response, std::string strRes);
	

};