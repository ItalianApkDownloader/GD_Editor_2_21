#pragma once
#define GDPS GDPSManager::sharedState()

#include <iostream>
#include "json.hpp"



class GDPSManager{

	RSJresource jsonObject;

	
public:
	
	//TODO make this private and add getters and setters
	int opacityLeft;
	int opacityRight;
	int newsLevelID;
	bool showNewNewsIndicator;
	
	

	int particles;
	bool acceptedHackAlert;
	
	
	//news layer
	bool isNews;
	bool g1;
	bool g2;
	bool g3;
	std::string tempPassword;
	
	//news layer new announcements indicator
	int newsCount;
	
	bool bluestacks;
	bool ldplayer;

protected:
	void logString();
    bool init();
    void encodeDataTo();
    void dataLoaded();
    void firstLoad();
    void setup();
    void load();
	void detectEmulators();
	void writeToFile(std::string path, const char* content);
	const char* getJsonString();




public:
    static GDPSManager* sharedState();

    void save();
	
	void setNewsCount(int n);
	int getNewsCount();
	bool shouldShowNews();
	
	bool isEmulator();
	bool isBluestacks();
	bool isLDPlayer();
	
	
	static void ApplyHooks();

protected:
    std::string m_sFileName;
};