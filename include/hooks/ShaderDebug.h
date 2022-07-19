#include "cocos2d.h"

class UILayerDebug : cocos2d::CCLayer {
public:
    void scheduled(float);
    void doInit();
};

class DevDebugHooks {
public:
    static void ApplyHooks();
};