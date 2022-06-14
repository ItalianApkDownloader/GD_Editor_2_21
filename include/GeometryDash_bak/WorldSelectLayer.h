
#include "GeometryDash.h"
#include "cocos2d.h"

USING_NS_CC;

/* Main menu for user-created levels */
class GD_DLL WorldSelectLayer : public CCLayer {
public:
	~WorldSelectLayer();
	static CCScene *scene(int);
};
