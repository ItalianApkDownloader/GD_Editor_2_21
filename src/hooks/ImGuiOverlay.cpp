#include "ImGuiOverlay.h"
#include <map>
#include <vector>
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"
#include "GDPSHelper.h"
#include "FunctionHelper.h"
#include "gd.h"


#include "../../include/imgui/imgui.h"
#include "../../include/imgui/backends/imgui_impl_opengl3.h"
#include "../../include/imgui/backends/imgui_impl_glfw.h"

using namespace cocos2d;
using namespace ImGui;



#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)


bool LOGGER_TRANSLUCENT = false;
float LOGGER_ALPHA = 0.9f;
bool LOGGER_AUTOSCROLL = true;

bool SCENE_TRANSLUCENT = false;
float SCENE_ALPHA = 0.9f;

bool SHOW_DEBUG_OPTIONS = false;
bool SHOW_LOGGER = false;
bool SHOW_VARS = false;
bool SHOW_EXPLORER = false;
bool SHOW_DEMO = false;
bool SHOW_TEST = true;





bool GAME_IN_WINDOW = false;
cocos2d::CCRenderTexture* GAME_TEX = nullptr;

constexpr float DPI_NORMAL_FACTOR = 432.0f;
float DT_COUNTER = 0.0f;
uint32_t FRAME_COUNTER = 0u;
float CURRENT_FPS = 0.0f;
bool IS_ACTIVE = false;

#include <typeinfo>
#include <memory>
#include <cxxabi.h>

vector<const char*> ImGuiOverlay::debug_print = {"init, empty"};
vector<const char*> debug_messages = { "init, empty" };


std::string get_name_for_node(cocos2d::CCNode* node)
{
    // stolen from mat (thanks mat)
    // and also stolen from https://stackoverflow.com/a/4541470

    auto name = typeid(*node).name();
    auto status = -1;

    std::unique_ptr<char, void (*)(void*)> res {
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    };

    return (status == 0) ? res.get() : name;
}


void imgui_draw_vars_window()
{
    if (ImGui::Begin("Variables", &SHOW_VARS, ImGuiWindowFlags_NoResize)) {
        auto gm = GameManager::sharedState();

        auto nc_val = gm->getGameVariable("4099");
        if (ImGui::Checkbox("Ignore Damage", &nc_val)) {
            gm->toggleGameVariable("4099");
        }

        auto pm_val = gm->getGameVariable("4098");
        if (ImGui::Checkbox("Practice Music", &pm_val)) {
            gm->toggleGameVariable("4098");
        }

        auto sc_val = gm->getGameVariable("4093");
        if (ImGui::Checkbox("Speedhack Controls", &sc_val)) {
            gm->toggleGameVariable("4093");
        }

        auto ru_val = gm->getGameVariable("4094");
        if (ImGui::Checkbox("Replay Utils", &ru_val)) {
            gm->toggleGameVariable("4094");
        }

        auto vfb_val = gm->getGameVariable("3001");
        if (ImGui::Checkbox("Bypass Verify", &vfb_val)) {
            gm->toggleGameVariable("3001");
        }
    }

    ImGui::End();
}

void draw_tree_for_node(cocos2d::CCNode* node)
{
    auto ccd = cocos2d::CCDirector::sharedDirector();
    float dpi = CallBySymbol(float, "libcocos2dcpp.so", "_ZN7cocos2d8CCDevice6getDPIEv", void)();
        
    if (ImGui::TreeNode(get_name_for_node(node).c_str())) {
        ImGui::PushItemWidth(std::min(ImGui::GetWindowWidth() * 0.65f, (dpi / DPI_NORMAL_FACTOR) * 400.0f));
        {
            auto px = node->getPositionX();
            auto py = node->getPositionY();
            float pos_vals[] = { px, py };

            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            if (ImGui::DragFloat2("Pos.", pos_vals, 0.1f, -FLT_MAX, FLT_MAX, "%.1f")) {
                node->setPosition(cocos2d::CCPoint(pos_vals[0], pos_vals[1]));
            }
        }

        {
            auto scale = node->getScale();
            auto sx = node->getScaleX();
            auto sy = node->getScaleY();
            float scale_vals[] = { scale, sx, sy };

            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            if (ImGui::DragFloat3("Scale", scale_vals, 0.01f, -FLT_MAX, FLT_MAX, "%.2f")) {
                if (scale_vals[1] == scale_vals[2]) {
                    node->setScale(scale_vals[0]);
                } else {
                    node->setScaleX(scale_vals[1]);
                    node->setScaleY(scale_vals[2]);
                }
            }
        }
        {
            auto rotation = node->getRotation();
            auto rx = node->getRotationX();
            auto ry = node->getRotationY();
            float rot_vals[] = { rotation, rx, ry };

            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            if (ImGui::DragFloat3("Rot.", rot_vals, 0.1f, -FLT_MAX, FLT_MAX, "%.1f")) {
                if (rot_vals[1] == rot_vals[2]) {
                    node->setRotation(rot_vals[0]);
                } else {
                    node->setRotationX(rot_vals[1]);
                    node->setRotationY(rot_vals[2]);
                }
            }
        }

        {
            auto is_visible = node->isVisible();
            if (ImGui::Checkbox("Visible", &is_visible)) {
                node->setVisible(is_visible);
            }
        }

        if (dynamic_cast<cocos2d::CCRGBAProtocol*>(node) != nullptr) {
            auto c_node = dynamic_cast<cocos2d::CCRGBAProtocol*>(node);

            auto color = c_node->getColor();
            auto opacity = c_node->getOpacity();

            float color_vals[] = {
                static_cast<float>(color.r) / 255.0f,
                static_cast<float>(color.g) / 255.0f,
                static_cast<float>(color.b) / 255.0f,
                static_cast<float>(opacity) / 255.0f };

            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            if (ImGui::ColorEdit4("Color", color_vals, ImGuiColorEditFlags_Uint8)) {
                c_node->setColor({ static_cast<uint8_t>(color_vals[0] * 255),
                    static_cast<uint8_t>(color_vals[1] * 255),
                    static_cast<uint8_t>(color_vals[2] * 255) });
                c_node->setOpacity(color_vals[3] * 255);
            }
        }

        if (node->getChildrenCount() >= 1) {
            if (ImGui::TreeNode("node_children", "Children (%i)", node->getChildrenCount())) {
                auto children = node->getChildren();
                for (auto i = 0u; i < children->count(); i++) {
                    auto child = reinterpret_cast<cocos2d::CCNode*>(children->objectAtIndex(i));

                    draw_tree_for_node(child);
                }

                ImGui::TreePop();
            }
        }

        ImGui::PopItemWidth();
        ImGui::TreePop();
    }
}

void imgui_draw_cocos_window()
{
    constexpr auto window_height = 600.0f;
    constexpr auto window_width = 800.0f;
    constexpr auto window_xoffset = 120.0f;
    constexpr auto window_yoffset = 60.0f;

    float dpi = CallBySymbol(float, "libcocos2dcpp.so", "_ZN7cocos2d8CCDevice6getDPIEv", void)();
    auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
    auto pixel_size = eglview->getFrameSize();

//  ImGui::SetNextWindowSize(ImVec2((dpi / DPI_NORMAL_FACTOR) * window_width, (dpi / DPI_NORMAL_FACTOR) * window_height), ImGuiCond_FirstUseEver);
  //  ImGui::SetNextWindowPos(ImVec2(window_xoffset, (pixel_size.height - window_height) - window_yoffset), ImGuiCond_FirstUseEver);

    float backup_alpha = ImGui::GetStyle().Alpha;
    if (SCENE_TRANSLUCENT) {
        ImGui::GetStyle().Alpha = SCENE_ALPHA;
    }

    if (ImGui::Begin("Scene", &SHOW_EXPLORER)) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {800.f,600.f });
        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Translucent", &SCENE_TRANSLUCENT);
            if (SCENE_TRANSLUCENT) {
                ImGui::SliderFloat("Alpha", &SCENE_ALPHA, 0.25f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
            }

            ImGui::EndPopup();
        }

        if (ImGui::Button("Options")) {
            ImGui::OpenPopup("Options");
        }

        auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (scene == nullptr) {
            ImGui::Text("No running scene!");
        } else {
            ImGui::SetNextItemOpen(true);
            draw_tree_for_node(scene);
        }

        ImGui::EndChild();
    }

    if (ImGui::GetStyle().Alpha != backup_alpha) {
        ImGui::GetStyle().Alpha = backup_alpha;
    }

    ImGui::End();
}


void imgui_draw_cocos_window(CCNode* self)
{
    constexpr auto window_height = 600.0f;
    constexpr auto window_width = 800.0f;
    constexpr auto window_xoffset = 120.0f;
    constexpr auto window_yoffset = 60.0f;

    float dpi = CallBySymbol(float, "libcocos2dcpp.so", "_ZN7cocos2d8CCDevice6getDPIEv", void)();
    auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
    auto pixel_size = eglview->getFrameSize();

//  ImGui::SetNextWindowSize(ImVec2((dpi / DPI_NORMAL_FACTOR) * window_width, (dpi / DPI_NORMAL_FACTOR) * window_height), ImGuiCond_FirstUseEver);
  //  ImGui::SetNextWindowPos(ImVec2(window_xoffset, (pixel_size.height - window_height) - window_yoffset), ImGuiCond_FirstUseEver);

    float backup_alpha = ImGui::GetStyle().Alpha;
    if (SCENE_TRANSLUCENT) {
        ImGui::GetStyle().Alpha = SCENE_ALPHA;
    }

    if (ImGui::Begin("Scene", &SHOW_EXPLORER)) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {800.f,600.f });
        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Translucent", &SCENE_TRANSLUCENT);
            if (SCENE_TRANSLUCENT) {
                ImGui::SliderFloat("Alpha", &SCENE_ALPHA, 0.25f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
            }

            ImGui::EndPopup();
        }

        if (ImGui::Button("Options")) {
            ImGui::OpenPopup("Options");
        }

        auto scene = self;

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            ImGui::Text("No running scene!");

        ImGui::EndChild();
    }

    if (ImGui::GetStyle().Alpha != backup_alpha) {
        ImGui::GetStyle().Alpha = backup_alpha;
    }

    ImGui::End();
}
std::string testdebug = "0";
void imgui_showTest() {
	
// Create a window called "My First Tool", with a menu bar.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {800.f,600.f });
    ImGui::Begin("Debug logs", &SHOW_TEST, ImGuiWindowFlags_MenuBar);
/*
for(const char* i : debug_messages) {
    Text(i);
    //CCLog(i);
}
*/
if(ImGui::TreeNode("Logs 1")) {
    Text(testdebug.c_str());
    ImGui::TreePop();
}
ImGui::End();
}

void AddDebugLine(const char* str) {
    debug_messages.push_back(str);
}
void CleanDebugPrint() {
    debug_messages.clear();
}
void ClearDebugPrint() {
    debug_messages.clear();
}
void EmptyDebugPrint() {
    debug_messages.clear();
}

void imgui_tick(void* tex = nullptr)
{
    if (!IS_ACTIVE) {
        return;
    }

    if (SHOW_DEMO) {
        ImGui::ShowDemoWindow(&SHOW_DEMO);
    }

    if (SHOW_VARS) {
        imgui_draw_vars_window();
    }

    if (SHOW_EXPLORER) {
        imgui_draw_cocos_window();
    }
    if(SHOW_TEST) {
        imgui_showTest();
    }

}
void MySaveFunction() {
	CCLog("save pressed");
}


void imgui_init() {
	
	if (IS_ACTIVE) {
        return;
    }
	
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImFont* font = io.Fonts->AddFontDefault();
    
    //edit this value to change main font size
    font->Scale *= 3;

    io.BackendPlatformName = "imgui_impl_cocos2dx";
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    // official example recommends using <Load/Save>IniSettingsToMemory but i can't be bothered rn
    io.IniFilename = nullptr;

    //    ImGui_ImplAndroid_Init(window);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    float dpi = CallBySymbol(float, "libcocos2dcpp.so", "_ZN7cocos2d8CCDevice6getDPIEv", void)();

    ImGui::GetStyle().ScaleAllSizes((dpi / DPI_NORMAL_FACTOR) * 10.0f);
   // imgui_stylecolorslightgreen(nullptr);

	IS_ACTIVE = true;

}

FUNCTIONHOOK(void*, Application_run, void* self) {

	auto ret = Application_runO(self);
    
	
	imgui_init();
	
	return ret;
}



FUNCTIONHOOK(void, CCScheduler_update, cocos2d::CCScheduler* self, float dt)
{
    FRAME_COUNTER++;
    DT_COUNTER += dt;

    if (DT_COUNTER >= 0.5f) {
        CURRENT_FPS = FRAME_COUNTER / DT_COUNTER;

        FRAME_COUNTER = 0u;
        DT_COUNTER = 0.0f;
    }

    return CCScheduler_updateO(self, dt);
}


void imgui_end_frame()
{
    if (!IS_ACTIVE) {
        return;
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void imgui_start_frame()
{
    if (!IS_ACTIVE) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();

    auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
    auto pixel_size = eglview->getFrameSize();

    io.DisplaySize = ImVec2(pixel_size.width, pixel_size.height);
    if (pixel_size.width > 0.0f && pixel_size.height > 0.0f) {
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    }

    ImGui::NewFrame();
}

FUNCTIONHOOK(void, CCDirector_drawScene, cocos2d::CCDirector* self)
{
    CCDirector_drawSceneO(self);
    imgui_start_frame();
    
   // GDPSHelper::logNames(self->getRunningScene()->getChildren());

    if (IS_ACTIVE) {
        imgui_tick(nullptr);
        imgui_end_frame();

        glFlush();
    }
}


FUNCTIONHOOK(void, CCTouchDispatcher_touches, cocos2d::CCTouchDispatcher* self, cocos2d::CCSet* touches, cocos2d::CCEvent* event, uint32_t index)
{
    // note that touch support is still very buggy. like incredibly buggy
    // hopefully in the future it can be stabilized
    if (!IS_ACTIVE) {
        CCTouchDispatcher_touchesO(self, touches, event, index);
        return;
    }

    ImGuiIO& io = ImGui::GetIO();

    // this prevents "hovering" from beginning capture
    bool pass_mouse_pos_to_gui = io.WantCaptureMouse;

    switch (index) {
    case 0: { // CCTOUCHBEGAN
        auto touch = reinterpret_cast<cocos2d::CCTouch*>(touches->anyObject());
        if (touch != nullptr) {
            auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
            auto loc = touch->getLocationInView();

            io.AddMousePosEvent(loc.x * eglview->getScaleX(), loc.y * eglview->getScaleY());
        }

        io.AddMouseButtonEvent(0, true);
        break;
    }
    case 1: { // CCTOUCHMOVED
        auto touch = reinterpret_cast<cocos2d::CCTouch*>(touches->anyObject());
        if (touch != nullptr && pass_mouse_pos_to_gui) {
            auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
            auto loc = touch->getLocationInView();

            io.AddMousePosEvent(loc.x * eglview->getScaleX(), loc.y * eglview->getScaleY());
        }
        break;
    }
    case 2: // CCTOUCHENDED
    case 3: // CCTOUCHCANCELLED
        io.AddMouseButtonEvent(0, false);
        break;
    }

    // always pass end/cancel events. may introduce crashes but should fix other bugs
    if (!io.WantCaptureMouse || index == 2 || index == 3) {
        CCTouchDispatcher_touchesO(self, touches, event, index);
    }
}






void ImGuiOverlay::ApplyHooks() 
{	
	HOOK("_ZN7cocos2d13CCApplication3runEv", Application_runH, Application_runO);
	HOOK("_ZN7cocos2d11CCScheduler6updateEf", CCScheduler_updateH, CCScheduler_updateO);
	HOOK("_ZN7cocos2d10CCDirector9drawSceneEv", CCDirector_drawSceneH, CCDirector_drawSceneO);
	HOOK("_ZN7cocos2d17CCTouchDispatcher7touchesEPNS_5CCSetEPNS_7CCEventEj", CCTouchDispatcher_touchesH, CCTouchDispatcher_touchesO);
}