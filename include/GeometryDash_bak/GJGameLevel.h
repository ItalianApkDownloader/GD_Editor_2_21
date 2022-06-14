#ifndef __GJGAMELEVEL_H__
#define __GJGAMELEVEL_H__

#include "GeometryDash.h"
#include "cocos2d.h"
#include <string.h>



enum GJLevelType {
	kGJLevelDefault,
	kGJMainLevel,
	kGJOnlineLevel,
	kGJLocalLevel
};

class GJGameLevel : public CCNode {
public:
    PAD(12)

    cocos2d::CCDictionary *lastBuildSave;

    int levelID_rand; // levelID + seed = levelID_rand
    int levelID_seed;
    int levelID;

    std::string levelName;
    std::string levelDesc;
    std::string levelString;
    std::string userName;
    std::string recordString;
	int dword11C;
	int dword120;
	int dword124;
	int userIDEnc_;
	int userIDRand_;
	int userID_;
	int accountIDEnc_;
	int accountIDRand_;
	int accountID_;
	int difficulty_;
	int audioTrack_;
	int songID_;
	int levelRev_;
	bool isUnlisted_;
	bool friendsOnly_;
	int objectCountEnc_;
	int objectCountRand_;
	int objectCount_;
	int levelOrder_;
	int ratings_;
	int ratingsSum_;
	int downloads_;
	bool isEditable_;
	bool isGauntlet_;
	bool isGauntlet2_;
	int wt_;
	int wt2_;
	bool lowDetailMode_;
	bool toggleLowDetail_;
	bool byte17E;
	bool byte17F;
	int isVerifiedEnc_;
	int isVerifiedRand_;
	bool isVerified_;
	bool isUploaded_;
	bool hasBeenModified_;
	int levelVersion_;
	int gameVersion_;
	int attemptsEnc_;
	int attemptsRand_;
	int attempts_;
	int jumpsEnc_;
	int jumpsRand_;
	int jumps_;
	int clicksEnc_;
	int clicksRand_;
	int clicks_;
	int attemptTimeEnc_;
	int attemptTimeRand_;
	int attemptTime_;
	int levelScoreSeed_;
	bool isChkValid_;
	bool anticheat_;
	int normPercent_;
	int normPercentRand_;
	int normPercentEnc_;
	int newNormPercentEnc_;
	int newNormPercentRand_;
	int newNormPercent_;
	int newNormPercent2Enc_;
	int newNormPercent2Rand_;
	int newNormPercent2_;
	int practicePercent_;
	int likes_;
	int dislikes_;
	int levelLength_;
	int featured_;
	int epic_;
	bool isHearted_;
	int levelFolder_;
	int dailyIDEnc_;
	int dailyIDRand_;
	int dailyID_;
	int demonEnc_;
	int demonRand_;
	int demon_;
	int demonDifficulty_;
	int starsEnc_;
	int starsRand_;
	int stars_;
	bool autoLevel_;
	int coins_;
	int coinsVerifiedEnc_;
	int coinsVerifiedRand_;
	int coinsVerified_;
	int passwordEnc_;
	int passwordRand_;
	int originalLevelEnc_;
	int originalLevelRand_;
	int originalLevel_;
	bool twoPlayerMode_;
	int failedPasswordAttempts_;
	int coin1VerifiedEnc_;
	int coin1VerifiedRand_;
	int coin1Verified_;
	int coin2VerifiedEnc_;
	int coin2VerifiedRand_;
	int coin2Verified_;
	int coin3VerifiedEnc_;
	int coin3VerifiedRand_;
	int coin3Verified_;
	int requiredStars_;
	bool showedStarWarning_;
	int starRatings_;
	int starRatingsSum_;
	int maxStarRatings_;
	int minStarRatings_;
	int demonVotes_;
	int rateStars_;
	bool rateFeature_;
	std::string rateUser_;
	bool dontSave_;
	bool isHidden_;
	int requiredCoins_;
	bool isUnlocked_;
	cocos2d::CCPoint lastCameraPos_;
	float lastEditorZoom;
	int lastBuildTab_;
	int lastBuildPage_;
	int lastBuildGroupID_;
	int levelType_;
	int M_ID;
	std::string tempName_;
	std::string capacityString_;
	bool highObjectCount_;
	bool byte2E9;
	std::string levelSeed_;
	int dword2F0;



    void setJumps(int);
	

	~GJGameLevel();

	
	static GJGameLevel *create();
	bool init();
	
	static int getLengthKey(int levelDistance);
	static std::string lengthKeyToString(int lengthKey);
	
	bool canEncode();
	std::string getSongName();
	void levelWasAltered();
	void levelWasSubmitted();
	void savePercentage(int percentage, bool practiceMode);
	void savePercentage(int,bool isPractice,int,int,bool);

	void setNormalPercent(int);
	void setNewNormalPercent(int);
	void setNewNormalPercent2(int);
	int getNormalPercent();
	void setStars(int);
	void setDemon(int);
	void setLevelID(int);
	void setAttempts(int);
	CCString* getCoinKey(int);

	std::string  getAudioFileName();
	
};

class LevelSettingsObject : public cocos2d::CCNode
{
public:
    PAD(12);
	GJEffectManager* colorManager_; // 0xF8
	int startMode_;
	int startSpeed_;
	bool miniMode_;
	bool startDualMode_;
	bool twoPlayerMode_;
	bool startPlatformerMode_;
	float songOffset_;
	bool fadeIn_;
	bool fadeOut_;
	bool gap10E[ 2 ];
	int BGIdx_;
	int GIdx_;
	int fontIdx_;
	int BG2Idx_;
	bool isStartPos_;
	bool gravityFlipped_;
	bool reversePlayer_;
	GJGameLevel* gameLevel_;
	std::string guidelines_;
	bool byte130;
	bool gap131[ 3 ];
	int lastColorPage_;
	int GLineIdx_;
	bool byte13C;
	bool isVisible_;

public:
	LevelSettingsObject();
	virtual ~LevelSettingsObject();

	static LevelSettingsObject* create( );

	virtual bool init( void );
};
#endif //__GJGAMELEVEL_H__
