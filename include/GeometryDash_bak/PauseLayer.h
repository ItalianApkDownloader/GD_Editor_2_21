
#include "cocos2d.h"
#include "CCBlockLayer.h"

class  PauseLayer : public CCBlockLayer {
public:
	~PauseLayer();
	
	static PauseLayer *create();
	void customSetup();
	
	void keyBackClicked();
	
	
	int createToggleButton(std::string,void (cocos2d::CCObject::*)(cocos2d::CCObject*),bool,cocos2d::CCMenu *,cocos2d::CCPoint);
	void onEdit();
	void onNormalMode();
	void onPracticeMode();
	void onResume();
	void onQuit();
	void onRestart();
	void onMusic();
	void onFX();
	void onAutoCheck();
	void onHelp();
	
	void setupProgressBars();

	void onRecordReplays(cocos2d::CCObject *a2);
	
	
	
	
};
