#include <dlfcn.h>
#include "blaze.h"
#include "cocos2d.h"
#include "MoreOptionsLayerExt.h"
#include "GDPSManager.h"

void MoreOptionsLayerExt::addToggle_hk(const char *title, const char *code, const char *desc)
{
    if (GDPSManager::sharedState()->isGDPSSettings)
    {
        if (strcmp(title, "Smooth fix in editor") == 0)
        {
            addToggle_trp(this, "Playtest", "100001", "Enable <cy>play test</c> in the editor.\nIt's really unstable so <cr>you shouldn't turn this option on</c>.\nWe are not responsible for any data loss.");
            addToggle_trp(this, "Level Info", "0109", "Show advanced level info (turn off for cleaner gameplay)");
            //    addToggle_trp(this,"Original Textures","100002","Use the original textures instead of the new one.");
            addToggle_trp(this, "Remove pause button", "100005", "Meant for cleaner recordings, use the back button or the ESC key on emulator to go back");
            addToggle_trp(this, "Disable linked objects\nin editor", "100006", "To use the editor with large levels with around <cr>80K</c> objects or more you will need to <cy>activate this option</c>, otherwise <cr>entering editor may crashes</c>. After activating this option, the objects linked with the \"link controls\" option will not be linked anymore but you will be able to open the editor. <cg>Do not enable this option if you are not dealing with so many objects.</c>");
            addToggle_trp(this, "Show editor shortcut", "100007", "Adds an editor shortcut at the bottom right of the menu");
            addToggle_trp(this, "Timer", "100008", "Adds a timer for level speedruns, red means the attempt is invalid");

            GDPSManager::sharedState()->isGDPSSettings = false;
        }
    }
    else
    {
        addToggle_trp(this, title, code, desc);
    }
}

void MoreOptionsLayerExt::onToggle_hk(CCObject *sender)
{
}
