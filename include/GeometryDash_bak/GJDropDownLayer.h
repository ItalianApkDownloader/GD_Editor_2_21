#ifndef __GJDROPDOWNLAYER_H__
#define __GJDROPDOWNLAYER_H__

#include "GeometryDash.h"
#include "cocos2d.h"
#include "GJListLayer.h"


using namespace cocos2d;



class GD_DLL GJDropDownLayer : public CCLayerColor {
public:

	cocos2d::CCPoint m_obEndPosition;
	cocos2d::CCPoint m_obStartPosition;
	cocos2d::CCMenu* m_pButtonMenu;
	GJListLayer* m_pListLayer;
	bool m_bControllerEnabled; //?
	cocos2d::CCLayer* m_pLayer;
	bool m_bHidden; //?


	~GJDropDownLayer();
	
	static GJDropDownLayer *create(const char *title);
	static GJDropDownLayer *create(const char *title, float height);

	bool init(const char *title);
	bool init(const char *title, float height);




	virtual void draw(void);
	virtual bool ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void registerWithTouchDispatcher(void);
	virtual void keyBackClicked(void);
	virtual void customSetup(void);
	virtual int enterLayer(void);
	virtual int exitLayer(cocos2d::CCObject *);
	virtual cocos2d::CCAction * showLayer(bool);
	virtual cocos2d::CCAction * hideLayer(bool);
	virtual int layerVisible(void);
	virtual void layerHidden(void);
	virtual void enterAnimFinished(void);
	virtual int disableUI(void);
	virtual int enableUI(void);


	//bool init(const char *title);
	//bool init(const char *title, float height);
	
	//void draw();
	//void registerWithTouchDispatcher();
	
	//bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
	//void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
	//void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
	//void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent);





	/*
	virtual void customSetup();
	
	virtual void hideLayer(bool dontAnimate);
	virtual void layerVisible();
	virtual void layerHidden();
	virtual void enterAnimFinished();
	virtual void disableUI();
	
	virtual void enterLayer();
	virtual void exitLayer();
	virtual void showLayer(bool dontAnimate);
	virtual void enableUI();
	virtual void keyBackClicked();
	
	CCPoint endPosition_;
	CCPoint startPosition_;
	
	CCMenu *menu_;
	GJListLayer *listLayer_;
	
	CC_SYNTHESIZE_READONLY(CCLayer*, internalLayer_, InternalLayer);
	CC_SYNTHESIZE(bool, removeOnExit_, RemoveOnExit);
	 */
};

#endif //__GJDROPDOWNLAYER_H__
