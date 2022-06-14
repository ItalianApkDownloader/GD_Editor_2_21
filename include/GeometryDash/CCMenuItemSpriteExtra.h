#pragma once

#include "CCMenuItem.h"
#include "GDAPI_Macros.h"


class CCMenuItemSpriteExtra : public cocos2d::CCMenuItemSprite 
{
public:
    int m_unk_0;                        //0x124
    int m_unk_4;
    int m_unk_8;
    int m_unk_c;
    int m_unk_10;
    int m_unk_14;
    int m_unk_18;
    CCPoint m_itemPosition;            //0x140
    int m_unk_24;
    int m_unk_28;
    int m_unk_2c;
    int m_unk_30;
    int m_unk_34;
    int m_unk_38;
    int m_unk_3c;
    int m_unk_40;
    int m_unk_44;
    int m_unk_48;
    int m_unk_4c;
    int m_unk_50;
    int m_unk_54;
    int m_unk_58;
    int m_unk_5c;
    int m_unk_60;
    int m_unk_64;
    int m_unk_68;
    int m_unk_6c;
    int m_unk_70;
    int m_unk_74;
    int m_unk_78;
    int m_unk_7c;
    int m_unk_80;
    int m_unk_84;
    int m_unk_88;
    int m_unk_8c;
    int m_unk_90;
    int m_unk_94;
    int m_unk_98;
    int m_unk_9c;
    int m_unk_a0;
    int m_unk_a4;
    int m_unk_a8;
    int m_unk_ac;
    int m_unk_b0;
    int m_unk_b4;
    int m_unk_b8;
    int m_unk_bc;
    int m_unk_c0;
    int m_unk_c4;
    int m_unk_c8;
    int m_unk_cc;
    int m_unk_d0;
    int m_unk_d4;
    int m_unk_d8;
    int m_unk_dc;
    int m_unk_e0;
    int m_unk_e4;
    int m_unk_e8;
    int m_unk_ec;
    int m_unk_f0;
    int m_unk_f4;
    int m_unk_f8;
    int m_unk_fc;
    int m_unk_100;
    int m_unk_104;
    int m_unk_108;
    int m_unk_10c;
    int m_unk_110;
    int m_unk_114;
    int m_unk_118;
    int m_unk_11c;
    int m_unk_120;
    int m_unk_124;
    int m_unk_128;
    int m_unk_12c;
    int m_unk_130;
    int m_unk_134;
    int m_unk_138;
    int m_unk_13c;
    int m_unk_140;
    int m_unk_144;
    int m_unk_148;
    int m_unk_14c;
    int m_unk_150;
    int m_unk_154;
    int m_unk_158;

    virtual ~CCMenuItemSpriteExtra();
    
    static CCMenuItemSpriteExtra* create( cocos2d::CCNode* sprite, cocos2d::CCNode* a2, cocos2d::CCObject* parentNode, cocos2d::SEL_MenuHandler selector );

    bool init(cocos2d::CCNode *,cocos2d::CCNode *,cocos2d::CCObject *,void (cocos2d::CCObject::*)(cocos2d::CCObject *));

};