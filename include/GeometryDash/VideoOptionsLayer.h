#ifndef VIDEOOPTIONSLAYER_H
#define VIDEOOPTIONSLAYER_H
#include <cocos2d.h>

class VideoOptionsLayer : public cocos2d::CCLayerColor {
public:
	static cocos2d::CCScene* scene();
	
	static cocos2d::CCLayer* create();
	void onAdvanced(cocos2d::CCObject* pSender);
	bool init();
};
#endif