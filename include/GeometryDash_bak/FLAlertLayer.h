#include <string.h>
#include <gd.h>
USING_NS_CC;

#define __STR_CAT___(str1, str2) str1##str2
#define __STR_CAT__(str1, str2) __STR_CAT___(str1, str2)
#define PAD(size) char __STR_CAT__(pad, __LINE__)[size] = {};

class FLAlertLayer;

class FLAlertLayerProtocol {
public:
	virtual void FLAlert_Clicked(FLAlertLayer *layer, bool accept);
};





/* Floating alert layer. By default it's a message box that returns user's choice to the delegate, but you can extend it to create any kind of floating layers */
class FLAlertLayer : public CCLayerColor {
public:

    PAD(10);
	cocos2d::CCMenu* m_pButtonMenu;
	int m_nControlConnected; //?
	cocos2d::CCObject* m_pTarget;
	PAD(8);
	cocos2d::CCLayer* m_pLayer;
	int m_nZOrder2;
	bool m_bNoElasticity;
	cocos2d::ccColor3B m_cColor2; //?
	ButtonSprite* m_pButton1;
	ButtonSprite* m_pButton2;
	void* m_pScrollingLayer;
	int m_nJoystickConnected;
	bool m_bBorder; //?
	bool m_bNoAction; //?


public:
	FLAlertLayer();

	static FLAlertLayer *create(FLAlertLayerProtocol *delegate, const char *title, std::string text, const char *btn1Text, const char *btn2Text);
	static FLAlertLayer *create(FLAlertLayerProtocol *delegate, const char *title, std::string text, const char *btn1Text, const char *btn2Text, float layerHeight);
	static FLAlertLayer *create(FLAlertLayerProtocol *delegate, const char *title, std::string text, const char *btn1Text, const char *btn2Text, float layerHeight, bool scrollable, float );
	bool init(FLAlertLayerProtocol *delegate, const char *title, const char *text, std::string btn1Text, const char *btn2Text, float layerHeight);
	void onBtn2(cocos2d::CCObject *);
	void onBtn1(cocos2d::CCObject *);


	virtual void onEnter(void);
	virtual bool ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *){};
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *){};
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *){};
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *){};
	virtual void registerWithTouchDispatcher(void){};
	virtual void keyBackClicked(void){};
	virtual void keyDown(cocos2d::enumKeyCodes){};
	virtual void show(void);
};
