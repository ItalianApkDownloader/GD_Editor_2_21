#include "cocos2d.h"


enum CurrencySpriteType{

};

enum CurrencyRewardType{

};


class CurrencyRewardLayer : public cocos2d::CCLayer
{
	public:
			

	    static CurrencyRewardLayer*	create(int,int,int,CurrencySpriteType,int,CurrencySpriteType,int,cocos2d::CCPoint,CurrencyRewardType,float);
};