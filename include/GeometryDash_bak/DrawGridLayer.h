#include "cocos2d.h"

using namespace cocos2d;

class DrawGridLayer : public CCLayer{
public:
    static DrawGridLayer* create(CCNode*,LevelEditorLayer*);

    void updateTimeMarkers( );
};