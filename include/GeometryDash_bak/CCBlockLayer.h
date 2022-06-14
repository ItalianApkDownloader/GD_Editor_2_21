#ifndef __CCBLOCKLAYER_H__
#define __CCBLOCKLAYER_H__

#include "GeometryDash.h"
#include "cocos2d.h"

USING_NS_CC;

/* A layer that blocks everything else when shown */
class CCBlockLayer : public CCLayerColor {
public:
	~CCBlockLayer();
	
	static CCBlockLayer  *create();
	
	bool init();
	
	void draw();
	void registerWithTouchDispatcher();
	
	bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);
	
	virtual void customSetup();
	virtual void enterLayer();
	virtual void exitLayer();
	virtual void showLayer(bool dontAnimate);
	virtual void hideLayer(bool dontAnimate);
	virtual void layerVisible();
	virtual void layerHidden();
	virtual void enterAnimFinished();
	virtual void disableUI();
	virtual void enableUI();
	virtual void keyBackClicked();
	
	CC_SYNTHESIZE(bool, removeOnExit_, RemoveOnExit);
};

#endif //__CCBLOCKLAYER_H__
