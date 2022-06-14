#include "cocos2d.h"

using namespace cocos2d;

class GameLevelManager : public CCNode
{
	public:
		const char * getUserInfoKey(int);
		void * getGJUserInfo(int);
		static GameLevelManager * sharedState();
		void setDiffVal(int a2, bool a3);
		GJGameLevel* getSavedLevel(int);
		GJGameLevel* getSavedLevel(GJGameLevel *);
		int gotoLevelPage(GJGameLevel *);
		void saveLevel(GJGameLevel *);
		bool getBoolForKey(const char*);
		void  setBoolForKey(bool,const char*);
		CCScene* getSearchScene(const char*);
};