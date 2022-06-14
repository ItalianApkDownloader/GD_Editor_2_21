#include <cocos2d.h>
#include <gd.h>
#include "DialogObject.h"
#include "DialogLayer.h"

class CreatorLayerExt : public CCLayer
{
public:

	virtual void keyBackClicked() { this->onBack(nullptr); }
		
	void onBack(cocos2d::CCObject*) {
        auto dir = CCDirector::sharedDirector();
        dir->pushScene(cocos2d::CCTransitionFade::create(0.5, CreatorLayer::scene()));
    }


    void onWorld(cocos2d::CCObject*) {
        auto dir = CCDirector::sharedDirector();
        dir->pushScene(cocos2d::CCTransitionFade::create(0.5, WorldSelectLayer::scene(0)));
    }
	
	void onEvent(cocos2d::CCObject*) {
    }
	
	void onLists(cocos2d::CCObject*) {
		

		
		//create(std::string title, std::string text, int type, float text_scale, bool is_unskippable, _ccColor3B color);
		
    }
	
	 void onSafe(CCObject*)
    {

      //  FLAlertLayer::create(nullptr, "New update!", "New update!\n<cg>Download</c> the new update in the discord server\n Join using the <co>join discord!</c> button",  "OK", nullptr, 400, false, 300)->show();



		
    }
};