#pragma once
#include <cocos2d.h>
#include <gd.h>
#include "CCNode.h"
#include "DialogObject.h"
class DialogLayer : public cocos2d::CCLayerColor
{
public:

public:
    DialogLayer();
    virtual ~DialogLayer();

	 DialogLayer* create(DialogLayer* , DialogObject*, int , int);
	 void displayDialogObject(DialogObject*);
	 DialogLayer* createDialogLayer(DialogObject*, cocos2d::CCArray *, int);
	 void onEnter();

};