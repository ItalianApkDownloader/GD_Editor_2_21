#include "cocos2d.h"




class  SongInfoObject : public cocos2d::CCLayer{
	public:
		static SongInfoObject* create(int);
		static SongInfoObject* create(int,std::string,std::string,int,float,std::string,std::string,std::string,int);


	virtual void encodeWithCoder(DS_Dictionary *);
		virtual bool canEncode(void);

};