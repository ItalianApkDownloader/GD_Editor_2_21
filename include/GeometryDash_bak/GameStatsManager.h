#include "cocos2d.h"

using namespace cocos2d;


class GJRewardItem;
class GJGameLevel;


enum GJRewardType{

};

class GameStatsManager : public CCNode
{
	public:
		static GameStatsManager * sharedState();
		GameStatsManager * setStat(const char *,int);
		int getBaseCurrency(int,bool,int);
		int checkAchievement(const char*);

		void collectReward(GJRewardType,GJRewardItem *);


		
		inline CCDictionary* getDict(){ return value_0;};
		
		
			CCDictionary *value_0;
			CCDictionary *value_1;
			CCDictionary *value_2;
			CCDictionary *value_3;
			CCDictionary *value_4;
			CCDictionary *value_5;
			CCDictionary *value_6;
			CCDictionary *value_7;
			CCDictionary *value_8;
			CCDictionary *value_9;
			CCDictionary *value_10;
			CCDictionary *value_11;
			CCDictionary *value_12;
			CCDictionary *value_13;
			CCDictionary *value_14;
			CCDictionary *value_15;
			CCDictionary *value_16;
			CCDictionary *value_17;
			CCDictionary *value_18;
			CCDictionary *value_19;
			CCDictionary *value_20;
			CCDictionary *value_21;
			CCDictionary *value_22;
			CCDictionary *value_23;
			CCDictionary *value_24;
			CCDictionary *value_25;
			CCDictionary *value_26;
			CCDictionary *value_27;
			CCDictionary *value_28;
			CCDictionary *value_29;
			CCDictionary *value_30;
			CCDictionary *value_31;
			CCDictionary *value_32;
			CCDictionary *value_33;
			CCDictionary *value_34;
			CCDictionary *value_35;
			CCDictionary *value_36;
			CCDictionary *value_37;
			CCDictionary *value_38;
			CCDictionary *value_39;
			CCDictionary *value_40;
			CCDictionary *value_41;
			CCDictionary *value_42;
			CCDictionary *value_43;
			CCDictionary *value_44;
			CCDictionary *value_45;
			CCDictionary *value_46;
			CCDictionary *value_47;
			CCDictionary *value_48;
			CCDictionary *value_49;
			CCDictionary *value_50;
			CCDictionary *value_51;
			CCDictionary *value_52;
			CCDictionary *value_53;
			CCDictionary *value_54;
			CCDictionary *value_55;
			CCDictionary *value_56;
			CCDictionary *value_57;
			CCDictionary *value_58;
			CCDictionary *value_59;
			CCDictionary *value_60;
			CCDictionary *value_61;
			CCDictionary *value_62;
			CCDictionary *value_63;
			CCDictionary *value_64;


    int getCollectedCoinsForLevel(GJGameLevel *pLevel);
    void stopBackgroundMusic( );
};