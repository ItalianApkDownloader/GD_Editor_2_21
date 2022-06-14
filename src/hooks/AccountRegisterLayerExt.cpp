#include <gd.h>
#include "cocos2d.h"
#include "AccountRegisterLayerExt.h"
//#include "../../include/hooks/MenuLayerExt.h"

bool AccountRegisterLayerExt::init_hk()
{
    auto ret = AccountRegisterLayerExt::init_trp(this);
    {

        AccountRegisterLayerExt::onMenu();

        auto app = cocos2d::CCApplication::sharedApplication();
        auto url = ("http://game.gdpseditor.com/server/tools/account/registerAccount.php");

        app->openURL(url);

        return ret;
    }
}