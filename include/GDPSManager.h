#pragma once
#define GDPS GDPSManager::sharedState()

#include <iostream>
#include "json.hpp"



class GDPSManager{
	
	RSJresource jsonObject;
	
	
public:
	
	int opacityLeft;
	int opacityRight;
	std::string password;
	
	//news layer
	bool isNews = false;
	bool g1;
	bool g2;
	bool g3;
	
	//news layer new announcements indicator
	int newsCount = 0;
	bool showNewNewsIndicator = false;

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
	static void ApplyHooks();

protected:
    std::string m_sFileName;
};