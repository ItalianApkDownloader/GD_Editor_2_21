#include <fstream>
#include <string>
#include <sstream>

//#include <gd.h>
//#include <CCFileUtils.h>
#include "GDPSManager.h"

#include "json.hpp"
#include <cstdio>

using namespace std;



GDPSManager *s_sharedGDPSManager = nullptr;

GDPSManager *GDPSManager::sharedState()
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
	jsonData["opacity"] = this->opacity;
	jsonData["password"] = this->password;
	this->jsonObject = jsonData;
}



void GDPSManager::dataLoaded()
{
	this->opacity = this->jsonObject["opacity"].as<int>(255);
	this->password = this->jsonObject["password"].as<std::string>("0");
	
}

void GDPSManager::firstLoad()
{
	this->opacity = 255;
	this->password = "0";
}

bool GDPSManager::init()
{
     this->m_sFileName = "GDPSManager.json";
     this->setup();
     return true;
}

void GDPSManager::writeToFile(string path, const char* content) 
{
	 ofstream outputFile;
     outputFile.open(path.c_str());
	 outputFile << content;
	 outputFile.close();
}


void GDPSManager::save()
{
	 this->encodeDataTo();
	 this->writeToFile(this->m_sFileName, jsonObject.as_str().c_str());
}

void GDPSManager::setup()
{
    auto path = this->m_sFileName;
    std::ifstream infile(path.c_str());
    if (infile.good())
    {
        this->load();
    }
    else
    {
		this->jsonObject = RSJresource("{}");
        this->firstLoad();
    }
}

void GDPSManager::load()
{
	 auto path = m_sFileName;
	 std::ifstream ifs(path);
	 
	 this->jsonObject = RSJresource(ifs);
	 this->dataLoaded();
}
