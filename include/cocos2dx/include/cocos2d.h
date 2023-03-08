#ifndef __COCOS2D_H__
#define __COCOS2D_H__

using namespace std;
#include <android/log.h>
#include <fmt/format.h>

#define CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

#define CCBOTTOM CCDirector::sharedDirector()->getScreenBottom()
#define CCTOP CCDirector::sharedDirector()->getScreenTop()
#define CCLEFT CCDirector::sharedDirector()->getScreenLeft()
#define CCRIGHT CCDirector::sharedDirector()->getScreenRight()
#define CCMIDX CCDirector::sharedDirector()->getScreenRight() / 2
#define CCMIDY CCDirector::sharedDirector()->getScreenTop() / 2
#define CCMIDL CCDirector::sharedDirector()->getScreenLeft() / 2

#define FMOD FMODAudioEngine::sharedEngine()

#define FUNCTIONHOOK(returntype, name, ...) \
static returntype (*name##O)(__VA_ARGS__);			\
static returntype name##H(__VA_ARGS__)



#define GPY() getPositionY()
#define GPX() getPositionX()

#define version2 14
#define version2str "1.4"
#define DEVDEBUG
//#define SHADERDEBUG
//#define EMUI_FIX

#define CallBySymbol(returntype, library, symbol, ...) (*(returntype(*)(__VA_ARGS__))reinterpret_cast<void*>(dlsym(dlopen(library, RTLD_LAZY), symbol)))

#define CBS(a, b, c, ...) CallBySymbol(a, b, c, __VA_ARGS__)
#define CBSR(a, c, ...) CBS(void*, "libcocos2dcpp.so", c, __VA_ARGS__)

#define __STR_CAT___(str1, str2) str1##str2
#define __STR_CAT__(str1, str2) __STR_CAT___(str1, str2)
#define PAD(size) char __STR_CAT__(pad, __LINE__)[size] = {};

#define TAG "cocos2x"

#ifdef DEVDEBUG
	#define CCLog(...) ((void)__android_log_print(3, TAG, __VA_ARGS__))
	#define fmtlog(...) CCLog("%s", fmt::format(__VA_ARGS__).c_str())
#else
	#define CCLog
	#define fmtlog
#endif
#define ccp(__X__,__Y__) cocos2d::CCPointMake((float)(__X__), (float)(__Y__))

// actions
#include "CCAction.h"
#include "CCActionInterval.h"
#include "CCActionCamera.h"
#include "CCActionManager.h"
#include "CCActionEase.h"
#include "CCActionInstant.h"
#include "CCActionTween.h"

// base_nodes
#include "CCNode.h"

// cocoa
#include "CCAffineTransform.h"
#include "CCDictionary.h"
#include "CCObject.h"
#include "CCArray.h"
#include "CCGeometry.h"
#include "CCSet.h"
#include "CCAutoreleasePool.h"
#include "CCInteger.h"
#include "CCFloat.h"
#include "CCDouble.h"
#include "CCBool.h"
#include "CCString.h"
#include "CCZone.h"
#include "ZipUtils.h"

#include "CCSpriteFrameCache.h"

#include "CCGrid.h"

#include "CCProtocols.h"
#include "ccTypes.h"




#include "CCKeypadDispatcher.h"
#include "CCFileUtils.h"


#include "CCLabelBMFont.h"

// layers_scenes_transitions_nodes
#include "CCLayer.h"
#include "CCScene.h"
#include "CCTransition.h"

// menu_nodes
#include "CCMenu.h"
#include "CCMenuItem.h"

// misc_nodes
#include "CCMotionStreak.h"
#include "CCRenderTexture.h"

#include "particle_nodes/CCParticleSystem.h"
#include "particle_nodes/CCParticleSystemQuad.h"

NS_CC_BEGIN

CC_DLL const char* cocos2dVersion();

NS_CC_END

#endif // __COCOS2D_H__
