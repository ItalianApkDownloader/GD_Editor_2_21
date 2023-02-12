#include "cocos2d.h"

#include <jni.h>

class Keybinds {
public:
    static void ApplyHooks();
    static cocos2d::enumKeyCodes convertToCCkey(jint);
    static void handleSpecialKeys(cocos2d::enumKeyCodes, bool);
};