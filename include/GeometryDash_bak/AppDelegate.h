
#include "cocos2d.h"

USING_NS_CC;

class AppDelegate : public CCApplication {
public:
	AppDelegate();
	~AppDelegate();
	
	static AppDelegate *get();

	virtual void applicationDidEnterBackground();
	virtual bool applicationDidFinishLaunching();
	virtual void applicationWillEnterForeground();
	virtual void applicationWillResignActive();
	
	/* Called on loading end */
	void loadingIsFinished();
	
	void pauseGame();
	void resumeSound();
	/* Don't use this function */
	
	/* Saves the managers if loading is finished */
	void trySaveGame();
	
	virtual void willSwitchToScene(CCScene *pScene);
	
};