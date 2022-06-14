#include <dlfcn.h>
#include "blaze.h"
#include "cocos2d.h"
#include "PlayLayerExt.h"
#include "GDPSManager.h"
#include "patch.h"
#include "gd.h"
#include "CCMenuItemToggler.h"
#include <sstream>

template <class T>
void *getPointer(T value)
{
    auto val = reinterpret_cast<void *&>(value);
    return val;
}



std::string PlayLayerExt::formatSecond(float value)
{
    /*
        r - remainder
        m - minute
        s - second
    */

    float r = value;

    int m = r / 60;
    r = fmod(r, 60.0);

    int s = r;

    std::string mm = GDPS->itos(m);
    std::string ss = m > 0 ? std::string(std::max(2 - static_cast<int>(GDPS->itos(s).length()), 0), '0') + GDPS->itos(s) : GDPS->itos(s);
    std::string msms = std::string(CCString::createWithFormat("%.3f", r - floor(r))->getCString()).erase(0, 2);

    std::string result;

    if (m != 0)
    {
        result = mm + ":" + ss + "." + msms;
    }
    else
    {
        result = ss + "." + msms;
    }

    return result;
}

void PlayLayerExt::onUpdateTimer(float dt)
{
    extern float timer;
    timer = timer + dt;

    extern CCLabelBMFont *timerLabel;
    timerLabel->setString(formatSecond(timer).c_str());
};

void PlayLayerExt::levelCompleted_hk()
{
	        levelCompleted_trp(this);

	if(GM->getGameVariable("100008")) {
        
            PlayLayer* pl = GameManager::sharedState()->_playLayer();

        UILayer * layer = reinterpret_cast<UILayer *>(*((int *) pl + 409));

        layer->unschedule(schedule_selector(PlayLayerExt::onUpdateTimer));
	}
}

void PlayLayerExt::togglePracticeMode_hk(bool practiceMode) {
	

	togglePracticeMode_trp(this, practiceMode);
	
			if(GM->getGameVariable("100008")) {


	cocos2d::_ccColor3B	color = practiceMode ? ccc3(255, 0, 0) : ccc3(255, 255, 255);
	
    extern CCLabelBMFont *timerLabel;
	timerLabel->setColor(color);
		}
}

void PlayLayerExt::resume_hk() {
	
	
	resume_trp(this);
		if(GM->getGameVariable("100008")) {

	cocos2d::_ccColor3B	color = ccc3(255, 0, 0);
	
    extern CCLabelBMFont *timerLabel;
	timerLabel->setColor(color);
		}
}


void PlayLayerExt::resetLevel_hk()
{
    resetLevel_trp(this);
	if(GM->getGameVariable("100008")) {

		bool practiceMode = this->_practiceMode();
		
		if(!practiceMode) {
		    extern float timer;
			timer = 0;
			
			cocos2d::_ccColor3B	color = ccc3(255, 255, 255);
	
			extern CCLabelBMFont *timerLabel;
			timerLabel->setColor(color);
		}

    UILayer *layer = reinterpret_cast<UILayer *>(*((int *)this + 409));
    layer->schedule(schedule_selector(PlayLayerExt::onUpdateTimer), 0.01);
	}
}

void PlayLayerExt::destroyPlayer_hk(PlayerObject *self, int a2)
{
    destroyPlayer_trp(self, a2);
			if(GM->getGameVariable("100008")) {

    PlayLayer *pl = GameManager::sharedState()->_playLayer();

    UILayer *layer = reinterpret_cast<UILayer *>(*((int *)pl + 409));

    layer->unschedule(schedule_selector(PlayLayerExt::onUpdateTimer));
			}
}

void PlayLayerExt::update_hk(float a1)
{
    update_trp(this, a1);

    UILayer *layer = reinterpret_cast<UILayer *>(*((int *)this + 409));

    if (*(int *)((long)layer + 502))
    { // is platformer

        auto sp = reinterpret_cast<CCSprite *>(reinterpret_cast<CCMenu *>(layer->getChildren()->objectAtIndex(0)));

        sp->setOpacity(GDPS->opacity);
    }
	
	if(GM->getGameVariable("100008")) {
		extern CCLabelBMFont* timerLabel;
		timerLabel->setOpacity(GDPS->opacityTimer);
	}

}
