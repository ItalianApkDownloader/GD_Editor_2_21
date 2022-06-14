#include "cocos2d.h"


class TableView;

enum TableViewCellEditingStyle{

};


enum BoomListType{

};


class  BoomListView : public cocos2d::CCLayer{
	public:
		int aa;
		int a2;
		TableView * tableView;


		bool init(cocos2d::CCArray *,float,float,int,BoomListType);


		static BoomListView*  create(cocos2d::CCArray*, float, float, int, BoomListType);



		/*

		virtual void draw(void);
		virtual int setupList(void);
		virtual void TableViewWillDisplayCellForRowAtIndexPath(void* ,CCLayer * tableViewCell ,CCLayer * tableView );
		virtual int cellHeightForRowAtIndexPath(void* ,CCLayer * tableView );
		virtual bool didSelectRowAtIndexPath(void* , CCLayer *tableView);
		virtual int numberOfRowsInSection(uint,CCLayer * tableView);
		virtual signed int numberOfSectionsInTableView(CCLayer * tableView);
		virtual int cellForRowAtIndexPath(void* ,CCLayer * tableView);
		virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(CCLayer * tableView,TableViewCellEditingStyle,void* );
		virtual void TableViewWillReloadCellForRowAtIndexPath(void* ,CCLayer * tableViewCell ,CCLayer * tableView);
		virtual cocos2d::CCLayer * getListCell(char const*);
		virtual CCLayer* loadCell(CCLayer * tableViewCell,int);
		 */

};