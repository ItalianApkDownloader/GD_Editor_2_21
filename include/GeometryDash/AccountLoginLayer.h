#pragma once
#include "cocos2d.h"
#include "CCTextInputNode.h"

enum AccountError {
	usernameShort = 0xFFFFFFF7,
	passwordShort = 0xFFFFFFF8,
	loginFailed = 0
};

class AccountLoginLayer : public cocos2d::CCLayerColor
{
public:

public:
    AccountLoginLayer();
    virtual ~AccountLoginLayer();
	
	
	CLASS_MEMBER(CCTextInputNode*, inputPassword, 0x1FC);
	CLASS_MEMBER(CCTextInputNode*, inputUser, 0x1F8);
	
	void resetLabels();
	void disableNodes();
	void updateLabel(AccountError);
	void showLoadingUI();


};
