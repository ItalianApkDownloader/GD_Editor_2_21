#include "GeometryDash.h"
#include "cocos2d.h"

USING_NS_CC;

/* Dropdown bar for displaying unlocked achievements */
class AchievementBar : public CCNode {
public:
	static AchievementBar *create(const char *title, const char *description, const char *icon);
	
	bool init(const char *title, const char *description, const char *icon);
	
	CCLayerColor *internalLayer_;
	/* Position after the end of dropdown animation. */
	float yPos_;
	
	void show();
	
	CC_SYNTHESIZE(CCNode*, targetScene_, TargetScene);
};

class AchievementNotifier : public CCNode {
public:
	~AchievementNotifier();
	
	static AchievementNotifier *sharedState();
	
	bool init();
	void notifyAchievement(const char *title, const char *description, const char *icon,bool);
	
	CCArray *notificationQueue_;
	AchievementBar *showingNotification_;
	
	void showNextAchievement();
	
	/* Automatically called by AppDelegate */
	void willSwitchToScene(CCScene *scene);
};