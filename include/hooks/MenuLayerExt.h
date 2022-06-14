//#include <ColoursPalette.h>
#include <sstream>
#include "../cocos2dx/extensions/network/HttpClient.h"
#include "../cocos2dx/extensions/network/HttpRequest.h"
#include "../cocos2dx/extensions/network/HttpResponse.h"
#include "cocos2d.h"
//#include "rapidjson/rapidjson.h"
//#include "rapidjson/document.h"
//#include "patch.h"
//#include "KittyMemory.h"
//#define version2 2207
//#include "../Encryption/Encryption.h"
//#include "../ZLIB/zlib.h"
//#include "../GDPSManager.h"
//#include "../obfuscate.h"
//#include "../layers/advancedOptionsLayer.h"
//#include "../layers/CreditsLayer.h"





class SliderThumb {
public:
    float getValue();
};

/*
class Slider : public CCSprite{
public:
    static Slider* create(cocos2d::CCNode *,void (cocos2d::CCObject::*)(cocos2d::CCObject*),float);
    void setValue(float);
    void setBarVisibility(bool);
    float getValue();

};
*/




template<class T>
extern void* getPointer(T value);


//this->addChild(CCParticleFire::create());



namespace cocos2d{
    class CCParticleFire : public CCParticleSystemQuad
    {
    public:

        CCParticleFire(){}

        virtual ~CCParticleFire(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleFire* create();
        static CCParticleFire* createWithTotalParticles(unsigned int numberOfParticles);
    };


    class CCParticleFireworks : public CCParticleSystemQuad
    {
    public:

        CCParticleFireworks(){}

        virtual ~CCParticleFireworks(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleFireworks* create();
        static CCParticleFireworks* createWithTotalParticles(unsigned int numberOfParticles);
    };



    class CCParticleSun : public CCParticleSystemQuad
    {
    public:

        CCParticleSun(){}

        virtual ~CCParticleSun(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleSun* create();
        static CCParticleSun* createWithTotalParticles(unsigned int numberOfParticles);
    };
	
	    class CCParticleGalaxy : public CCParticleSystemQuad
    {
    public:

        CCParticleGalaxy(){}

        virtual ~CCParticleGalaxy(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleGalaxy* create();
        static CCParticleGalaxy* createWithTotalParticles(unsigned int numberOfParticles);
    };
	
	    class CCParticleFlower : public CCParticleSystemQuad
    {
    public:

        CCParticleFlower(){}

        virtual ~CCParticleFlower(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleFlower* create();
        static CCParticleFlower* createWithTotalParticles(unsigned int numberOfParticles);
    };
	
	    class CCParticleMeteor : public CCParticleSystemQuad
    {
    public:

        CCParticleMeteor(){}

        virtual ~CCParticleMeteor(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleMeteor* create();
        static CCParticleMeteor* createWithTotalParticles(unsigned int numberOfParticles);
    };
	
	    class CCParticleSpiral : public CCParticleSystemQuad
    {
    public:

        CCParticleSpiral(){}

        virtual ~CCParticleSpiral(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleSpiral* create();
        static CCParticleSpiral* createWithTotalParticles(unsigned int numberOfParticles);
    };
	
	    class CCParticleExplosion : public CCParticleSystemQuad
    {
    public:

        CCParticleExplosion(){}

        virtual ~CCParticleExplosion(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleExplosion* create();
        static CCParticleExplosion* createWithTotalParticles(unsigned int numberOfParticles);
    };
	
	    class CCParticleSmoke : public CCParticleSystemQuad
    {
    public:

        CCParticleSmoke(){}

        virtual ~CCParticleSmoke(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleSmoke* create();
        static CCParticleSmoke* createWithTotalParticles(unsigned int numberOfParticles);
    };
	
	    class CCParticleSnow : public CCParticleSystemQuad
    {
    public:

        CCParticleSnow(){}

        virtual ~CCParticleSnow(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleSnow* create();
        static CCParticleSnow* createWithTotalParticles(unsigned int numberOfParticles);
    };
	
	    class CCParticleRain : public CCParticleSystemQuad
    {
    public:

        CCParticleRain(){}

        virtual ~CCParticleRain(){}
        bool init(){ return initWithTotalParticles(700); }
        virtual bool initWithTotalParticles(unsigned int numberOfParticles);

        static CCParticleRain* create();
        static CCParticleRain* createWithTotalParticles(unsigned int numberOfParticles);
    };
	
}



class MenuLayerExt : public MenuLayer {
public:

FLAlertLayer* alert;



public:
		
void onRequestCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
	
 


		
	
    void onDownload(CCObject* sender);
    void onJoinDiscord(CCObject* sender);
	void onUpdate(float dt) {
		//add all this extern shit to a class or something
		extern float timer;
		timer = timer + dt;
		
		std::ostringstream os;
		os << timer;
	string a = os.str();

	extern CCLabelBMFont* timerLabel;
		timerLabel->setString(a.c_str());
	};


    void onProgressChanged(CCObject *sender);


	void showUpdateAlert(string version, string weight, string date, string changelog);
    void onBlaze(CCObject* sender);
	void onPopupTest(CCObject* sender);
	virtual void FLAlert_Clicked(FLAlertLayer* alert, bool btn2);
	
    static inline bool (*init_trp)(MenuLayer * self);
    bool init_hk();

};



/*
server adresses

http://ps.fhgdps.com/GDPSEditor22

0x7A8513
0x7A8941	
0x7A8C21	
0x7A8CB0	
0x7A8D5C	
0x7A8DE7	
0x7A8DE7	
0x7A8DE7	
0x7A8EAA	
0x7A8EE5	
0x7A8F53	
0x7A8FCC	
0x7A9054	
0x7A90BB	
0x7A9107	
0x7A914B	
0x7A918A	
0x7A91D3	
0x7A922C	
0x7A92A4	
0x7A9353	
0x7A93A3	
0x7A93FA	
0x7A94A0	
0x7B7A1D	
0x7B8191
0x7B8202	
0x7B8278	
0x7B8446	
0x7B8A5D	
0x7B8A99	
*/