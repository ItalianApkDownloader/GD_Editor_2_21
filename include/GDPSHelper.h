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



//making everything static so i dont need an instance of the class lol

class GDPSHelper {
	
	
public:
	
	
	
public:

	
	static void createArrayAndAssign(LevelEditorLayer* p,int offset);
	static std::string NameForNode(cocos2d::CCObject* node);
	static void logNames(CCArray* objects);
	
	static void createToggleButton(
	const char *text, 
	CCPoint position, 
	float boxScale, 
	float labelScale, 
	CCObject *baseclass, 
	cocos2d::SEL_MenuHandler callback, 
	CCMenu *menu, 
	bool toggled, 
	bool enabled
	);
	
	static void createLabels(CCNode*, CCArray*, CCPoint pos, bool logNames);


};