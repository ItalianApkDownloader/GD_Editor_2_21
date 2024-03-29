﻿#pragma once

#include "GDAPI_Macros.h"

#include "CCNode.h"
#include "CCTextureAtlas.h"
//#include "../GDPSHelper.h"

NS_CC_BEGIN

class CCSpriteBatchNode;
class CCSpriteFrame;
class CCAnimation;
class CCRect;
class CCPoint;
class CCSize;
class CCTexture2D;

#define CCSpriteIndexNotInitialized 0xffffffff

//TODO: Add class members to CCSprite
class CC_DLL CCSprite : public CCNodeRGBA, public CCTextureProtocol
{
public:
	CCTextureAtlas *_pobTextureAtlas;
	unsigned int _uAtlasIndex;
	CCSpriteBatchNode *_pobBatchNode;
	bool _bDirty, _bRecursiveDirty, _bHasChildren, _bShouldBeHidden;
	CCAffineTransform _transformToBatch;
	ccBlendFunc _sBlendFunc;
	CCTexture2D *_pobTexture;
	CCRect _obRect;
	bool _bRectRotated;
	CCPoint _obOffsetPosition;
	CCPoint _obUnflippedOffsetPositionFromCenter;
	ccV3F_C4B_T2F_Quad _sQuad;
	bool _bOpacityModifyRGB;
	bool _bFlipX, _bFlipY;
	bool _bDontDraw;
	float _fTlVertexMod, _fTrVertexMod;
	float _fBlVertexMod, _fBrVertexMod;
//	PAD(20)

public:
	 CCSprite();
	virtual ~CCSprite();

	static CCSprite *create(const char *);
	static CCSprite *create(const char *, const CCRect &);
	static CCSprite *create();
	static CCSprite *createWithSpriteFrame(CCSpriteFrame *);
	static CCSprite *createWithSpriteFrameName(const char *);
	static CCSprite *createWithTexture(CCTexture2D *);
	static CCSprite *createWithTexture(CCTexture2D *, const CCRect &);



protected:
	void updateColor();

public:
//0090BA70

	virtual bool init();
	virtual void setVertexZ(float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
	void setFlipX(bool);
	void setFlipY(bool);
	virtual void setScale(float);
	virtual void setPosition(const CCPoint &);
	virtual void setSkewX(float);
	virtual void setSkewY(float);
	virtual void setAnchorPoint(const CCPoint &);
	virtual void setVisible(bool);
	virtual void setRotation(float);
	virtual void setRotationX(float);
	virtual void setRotationY(float);
	virtual void ignoreAnchorPointForPosition(bool);
	virtual void addChild(CCNode *);
	virtual void addChild(CCNode *, int);
	virtual void addChild(CCNode *, int, int);
	virtual void removeChild(CCNode *, bool);
	virtual void removeAllChildrenWithCleanup(bool);
	virtual void reorderChild(CCNode *, int);
	virtual void sortAllChildren();
	virtual void draw();
	virtual void updateTransform();
	virtual void setOpacity(unsigned char);
	virtual void updateDisplayedOpacity(unsigned char);
	virtual void setColor(const _ccColor3B &);
	virtual void updateDisplayedColor(const _ccColor3B &);
	virtual void setOpacityModifyRGB(bool);
	virtual bool isOpacityModifyRGB();

	virtual bool initWithTexture(CCTexture2D *);
	virtual bool initWithTexture(CCTexture2D *, const CCRect &);
	virtual bool initWithTexture(CCTexture2D *, const CCRect &, bool);
	virtual bool initWithSpriteFrame(CCSpriteFrame *);
	virtual bool initWithSpriteFrameName(const char *);
	virtual bool initWithFile(const char *);
	virtual bool initWithFile(const char *, const CCRect &);
	virtual void setTexture(CCTexture2D *);
	virtual CCTexture2D *getTexture();
	virtual void setBlendFunc(_ccBlendFunc);
	virtual _ccBlendFunc getBlendFunc();
	virtual void setChildColor(const _ccColor3B &);
	virtual void setChildOpacity(unsigned char);
	virtual CCSpriteBatchNode *getBatchNode();
	virtual void setBatchNode(CCSpriteBatchNode *);
	virtual void refreshTextureRect();
	virtual void setTextureRect(const CCRect &);
	virtual void setTextureRect(const CCRect &, bool, const CCSize &);
	virtual void setVertexRect(const CCRect &);
	virtual void setDisplayFrame(CCSpriteFrame *);
	virtual bool isFrameDisplayed(CCSpriteFrame *);
	virtual CCSpriteFrame *displayFrame();
	virtual void setDisplayFrameWithAnimationName(const char *, int);
	virtual bool isDirty();
	virtual void setDirty(bool);

	virtual void setTextureCoords(cocos2d::CCRect const&);
	virtual void updateBlendFunc(void);
	virtual void setReorderChildDirtyRecursively(void);
	virtual void setDirtyRecursively(bool);
	
public: //custom

	void setColorFix(const _ccColor3B & color);

};

class CC_DLL CCSpriteExtra : public CCSprite
{
public:
	float _fXOffset, _fYOffset;

public:
	CCSpriteExtra();
	virtual ~CCSpriteExtra();

	float getXOffset() const;
	float getYOffset() const;
	void setXOffset(float x);
	void setYOffset(float y);
};

NS_CC_END