#include <dlfcn.h>
#include "blaze.h"
#include "cocos2d.h"
#include "EditorUIExt.h"

EditorUI *EditorUIExt::EditorUI_hk()
{
    extern bool inEditor;
    inEditor = true;
    return EditorUI_trp(this);
}
