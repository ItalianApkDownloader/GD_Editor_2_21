#include "cocos2d.h"



class  GameCell : public TableViewCell{
public:

    GameCell(char const*, float, float);


    int loadFromString(std::string);
};