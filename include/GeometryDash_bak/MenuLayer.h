#ifndef __MENULAYER_H__
#define __MENULAYER_H__


#include "GeometryDash.h"
#include "cocos2d.h"
#include <gd.h>


class GD_DLL MenuGameLayer : public CCLayer {
public:
	~MenuGameLayer();
	
	static MenuGameLayer *create();
	bool init();
	
	void update(float dt);
	
	void tryJump();
	
};



class GD_DLL MenuLayer : public CCLayer, public FLAlertLayerProtocol {
	public:
		bool init();
		void openOptions(bool);
		static CCScene * scene(bool);
		void onGarage(CCObject * sender);
};

#endif //__MENULAYER_H__