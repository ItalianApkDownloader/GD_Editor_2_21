#include "GeometryDash.h"
#include "cocos2d.h"




class GD_DLL LevelSelectLayer : public CCLayer {
	public:
		bool init();
		static CCScene * scene(int);
		static LevelSelectLayer * create(int);
		
};
