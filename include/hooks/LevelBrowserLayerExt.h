#pragma once
#include <gd.h>
#include "cocos2d.h"
#include "LevelBrowserLayer.h"
#include "GJSearchObject.h"
class  LevelBrowserLayerExt : public LevelBrowserLayer {
public:

    static inline  bool (*init_trp)(LevelBrowserLayer*, GJSearchObject*);
    bool init_hk(GJSearchObject* a2);

};
