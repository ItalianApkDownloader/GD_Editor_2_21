#ifndef __CREATORLAYER_H__
#define __CREATORLAYER_H__

#include "GeometryDash.h"
#include "cocos2d.h"

USING_NS_CC;

/* Main menu for user-created levels */
class GD_DLL CreatorLayer : public CCLayer {
public:
	~CreatorLayer();
	
	static CCScene *scene();
	static CreatorLayer *create();
	bool init();
	
	virtual void keyBackClicked();
	
	void onBack();
	
	void onMyLevels();
	void onSavedLevels();
	void onFeaturedLevels();
	void onOnlineLevels();
};

#endif //__CREATORLAYER_H__
