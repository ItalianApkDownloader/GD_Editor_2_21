#include <cocos2d.h>
#include "gd.h"

class PauseLayerExt : public PauseLayer {	
public:

CCLayer* firstLayer;
CCLayer* secondLayer;
CCArray* firstArray;

public:


    void onNextPage(CCObject * sender);
    void onPrevPage(CCObject * sender);

    void onPlatformOpacity(CCObject * sender);
    void onTimerOpacity(CCObject * sender);
    void onToggleTimer(CCObject * sender);



    static inline bool (*init_trp)(PauseLayer*);
    bool init_hk();






};