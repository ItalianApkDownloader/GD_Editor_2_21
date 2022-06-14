#pragma once

#include "GDAPI_Macros.h"

#include "CCObject.h"
#include "CCProtocols.h"
#include "CCAccelerometer.h"
#include "CCActionManager.h"
#include "CCKeyboardDispatcher.h"
#include "CCKeypadDispatcher.h"
#include "CCMouseDispatcher.h"
#include "CCScene.h"
#include "CCEGLView.h"

NS_CC_BEGIN

typedef enum
{
	kCCDirectorProjection2D,
	kCCDirectorProjection3D,
	kCCDirectorProjectionCustom,

	kCCDirectorProjectionDefault = kCCDirectorProjection3D,
} ccDirectorProjection;

enum PopTransition
{
	PopTransition_Fade, PopTransition_MoveInT
};

enum TextureQuality
{

};

#pragma pack(push, 1)
//TODO: Add class members to CCDirector for Windows
class CC_DLL CCDirector : public CCObject, public TypeInfo
{
public:
#ifdef GDAPI_WINDOWS
	unsigned char _unknown[41 * sizeof(int)];
	struct cc_timeval *_pLastUpdate;
	bool _bNextDeltaTimeZero;
	char _unk2[3];
	ccDirectorProjection _eProjection;
	CCSize _obWinSizeInPoints;
	float _fContentScaleFactor;
	char *_pszFPS;
	CCNode *_pNotificationNode;
	CCDirectorDelegate *_delegate;
	CCSceneDelegate *_sceneDelegate;
	CCSize _unk0, _unk1;
	int _contentScaleType;
	bool _dontCallWillSwitch;
#else
	// 
#endif

public:
	CCDirector();
	virtual ~CCDirector();

	static CCDirector *sharedDirector();

	void checkSceneReference();
	CCPoint convertToGL(const CCPoint &);
	CCPoint convertToUI(const CCPoint &);
	void drawScene();
	void end();
	float getActualDeltaTime() const;
	double getAnimationInterval();
	float getContentScaleFactor();
	CCDirectorDelegate *getDelegate() const;
	bool getDontCallWillSwitch() const;
	bool getForceSmoothFix() const;
	bool getIsTransitioning() const;
	TextureQuality getLoadedTextureQuality() const;
	CCScene *getNextScene();
	CCNode *getNotificationNode();
	CCEGLView *getOpenGLView();
	ccDirectorProjection getProjection();
	CCScene *getSceneReference() const;
	float getScreenBottom();
	float getScreenLeft();
	float getScreenRight();
	float getScreenScaleFactor();
	float getScreenScaleFactorH();
	float getScreenScaleFactorMax();
	float getScreenScaleFactorW();
	float getScreenTop();
	float getSecondsPerFrame();
	bool getSmoothFix() const;
	bool getSmoothFixCheck() const;
	int getSmoothFixCounter() const;
	unsigned int getTotalFrames();
	CCPoint getVisibleOrigin();
	CCSize getVisibleSize();
	CCSize getWinSize();
	CCSize getWinSizeInPixels();
	float getZEye();
	bool isDisplayStats();
	bool isNextDeltaTimeZero();
	bool isPaused();
	bool isSendCleanupToScene();
	int levelForSceneInStack(CCScene *);
	void pause();
	void popScene();
	bool popSceneWithTransition(float, PopTransition);
	void popToRootScene();
	void popToSceneInStack(CCScene *);
	void popToSceneStackLevel(int);
	void purgeCachedData();
	bool pushScene(CCScene *);
	void removeStatsLabel();
	bool replaceScene(CCScene *);
	void resetSmoothFixCounter();
	void reshapeProjection(const CCSize &);
	void resume();
	void runWithScene(CCScene *);
	int sceneCount();
	void setActualDeltaTime(float);
	void setAlphaBlending(bool);
	void setContentScaleFactor(float);
	void setDefaultValues();
	void setDelegate(CCDirectorDelegate *);
	void setDeltaTime(float);
	void setDepthTest(bool);
	void setDisplayStats(bool);
	void setDontCallWillSwitch(bool);
	void setForceSmoothFix(bool);
	void setGLDefaultValues();
	void setNextDeltaTimeZero(bool);
	void setNotificationNode(CCNode *);
	void setOpenGLView(CCEGLView *);
	void setProjection(ccDirectorProjection);
	void setSceneReference(CCScene *);
	void setSmoothFix(bool);
	void setSmoothFixCheck(bool);
	void setViewport();
	void setupScreenScale(CCSize, CCSize, TextureQuality);
	void updateContentScale(TextureQuality);
	void updateScreenScale(CCSize);
	void willSwitchToScene(CCScene *);

protected:
	void calculateDeltaTime();
	void calculateMPF();
	void createStatsLabel();
	void getFPSImageData(unsigned char **, unsigned int *);
	void purgeDirector();
	void setNextScene();
	void showStats();



public:
        inline CCScene* getRunningScene(void) { return m_pRunningScene; }
	virtual bool init();

	virtual long getClassTypeInfo();

	virtual void setAnimationInterval(double dValue);
	virtual void stopAnimation();
	virtual void startAnimation();
	virtual void mainLoop();
	virtual CCScheduler *getScheduler();
	virtual void setScheduler(CCScheduler *);
	virtual CCActionManager *getActionManager();
	virtual void setActionManager(CCActionManager *);
	virtual CCTouchDispatcher *getTouchDispatcher();
	virtual void setTouchDispatcher(CCTouchDispatcher *);
	virtual CCKeypadDispatcher *getKeypadDispatcher();
	virtual void setKeypadDispatcher(CCKeypadDispatcher *);
	virtual CCKeyboardDispatcher *getKeyboardDispatcher();
	virtual void setKeyboardDispatcher(CCKeyboardDispatcher *);
	virtual CCMouseDispatcher *getMouseDispatcher();
	virtual void setMouseDispatcher(CCMouseDispatcher *);
	virtual CCAccelerometer *getAccelerometer();
	virtual void setAccelerometer(CCAccelerometer *);
	virtual float getDeltaTime();
	virtual CCSceneDelegate *getSceneDelegate() const;
	virtual void setSceneDelegate(CCSceneDelegate *);

		/* The CCEGLView, where everything is rendered */
		CCEGLView    *m_pobOpenGLView;

		double m_dAnimationInterval;
		double m_dOldAnimationInterval;

		/* landscape mode ? */
		bool m_bLandscape;

		bool m_bDisplayStats;
		float m_fAccumDt;
		float m_fFrameRate;

		void *m_pFPSLabel;
		void *m_pSPFLabel;
		void *m_pDrawsLabel;

		/** Whether or not the Director is paused */
		bool m_bPaused;

		/* How many frames were called since the director started */
		unsigned int m_uTotalFrames;
		unsigned int m_uFrames;
		float m_fSecondsPerFrame;
		int a1;
		int a2;
		int a3;
		int a4;
		int a5;
		int a6;
		int a7;
		int a8;
		int a9;
		int a10;
		int a11;
		int a12;
		int a13;
		int a14;
		/* The running scene */
		CCScene *m_pRunningScene;

		/* will be the next 'runningScene' in the next frame
         nextScene is a weak reference. */
		CCScene *m_pNextScene;

		/* If YES, then "old" scene will receive the cleanup message */
		bool    m_bSendCleanupToScene;

		/* scheduled scenes */
		CCArray* m_pobScenesStack;

		/* last time the main loop was updated */
		struct cc_timeval *m_pLastUpdate;

		/* whether or not the next delta time will be zero */
		bool m_bNextDeltaTimeZero;

		/* projection used */
		ccDirectorProjection m_eProjection;

		/* window size in points */
		CCSize    m_obWinSizeInPoints;

		/* content scale factor */
		float    m_fContentScaleFactor;

		/* store the fps string */
		char *m_pszFPS;

		/* This object will be visited after the scene. Useful to hook a notification node */
		CCNode *m_pNotificationNode;

		/* Projection protocol delegate */
		CCDirectorDelegate *m_pProjectionDelegate;

		// CCEGLViewProtocol will recreate stats labels to fit visible rect
		friend class CCEGLViewProtocol;
};
#pragma pack(pop)

NS_CC_END