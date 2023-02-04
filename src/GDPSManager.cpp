#include <fstream>
#include <string>
#include <sstream>
#include "hooking.h"
#include "cocos2d.h"
#include "obfuscate.h"



//#include <gd.h>
#include <CCFileUtils.h>
#include "GDPSManager.h"
#include "json.hpp"

using namespace std;
using namespace cocos2d;


GDPSManager* s_sharedGDPSManager = nullptr;

GDPSManager* GDPSManager::sharedState()
{
    if (!s_sharedGDPSManager)
    {
        s_sharedGDPSManager = new GDPSManager();
        s_sharedGDPSManager->init();
    }
    return s_sharedGDPSManager;
}



void GDPSManager::encodeDataTo()
{
	RSJresource jsonData("{}");
	jsonData["opacityLeft"] = this->opacityLeft;
	jsonData["opacityRight"] = this->opacityRight;
	jsonData["newsCount"] = this->newsCount;
	jsonData["newsLevelID"] = this->newsLevelID;
	jsonData["showNewNewsIndicator"] = this->showNewNewsIndicator;
	jsonObject = jsonData;
}



void GDPSManager::dataLoaded()
{
	opacityRight = this->jsonObject["opacityRight"].as<int>(255);
	opacityLeft = this->jsonObject["opacityLeft"].as<int>(255);
	newsCount = this->jsonObject["newsCount"].as<int>(0);
	newsLevelID = this->jsonObject["newsLevelID"].as<int>(0);
	showNewNewsIndicator = this->jsonObject["showNewNewsIndicator"].as<bool>(false);
}

void GDPSManager::firstLoad()
{
	opacityLeft = 255;
	opacityRight = 255;
	newsCount = 0;
	showNewNewsIndicator = false;
	newsLevelID = 0;
}

bool GDPSManager::init()
{
     m_sFileName = AY_OBFUSCATE("GDPSManager.json");
     setup();
     return true;
}

void GDPSManager::writeToFile(const string path, const char* content) 
{
	 ofstream outputFile;
     outputFile.open(path.c_str());
	 outputFile << content;
	 outputFile.close();
}


void GDPSManager::save()
{
	encodeDataTo();
	writeToFile(CCFileUtils::sharedFileUtils()->getWritablePath() + this->m_sFileName, jsonObject.as_str().c_str());
	// CCLog(jsonObject.as_str().c_str());
}

void GDPSManager::setup()
{
    auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + this->m_sFileName;
    std::ifstream infile(path.c_str());
    if (infile.good())
    {
        load();
	//	CCLog("file good");
    }
    else
    {
		jsonObject = RSJresource("{}");
        firstLoad();
	//	CCLog("file doesnt exist");
    }
}

void GDPSManager::load()
{
	 auto path = CCFileUtils::sharedFileUtils()->getWritablePath() + m_sFileName;
	 std::ifstream ifs(path);
	 
	 jsonObject = RSJresource(ifs);
	 //CCLog(jsonObject.as_str().c_str());
	 dataLoaded();
}


FUNCTIONHOOK(void*, trySaveGame, void* self, bool something) {
	GDPS->save();
	return trySaveGameO(self, something);
}

void GDPSManager::setNewsCount(int n) { newsCount = n; }
int GDPSManager::getNewsCount() { return newsCount; }
bool GDPSManager::shouldShowNews() { return showNewNewsIndicator; }

void GDPSManager::ApplyHooks() {
	HOOK("_ZN11AppDelegate11trySaveGameEb", trySaveGameH, trySaveGameO);
}