#pragma once

#include "CCLayer.h"
#include "GJDropDownLayer.h"

class EndLevelLayer : public GJDropDownLayer
{
public:
    EndLevelLayer();
    virtual ~EndLevelLayer();

    void goEditFix();
};
