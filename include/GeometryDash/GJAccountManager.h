#pragma once

#include <cocos2dx/CCNode.h>
#include <string>

#include "GDAPI_Macros.h"

class GJAccountRegisterDelegate;
class GJAccountLoginDelegate;
class GJAccountDelegate;
class GJAccountBackupDelegate;
class GJAccountSyncDelegate;
class GJAccountSettingsDelegate;

class GJAccountManager : public cocos2d::CCNode 
{
public:
    cocos2d::CCDictionary *DLDict_;                 // 0xF8
    std::string accountPassword_;                   // 0xFC
    std::string accountUserName_;                   // 0x100
    int accountIDRand_;                             // 0x104
    int accountIDSeed_;                             // 0x108
    int accountID_;                                 // 0x10C
    std::string sessionID_;                         // 0x110
    GJAccountRegisterDelegate *registerDelegate_;   // 0x114
    GJAccountLoginDelegate *loginDelegate_;         // 0x118
    GJAccountDelegate *accountDelegate_;            // 0x11C
    GJAccountBackupDelegate *backupDelegate_;       // 0x120
    GJAccountSyncDelegate *syncDelegate_;           // 0x124
    GJAccountSettingsDelegate *settingsDelegate_;   // 0x128

// fixed class members by offsets
public:
    CLASS_MEMBER(std::string, accountUserName, 0x100);
    CLASS_MEMBER(int, accountIDRand, 0x104);
    CLASS_MEMBER(int, accountIDSeed, 0x108);

    bool isLoggedIn() { return this->_accountIDRand() - this->_accountIDSeed() > 0; }

public:
    GJAccountManager();
    virtual ~GJAccountManager();

    static GJAccountManager* sharedState( );
    
};