#include "cocos2d.h"


USING_NS_CC;

enum SoundManagerState {
	kSoundManagerStopped,
	kSoundManagerReady = 4
};



class  GameSoundManager : public CCNode {
public:
	static GameSoundManager *sharedManager();
	bool init();
	
	void playBackgroundMusic(std::string,bool,bool);
	void playBackgroundMusic(std::string,bool,bool,bool);
	void asynchronousSetup();
	void setup();
	
	void setBGMusicVolume(float value);
	void stopBackgroundMusic();

	CCDictionary *uniqueEffects_;
	float uniqueEffectsTime_;
	
	void preload();
	bool isLoopedSoundPlaying(std::string);

	void fadeInMusic(bool b) ;
};