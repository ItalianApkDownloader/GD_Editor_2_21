#include "cocos2d.h"


enum LikeItemType{

};

class  CommentCell : public TableViewCell{
	public:

		CommentCell(char const*, float, float);

		virtual bool init(void);
		virtual void draw(void);
		virtual int likedItem(LikeItemType,int,bool);
		virtual int FLAlert_Clicked(FLAlertLayer *,bool);
};