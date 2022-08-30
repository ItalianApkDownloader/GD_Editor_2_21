#include "cocos2d.h"
#include "gd.h"

class DPADHooks {
public:
    static void UILayerInit(UILayer* layer);
    static void EditorUIInit(EditorUI* layer);

    static void ApplyHooks();
};