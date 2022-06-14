#include "cocos2d.h"





class  CustomListView : public BoomListView{
	public:
        static CustomListView*  create(cocos2d::CCArray*, float, float, int, BoomListType);

        void reloadAll();

		virtual int setupList(void);
		virtual CCLayer * getListCell(char const*);
		virtual void loadCell(TableViewCell *,int);


};