#pragma once
#define GDPS GDPSManager::sharedState()

#include <iostream>
#include "json.hpp"



class GDPSManager{
	
	RSJresource jsonObject;
	
	
public:
	
	int opacity;
	std::string password;

protected:
	void logString();
    bool init();
    void encodeDataTo();
    void dataLoaded();
    void firstLoad();
    void setup();
    void load();
	void writeToFile(std::string path, const char* content);
	const char* getJsonString();





public:
    static GDPSManager* sharedState();

    void save();

protected:
    std::string m_sFileName;
};