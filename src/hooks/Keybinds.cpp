#include "Keybinds.h"

#include "hooking.h"

using namespace cocos2d;

#define NUMBBETWEEN(numb, lower, higher) numb >= lower && numb <= higher

// convert
enumKeyCodes Keybinds::convertToCCkey(jint javaKeyCode) {
    int newkey = 0x20; // space is default

    // A - Z
    if(NUMBBETWEEN(javaKeyCode, 29, 54)) {
        newkey = javaKeyCode + 36;
    }

    // 1 - 9
    if(NUMBBETWEEN(javaKeyCode, 7, 16)) {
        newkey = javaKeyCode + 41;
    }

    // ARROW KEYS
    if(NUMBBETWEEN(javaKeyCode, 19, 22)) {
        newkey = javaKeyCode + 264;
    }

    // L/R ALT
    if(NUMBBETWEEN(javaKeyCode, 57, 58)) {
        newkey = javaKeyCode + 107;
    }

    // L/R SHIFT
    if(NUMBBETWEEN(javaKeyCode, 59, 60)) {
        newkey = javaKeyCode + 101;
    }

    // L/R CTRL
    if(NUMBBETWEEN(javaKeyCode, 113, 114)) {
        newkey = javaKeyCode + 49;
    }

    // ESC
    if(javaKeyCode == 111) newkey = 27;

    // DEL (for some reason the editor uses backspace for deleting???????)
    if(javaKeyCode == 67 || javaKeyCode == 112) newkey = 46;
    
    return (enumKeyCodes)newkey;
}

// special keys (ALT, CTRL, ETC)
void Keybinds::handleSpecialKeys(enumKeyCodes key, bool keyDown) {
    if(key == kLeftControl || key == kLeftShift || key == kLeftMenu) {
        auto dispatcher = CCDirector::sharedDirector()->getKeyboardDispatcher();

        // left shift
        if(key == kLeftShift) {
            // shift key bool
            MBO(bool, dispatcher, 0x40) = keyDown;
        }

        // left shift
        if(key == kLeftControl) {
            // ctrl key bool
            MBO(bool, dispatcher, 0x41) = keyDown;
        }

        // left alt
        if(key == kLeftMenu) {
            // alt key bool
            MBO(bool, dispatcher, 0x42) = keyDown;
        }
    }
}

/*
    KEYDOWN
*/
FUNCTIONHOOK(void, Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown, int env, int jobj, int keyCode) {
	Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDownO(env, jobj, keyCode);

    auto key = Keybinds::convertToCCkey(keyCode);
    CCDirector::sharedDirector()->getKeyboardDispatcher()->dispatchKeyboardMSG(key, true);

    Keybinds::handleSpecialKeys(key, true);
}

/*
    KEYUP
*/
extern "C" { // C symbol
    JNIEXPORT jboolean JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyUp(JNIEnv * env, jobject thiz, jint keyCode) {
        auto key = Keybinds::convertToCCkey(keyCode);
        CCDirector::sharedDirector()->getKeyboardDispatcher()->dispatchKeyboardMSG(key, false);

        Keybinds::handleSpecialKeys(key, false);

        return JNI_TRUE;
    }
}


// hooks
void Keybinds::ApplyHooks() {
    HOOK("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown", Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDownH, Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDownO);
}