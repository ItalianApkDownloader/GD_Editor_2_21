#pragma once

#include "cocos2d.h"
#include "gd.h"
#include "obfuscate.h"
#include "MapSelectLayer.h"

class ToolsLayer : public CCLayer {
	
public:
    bool init();
	
	    virtual void keyBackClicked() { this->onBack(nullptr); }

	    void onBack(CCObject*)
    {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, MenuLayer::scene(false)));
    }
	
	    void create(CCObject*)
		{
                auto scene = CCScene::create();
        scene->addChild(ToolsLayer::create(), 100);

        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
    }
	
		void onTheMap(CCObject*) {
		auto dir = CCDirector::sharedDirector();
		auto scene = CCScene::create();
		scene->addChild(MapSelectLayer::scene(100), 100);
		dir->replaceScene(CCTransitionFade::create(0.5f, scene));
	}
	
		void onNormalMod(CCObject*) {
	FLAlertLayer::create(nullptr, "Normal mod", "Normal mods request levels to elder moderators",  "OK", nullptr, 400, false, 300)->show();
	}
	
		void onElder(CCObject*) {
	FLAlertLayer::create(nullptr, "Elder mod", "Elder moderators can rate levels",  "OK", nullptr, 400, false, 300)->show();
	}
	
		void onBooster(CCObject*) {
	FLAlertLayer::create(nullptr, "Discord booster", "Has boosted the discord server with nitro\n-Privileges: Pink account comments",  "OK", nullptr, 400, false, 300)->show();
	}
	
		void onVerified(CCObject*) {
	FLAlertLayer::create(nullptr, "Verified", "- Known people in the GD community\n-Known creators\n-Youtubers that do not upload GDPS content.\n-This role has no extra privileges",  "OK", nullptr, 400, false, 300)->show();
	}
	
		void onYoutuber(CCObject*) {
	FLAlertLayer::create(nullptr, "Youtubers", "Youtubers that have a significantly big number of subscribers and uploads gdps content from time to time.\n-This role has no extra privileges",  "OK", nullptr, 400, false, 300)->show();
	}
	
		void onDev(CCObject*) {
	FLAlertLayer::create(nullptr, "Developer", "Work and help with GDPS updates \n- This role has no extra privileges",  "OK", nullptr, 400, false, 300)->show();
	}
	
		void onAdmin(CCObject*) {
	FLAlertLayer::create(nullptr, "Discord Admin", "is admin in the GDPS Editor 2.2 Discord server\nPrivileges: can rate levels, can delete accounts, light purple comment color",  "OK", nullptr, 400, false, 300)->show();
	}
	
		void onOwner(CCObject*) {
	FLAlertLayer::create(nullptr, "GDPS Owner", "Owner of the GDPS",  "OK", nullptr, 400, false, 300)->show();
	}
	
			void onUndefined(CCObject*) {
	FLAlertLayer::create(nullptr, "Undefined", "Undefined role",  "OK", nullptr, 400, false, 300)->show();
	}
	
void onAlert(CCObject*) {
	FLAlertLayer::create(nullptr, "The map!", "This is not the real map, only the functionality of the layer was leaked so placeholder textures were added to open it.",  "OK", nullptr, 400, false, 300)->show();
}	
void onLevelRequest(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://gdpseditor.com/levelRequest");
}
void onUsername(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://gdpseditor.com/username");
}
void onPassword(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://gdpseditor.com/password");
}
void onMusic(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://gdpseditor.com/music");
}
void onGDBrowser(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://gdpseditor.com/gdbrowser");
}
void onUnlisted(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://gdpseditor.com/unlisted");
}
		

    CREATE_FUNC(ToolsLayer)
};