#pragma once

#include "cocos2d.h"
#include "gd.h"


class CreditsLayer : public CCLayer {

		CCLayer* layer;
		CCLayer* layer1;
		CCLayer* layer2;
		CCLayer* layer3;
	
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
        scene->addChild(CreditsLayer::create(), 100);

        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
    }
		void on1(CCObject*){
	
	layer1->setVisible(true);
	layer2->setVisible(false);
	layer3->setVisible(false);
}

void on2(CCObject*){
	
	layer1->setVisible(false);
	layer2->setVisible(true);
	layer3->setVisible(false);
}

void on3(CCObject*){
	
	layer1->setVisible(false);
	layer2->setVisible(false);
	layer3->setVisible(true);
}

void onAndyTW(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://twitter.com/iAndy_HD3");
}
void onAndyYT(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://www.youtube.com/channel/UCleHhypAuPn2o09N4ISyBxw");
}
void onBlazeYT(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://www.youtube.com/channel/UCcfPtuop90e_JzxPkiZ6Q5Q");
}
void onIADYT(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://www.youtube.com/channel/UCj0BuCNiqG1wJrnSFEvtmTw");
}
void onIADDI(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://discord.gg/JGfq2Ga");
}
void onCollar(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://discord.gg/9GX4Dkb");
}
void onEitanYT(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://www.youtube.com/channel/UCT8BBgjzwNy76UOHPUUt6GA");
}
void onSaiYT(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://www.youtube.com/channel/UCvYEPVUVVLDhwfHCzgVdIIg");
}
void onSaiDI(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://discord.com/invite/qpUJgBC");
}
void onRobtopTW(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://twitter.com/RobTopGames");
}
void onRobtopYT(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://www.youtube.com/channel/UCz_yk8mDSAnxJq0ar66L4sw");
}
void onRetroYT(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://www.youtube.com/channel/UCAc5KwdpIg9m2mfO6TPQRbQ");
}
void onRetroTW(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://twitter.com/RetroAdvance64");
}
void onCattoYT(CCObject*){
cocos2d::CCApplication::sharedApplication()->openURL("https://www.youtube.com/channel/UCukOZuEy9mZZPq_hnFgAUjw");
}
		
		

    CREATE_FUNC(CreditsLayer)
};