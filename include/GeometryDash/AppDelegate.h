#pragma once

#include "CCApplication.h"
#include "CCScene.h"

class __attribute__(( aligned( 4 ) )) AppDelegate : public cocos2d::CCApplication, cocos2d::CCSceneDelegate
{
public:
	bool setupGLView_;
	bool paused_;
	cocos2d::CCScene* scenePointer_; 
	bool loadingFinished_;
	bool managersLoaded_;
	bool isIOS_;
	bool isLowMemoryDevice_;
	bool enableFPSTricks_;

public:
	AppDelegate( );
	virtual ~AppDelegate( void );

	static AppDelegate* get();

	void platformShutdown( void );
	void setupGLView( void );

	float bgScale( void );
	void pauseGame( void );

	 bool applicationDidFinishLaunching( void );
	 void applicationDidEnterBackground( void );
	 void applicationWillEnterForeground( void );

	 void applicationWillBecomeActive( void );
	 void applicationWillResignActive( void );

	void trySaveGame( bool );
	virtual void willSwitchToScene( cocos2d::CCScene* p_scene );
};