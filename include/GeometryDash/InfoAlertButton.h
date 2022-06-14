#pragma once

#include "cocos2d.h"

#include "CCMenuItemSpriteExtra.h"

class InfoAlertButton : public CCMenuItemSpriteExtra {
public:
    static InfoAlertButton* create(std::string title, std::string description, float scale);
};