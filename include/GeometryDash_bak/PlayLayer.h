
#include "cocos2d.h"
#include "GJGameLevel.h"
#include "PlayerObject.h"

enum EnterEffect {
	kEnterEffectDefault,
	kEnterEffectNone,
	kEnterEffectScaleUp,
	kEnterEffectScaleDown,
	kEnterEffectFinishTop,
	kEnterEffectFinishBottom,
	kEnterEffectFinishLeft,
	kEnterEffectFinishRight
};

class PlayerObject;
class CheckpointObject;

class PlayLayer : public CCLayer {
public:

	char _tmp[1*0x2E6];
	CCSprite* _progressBarProgressSprite;   //this + 254


	~PlayLayer();
	
	static CCScene *scene(GJGameLevel *level);
	static PlayLayer *create(GJGameLevel *level);
	int destroyPlayer( PlayerObject *, GameObject *);
	void levelComplete();
	void markCheckpoint();
	void flipGravity(PlayerObject*,bool,bool);
	void spawnFirework();
	void updateCamera(float dt);
	void onExit();
	
	void cameraMoveX(float xPos, float moveDuration, float easeRate);
	void commitJumps();
	void resetLevel();
	void update(float);
	void toggleProgressbar();
	void cameraMoveY(float,float,float);

	void willSwitchToMode(int,PlayerObject*);
	void playerWillSwitchMode(PlayerObject *a2, GameObject *a3);

};
