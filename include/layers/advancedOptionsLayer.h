
#include "cocos2d.h"
#include "gd.h"

class advancedOptionsLayer : public CCLayerColor
{
public:
	void show();

	bool init();

	static advancedOptionsLayer *create();

	virtual void keyBackClicked();
	void close();
};