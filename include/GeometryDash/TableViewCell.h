#pragma once

#include <gd.h>

	class TableViewCell : public cocos2d::CCLayer {
	public:
		PAD(9);
		PAD(4);
		std::string m_sUnknown;
		PAD(8);
		cocos2d::CCLayerColor* m_pLayerColor;
		cocos2d::CCLayer* m_pLayer;
		PAD(4);

		TableViewCell(const char* name, float height, cocos2d::ccColor4B color);
	};