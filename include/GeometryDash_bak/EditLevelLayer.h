#ifndef __EDITLEVELLAYER_H__
#define __EDITLEVELLAYER_H__

#include "GeometryDash.h"
#include "cocos2d.h"
#include "GJGameLevel.h"

USING_NS_CC;

/* Main menu for user-created levels */
class GD_DLL EditLevelLayer : public CCLayer {
public:

public:
    unsigned char _pad1[ 0x24 ];
    GJGameLevel* level; // 0x140
    cocos2d::CCArray* a1; // 0x144
    cocos2d::CCArray* a2; // 0x148
    bool inEditorLayer_; // 0x14C

public:
    EditLevelLayer();
    virtual ~EditLevelLayer();

    void closeTextInputs( );
    void verifyLevelName( );

};

#endif //__EDITLEVELLAYER_H__