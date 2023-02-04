#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"
#include "patch.h"
#include "GDPSHelper.h"
#include "GDPSManager.h"
#include "FunctionHelper.h"
#include "gd.h"
#include "../definitions.h"
#include "obfuscate.h"

//epic servers obfuscate moment

inline string replaceServers(std::string original)
{
	//const char *boomlings = AY_OBFUSCATE("http://www.boomlings.com/database");
	if(GM->ggv("1000015")) {
		std::string pcServer = "local/ip/sorry/lol";
		original.replace(0, 34, pcServer);
	}

/*
	if(original.find("gdpseditor.com") != std::string::npos)
		return original;
	
	const char *gdpseditor = AY_OBFUSCATE("http://game.gdpseditor.com/server");

	int c = strlen(gdpseditor);
	for (int i = 0; i < c; i++)
		original.at(i) = gdpseditor[i];
*/
	fmtlog("url: {}", original);
	return original;
}

void *(*LevelProcessO)(GameManager *gm, string url, string a2, string a3, int a4);
void *LevelProcessH(GameManager *gm, string url, string postData, string a3, int a4)
{
	if(postData.find("epic=") != std::string::npos) 
	{
		postData += fmt::format("&godlike={}", (int)GLM->getBoolForKey("legendary_filter_custom"));
		
		//the platformer filter is actually a length filter
		//however thats buggy if i add it like that so i will just use the old code
		//to add it in advanced filters
		if(GLM->getBoolForKey("platform_filter_custom"))
		{
			int lenpos = postData.find("len=");
			if(postData[lenpos + 4] != '-')
				postData.insert(postData.find("&page"), ",5");
			else
				postData[lenpos + 4] = '5';
		}
	}
	return LevelProcessO(gm, replaceServers(url), postData, a3, a4);
}

#include "AccountLoginLayer.h"
FUNCTIONHOOK(void, AccountLoginLayer_onSubmit, AccountLoginLayer* self, void* sender) {
	GDPS->tempPassword = self->_inputPassword()->getString();
	AccountLoginLayer_onSubmitO(self, sender);
	GDPS->tempPassword = "";
}

FUNCTIONHOOK(void*, GJAccountManager_loginAccount, void* self, std::string username, std::string pass) {
	return GJAccountManager_loginAccountO(self, username, GDPS->tempPassword);
}


void *(*MusicProcessO)(void *idk, string url, string a2, string a3, int a4);
void *MusicProcessH(void *idk, string url, string postData, string a3, int a4)
{
	return MusicProcessO(idk, replaceServers(url), postData, a3, a4);
}

void *(*AccountProcessO)(void *idk, string url, string a2, string a3, int a4);
void *AccountProcessH(void *idk, string url, string postData, string a3, int a4)
{
	return AccountProcessO(idk, replaceServers(url), postData, a3, a4);
}

FUNCTIONHOOK(void, send, void* self, cocos2d::extension::CCHttpRequest* req) {
	fmtlog("HTTP: {}, DATA: {}\n-----------------\n", req->getUrl(), std::string(req->_requestData.begin(), req->_requestData.end()));
	sendO(self, req);
}

FUNCTIONHOOK(void*, getResponse, void* self, cocos2d::extension::CCHttpResponse* resp) {
	std::vector<char> *buffer = resp->getResponseData();
	fmtlog("RESP: {}\n-----------------\n", (std::string(buffer->begin(), buffer->end())));
	return getResponseO(self, resp);
}


void Servers::ApplyHooks() 
{
	
	HOOK2("_ZN11GameToolbox11getResponseEPN7cocos2d9extension14CCHttpResponseE", getResponse);
	HOOK2("_ZN7cocos2d9extension12CCHttpClient4sendEPNS0_13CCHttpRequestE", send);
	
	HOOK2("_ZN16GJAccountManager12loginAccountESsSs", GJAccountManager_loginAccount);
	HOOK2("_ZN17AccountLoginLayer8onSubmitEPN7cocos2d8CCObjectE", AccountLoginLayer_onSubmit);
	
	HOOK("_ZN16GameLevelManager18ProcessHttpRequestESsSsSs10GJHttpType", LevelProcessH, LevelProcessO);
	HOOK("_ZN20MusicDownloadManager18ProcessHttpRequestESsSsSs10GJHttpType", MusicProcessH, MusicProcessO);
	HOOK("_ZN16GJAccountManager18ProcessHttpRequestESsSsSs10GJHttpType", AccountProcessH, AccountProcessO);
}