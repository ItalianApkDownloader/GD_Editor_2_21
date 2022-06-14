#include <gd.h>
#include "cocos2d.h"
#include "MoreSearchLayerExt.h"

void MoreSearchLayerExt::addLayerToggle(int row, int column, std::string name, SEL_MenuHandler handler, bool disabled)
{
    auto menu = reinterpret_cast<CCMenu *>(*((int *)this + 105));
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    float x = winSize.width * 0.5 - 170;
    float y = winSize.height * 0.5 + 75;
    this->createToggleButton(name, handler, disabled, menu, ccp(x + (column * 130), y - (row * 50)));
}

bool MoreSearchLayerExt::init_hk()
{
    auto ret = init_trp(this);
    bool selected = GLM->getBoolForKey("platform_filter_custom") ^ 1;
    bool selected2 = GLM->getBoolForKey("legendary_filter_custom") ^ 1;
    // this->addLayerToggle(3,2,"Platformer",menu_selector(MoreSearchLayerExt::onPlatformToggle),selected);
    this->addLayerToggle(3, 0, "Legendary", menu_selector(MoreSearchLayerExt::onLegendaryToggle), selected2);
    return ret;
}

void MoreSearchLayerExt::onPlatformToggle(CCObject *sender)
{
    bool selected = GLM->getBoolForKey("platform_filter_custom") ^ 1;
    GLM->setBoolForKey(selected, "platform_filter_custom");
}

void MoreSearchLayerExt::onLegendaryToggle(CCObject *sender)
{
    extern bool legendaryChanged;
    legendaryChanged = true;
    bool selected = GLM->getBoolForKey("legendary_filter_custom") ^ 1;
    GLM->setBoolForKey(selected, "legendary_filter_custom");
}
