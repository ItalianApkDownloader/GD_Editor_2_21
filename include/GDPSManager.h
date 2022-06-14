#pragma once
#define GDPS GDPSManager::sharedState()


class GDPSManager{

public:
    int opacity;
    int opacityTimer;
    bool isGDPSSettings;
    bool playTest;
    bool pauseBtn;
    bool oldTextures;
    bool cleanMenu;
	
	public:
    void createToggleButton(const char*, CCPoint, CCObject*, cocos2d::SEL_MenuHandler, CCMenu*, bool, bool);


protected:
    bool init();
    void encodeDataTo(CCDictionary * data);
    void dataLoaded(CCDictionary* data);
    void firstLoad();
    void setup();
    void load();





public:
    static GDPSManager* sharedState();

    void save();
	void changeServers(const char* server, const char* server_b64); //server needs to be 33 characters, server base 64 needs to be 44 characters
	void setPlayerSwing(int id);
	void setPlayerJetpack(int id);
	void setWorldIsland(int id);
	string itos(int Number);

protected:
    CCDictionary * dictionary = nullptr;
    std::string m_sFileName;
};
