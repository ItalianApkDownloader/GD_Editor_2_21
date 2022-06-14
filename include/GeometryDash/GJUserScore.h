#ifndef __GJUSERSCORE_H__
#define __GJUSERSCORE_H__

#include <gd.h>

class GJUserScore : public cocos2d::CCNode
{
public:
    std::string userName_;
    std::string userUDID_;
    int scoreType_; // legacy field, used in 1.9 to determine shown info
    int userID_;
    int accountID_;

    int stars_;
    int diamonds_;
    int demons_;
    int playerRank_;
    int creatorPoints_; // note - internally this is named levels
    int secretCoins_;
    int userCoins_;

    int iconID_; // for leaderboards, icon id to show
    int color1_;
    int color2_;
    int special_;

    int messageState_;
    int friendStatus_;
    int commentHistoryStatus_;

    std::string youtubeURL_;
    std::string twitterURL_;
    std::string twitchURL_;

    int idk; //idk
    int playerCube_; //cube
    int playerShip_; //ship
    int playerBall_; //ball
    int playerUfo_; //ufo
    int playerWave_; //wave
    int playerRobot_; //robot
    int playerSpider_; //spider
    bool glowEnabled_ : 4;

    int playerExplosion_; //idk

    int idk2;
    int modBadge_; //0x17C

    int globalRank_; //global rank
    int newMsgCount_; //idk
    int friendReqCount_; //idk2
    bool isBlocked : 4;

    std::string lastScoreAge;

// fixed members
public:
    //CLASS_MEMBER(int, modBadgeTexture, 0x17C); //the texture that gets created, not actually the mod badge 
};

class GJUserScoreExt : public GJUserScore {
public:
    int playerSwing_;
};

#endif