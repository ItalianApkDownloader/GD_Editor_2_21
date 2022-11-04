#pragma once

#include "CCLayer.h"
#include "CCScene.h"

#include "FLAlertLayer.h"
#include "CCMenuItemSpriteExtra.h"
#include "CCTextInputNode.h"

USING_NS_CC;

class CCTextInputNode;

class LevelSearchLayer : public CCLayer, public FLAlertLayerProtocol // TextInputProtocol, DemonFilterDelegate
{
public:
	CCTextInputNode* inputNode_; // 0x134 TODO: Implement CCTextInputNode
	CCSprite* starFilter_; // 0x138
	CCArray* diffFilters_; // 0x13C
	CCArray* timeFilters_; // 0x140
	CCDictionary* searchDict_; // 0x144
	CCSprite* demonFilter_; // 0x148
	CCMenuItemSpriteExtra* specialDemonBtn_; // 0x14C
	CLASS_MEMBER(CCTextInputNode*, input, 0x144);

public:
	virtual ~LevelSearchLayer( );

	CCScene* scene( );
	static LevelSearchLayer* create( );

	void onClose( CCObject* ref );
	void onBack( CCObject* ref );
	void obMostDownloaded( CCObject* ref );
	void onMostLikes( CCObject* ref );
	void onTrending( CCObject* ref );
	void onMagic( CCObject* ref );
	void onMostRecent( CCObject* ref );
	void onLatestStars( CCObject* ref );
	void onFriends( CCObject* ref );
	void onFollowed( CCObject* ref );
	void onStarAward( CCObject* ref );
	void onSearch( CCObject* ref );
	void onSearchUser( CCObject* ref );
	void onMoreOptions( CCObject* ref );
	void onSpecialDemon( CCObject* ref );

	void onDemonList(CCObject*) {
		this->onClose(nullptr);
		
		void* SearchObject = CallBySymbol(void*, "libcocos2dcpp.so",
			"_ZN16LevelSearchLayer15getSearchObjectE10SearchTypeSs",void*, int, std::string)(this, 30, std::string());
			
		//lmao i love call by symbol
		
		auto scene = CallBySymbol(CCScene*, "libcocos2dcpp.so",
			"_ZN17LevelBrowserLayer5sceneEP14GJSearchObject",void*)(SearchObject);
			
			
		auto transition = cocos2d::CCTransitionFade::create(0.5, scene);
		auto dir = cocos2d::CCDirector::sharedDirector( );
		dir->pushScene( transition );
	}

	virtual bool init( );
	virtual void keyBackClicked( );

	// TODO: Implement text node and protocols
	// TODO2: Implement demon protocol

	virtual void demonFilterSelectClosed( int unknown );
	
	public: //custom
	 void onClearString(CCObject* ref) {
		 this->_input()->setString("");
	 }
};

/* v3 = (cocos2d::CCDirector *)LevelSearchLayer::onClose(this, 0);
  v4 = (cocos2d::CCDirector *)cocos2d::CCDirector::sharedDirector(v3);
  sub_62DCC0(&v11, &byte_7DC8C0, (int)v10);
  SearchObject = LevelSearchLayer::getSearchObject(this, 4, &v11);
  v6 = LevelBrowserLayer::scene((LevelBrowserLayer *)SearchObject, (GJSearchObject *)HIDWORD(SearchObject));
  v8 = (cocos2d::CCScene *)cocos2d::CCTransitionFade::create((cocos2d::CCTransitionFade *)0x3F000000, *(float *)&v6, v7);
  cocos2d::CCDirector::pushScene(v4, v8);
  return sub_62B744(&v11);
  */