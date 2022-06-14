#include "cocos2d.h"


using namespace cocos2d;
class MoreSearchLayer : public FLAlertLayer
{

public:

    void createToggleButton(std::string,void (cocos2d::CCObject::*)(cocos2d::CCObject*),bool,cocos2d::CCMenu *,cocos2d::CCPoint);

};