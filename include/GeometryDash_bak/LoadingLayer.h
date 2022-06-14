#include "cocos2d.h"

using namespace cocos2d;

class LoadingLayer : public cocos2d::CCLayer
{
public:
    PAD(4);
    int m_nLoadIndex;
    cocos2d::CCLabelBMFont* m_pCaption;
    PAD(4);
    //artifacts of rob debugging something
    cocos2d::CCSprite* m_pSliderBar;
    float m_fSliderGrooveXPos;
    PAD(4);
    bool m_bFromRefresh;


    void setWillFadeIn(bool willFadeIn) {
        *reinterpret_cast<bool*>(
                reinterpret_cast<char*>(this) + 0x138
        ) = willFadeIn;
    }

    void setFromRefresh(bool value) {
        m_bFromRefresh = value;
    }

};