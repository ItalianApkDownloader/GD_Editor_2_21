#include "cocos2d.h"
#include <iostream>
#include <vector>
#include <stdarg.h>
class ImGuiOverlay {
public:
    static void ApplyHooks();
    static std::vector<const char*> debug_print;
};
    
void draw_tree_for_node(cocos2d::CCNode* node);
void imgui_draw_cocos_window();
void imgui_draw_cocos_window(cocos2d::CCNode*);
void ClearDebugPrint();
void CleanDebugPrint();
void EmptyDebugPrint();
void AddDebugLine(const char*);
