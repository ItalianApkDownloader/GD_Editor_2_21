#pragma once

#include "CCLayer.h"
#include "CCScene.h"

#include "FLAlertLayer.h"
#include "CCMenuItemSpriteExtra.h"
#include "CCTextInputNode.h"
#include "GJSearchObject.h"
#include "LevelBrowserLayer.h"

	enum SearchType {
		stp1 = 30
	};
	
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
	GJSearchObject* getSearchObject(SearchType, std::string);
	void toggleTimeNum(int, bool);

	void onDemonList(CCObject*) {
		this->onClose(nullptr);
		auto scene = LevelBrowserLayer::scene(this->getSearchObject(SearchType::stp1, std::string()));
		auto transition = cocos2d::CCTransitionFade::create(0.5, scene);
		auto dir = cocos2d::CCDirector::sharedDirector( );
		dir->pushScene( transition );
	}

	virtual bool init( );
	virtual void keyBackClicked( );


	virtual void demonFilterSelectClosed( int unknown );
	
	public: //custom
	 void onClearString(CCObject* ref) {
		 this->_input()->setString("");
	 }
};