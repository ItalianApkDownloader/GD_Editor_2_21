#include "cocos2d.h"

using namespace cocos2d;

class ListCell: public TableViewCell
{
public:
    ListCell( const char *a2, float a3, float a4);



    ListCell* loadFromObject(cocos2d::CCObject*, int, int, int);

    virtual bool init(void);
    virtual void draw(void);

};