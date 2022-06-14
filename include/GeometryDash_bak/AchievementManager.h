#include "cocos2d.h"
#include "GeometryDash.h"

class  AchievementManager : public CCNode {
	public:
		static AchievementManager * sharedState();
		CCArray * getAllAchievements();
		AchievementManager();
		bool init();
		void firstSetup();
		void notifyAchievementWithID(const char* identifier);
		void notifyAchievement(char const* title,char const* desc,char const* icon);
		bool areAchievementsEarned(cocos2d::CCArray *);
		bool isAchievementEarned(char const* identifier);
		int percentForAchievement(char const* identifier);
		int percentageForCount(int,int);                        //?? 
		void reportAchievementWithID(char const* identifier,int percentage,bool hidePopup);
		void reportPlatformAchievementWithID(char const*,int);  //?? 
		void resetAchievement(char const* identifier);
		void resetAchievements(void);
		void setReportedAchievements(cocos2d::CCDictionary *);
		_UNIMPLEMENTED void setup();
		void storeAchievementUnlocks();
		~AchievementManager();
		
};