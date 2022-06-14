#ifndef __GJLISTLAYER_H__
#define __GJLISTLAYER_H__

#include "GeometryDash.h"
#include "cocos2d.h"


class BoomListView;

using namespace cocos2d;

class GD_DLL GJListLayer : public CCLayerColor {
public:
	~GJListLayer();
		static GJListLayer * create(BoomListView * list,char const* text,cocos2d::_ccColor4B color ,float width,float height);


};

#endif //__GJLISTLAYER_H__
