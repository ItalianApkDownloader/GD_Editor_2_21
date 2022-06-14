#include "cocos2d.h"

namespace FMOD {
	class ChannelControl{
		public:
			void setPitch(float);
			void setPaused(bool);
	};

	class Sound{
	public:
		void setMusicSpeed(float musicSpeed);
	};

	class Channel{
	public:
		void setFrequency(float freq);
	};

}//*((FMOD::Channel **)this + 72)



class FMODAudioEngine  
{
	public:
		
		static FMODAudioEngine * sharedEngine();
		void playBackgroundMusic(std::string,bool,bool);
		void playEffect(std::string, bool, float, float, float);
		void stop();
		void start();
		void reloadEffects();
		void resumeAllEffects();

		void fadeBackgroundMusic(int i, double d) {

	}
};