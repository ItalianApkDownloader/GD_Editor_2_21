#include "cocos2d.h"
#include "gd.h"

class Options : public FLAlertLayer {
public:

    static void ApplyHooks();
	void onGDPSSettings(CCObject*);
	void onHacks(CCObject*);
	void onNormalSettings(CCObject*);
	void onEditorSettings(CCObject*);
	void platformOpacityLeft(CCNode*);
	void platformOpacityRight(CCObject*);
};