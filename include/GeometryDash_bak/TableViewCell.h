#include "cocos2d.h"

using namespace cocos2d;

class TableViewCell : public CCLayer{
public:


    TableViewCell( const char *a2, float a3, float a4);

    void updateVisibility();
};