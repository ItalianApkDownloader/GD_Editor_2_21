#include "cocos2d.h"
class ImGuiOverlay {
public:
    static void ApplyHooks();
};

void draw_tree_for_node(cocos2d::CCNode* node);
void imgui_draw_cocos_window();
void imgui_draw_cocos_window(cocos2d::CCNode*);