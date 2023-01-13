#include "DPADControl.h"
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"
#include "GDPSManager.h"

using namespace cocos2d;

#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

// vars
int touch1ID;
int touch2ID;
CCRect leftBtnRect;
CCRect leftBtnRect2;
CCRect rightBtnRect;
CCRect rightBtnRect2;
bool thisShitDoesSomething = false;
bool whatDoesThisDoTho = false;

void DPADHooks::UILayerInit(UILayer* self) {
    auto dir = CCDirector::sharedDirector();

    rightBtnRect = {dir->getScreenRight() - 240, 0, 240, 100};

    touch1ID = -1;
    touch2ID = -1;

    thisShitDoesSomething = false;
    whatDoesThisDoTho = false;
	
	auto dpad = MEMBERBYOFFSET(CCSprite*, self, 0x1D8);
	dpad->setVisible(true);
	
    if(self->isTwoPlayer()) {
        thisShitDoesSomething = false;
        whatDoesThisDoTho = false;
		
        auto dpadRight = CCSprite::createWithSpriteFrameName("Dpad_Btn.png");
		dpadRight->setPositionY(dpad->GPY());
		dpadRight->setPositionX(CCRIGHT - (dpad->GPX() - CCLEFT));
		dpadRight->setTag(0xBAE);
		self->addChild(dpadRight);

        rightBtnRect2 = {dpadRight->getPositionX() - 5, MBO(float, self, 0x1E4), 10, MBO(float, self, 0x1EC)};
    }
}

void DPADHooks::EditorUIInit(EditorUI* self) {
    touch1ID = -1;
    touch2ID = -1;

    thisShitDoesSomething = false;
    whatDoesThisDoTho = false;

    auto dir = CCDirector::sharedDirector();

    leftBtnRect = {0, 0, 240, 100};
    rightBtnRect = {dir->getScreenRight() - 240, 0, 240, 100};
}

FUNCTIONHOOK(bool, UILayer_ccTouchBegan, UILayer* self, CCTouch *touch, CCEvent *event) {
    auto ret = UILayer_ccTouchBeganO(self, touch, event);
    

    if(MBO(bool, self, 0x206)) {
        auto gm = GameManager::sharedState();
        auto dir = CCDirector::sharedDirector();
        auto winSize = dir->getWinSize();

        auto touchPos = touch->getLocation();

        if(self->isTwoPlayer()) {
            auto dpad2 = (CCSprite*)self->getChildByTag(0xBAE);
            if(dpad2 == nullptr) return false;

            if(rightBtnRect.containsPoint(touchPos)) {
                touch1ID = MBO(int, touch, 0x30);

                if(rightBtnRect2.containsPoint(touchPos)) {
                    thisShitDoesSomething = true;
                    return true;
                }

                // since virtuals are misaligned im calling the function like this
                auto setDisplayFrame = *(void (__fastcall **)(CCSprite*, CCSpriteFrame*))(*(int *)dpad2 + 572);
                setDisplayFrame(dpad2, CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Dpad_Btn_Dwn.png"));
                
                int btn = 0;
                if(touchPos.x < dpad2->getPosition().x) {
                    btn = 2;
                    dpad2->setFlipX(false);
                }
                else {
                    btn = 3;
                    dpad2->setFlipX(true);
                }
                gm->_playLayer()->queueButton(btn, true, true);
            }
        }

        // platformer jump

        auto dpad1rect = MBO(CCRect, self, 0x1E0);
        if(!dpad1rect.containsPoint(touchPos) && !rightBtnRect.containsPoint(touchPos)) {
            // dual mode
            if(MBO(bool, gm->_playLayer(), 0x4C1)) {
                // now this shit i dont understand, why do people wanna have jump on opposite sides
                bool p2jump = false;

                if(!gm->getGameVariable("100011")) p2jump = touchPos.x < winSize.width / 2;
                else p2jump = touchPos.x > winSize.width / 2;

                if(p2jump) {

                    if(MBO(int, self, 0x1D0) != -1) {
                        goto CONTINUE;
                    }
                    MBO(int, self, 0x1D0) = MBO(int, touch, 0x30);
                    gm->_playLayer()->queueButton(1, true, true);
                    return true;
                }
            }
        CONTINUE:
            if(MBO(int, self, 0x1CC) == -1) {
                MBO(int, self, 0x1CC) = MBO(int, touch, 0x30);
                gm->_playLayer()->queueButton(1, true, false);
                return true;
            }
        }
        
        if(!self->isTwoPlayer() && !dpad1rect.containsPoint(touchPos))
        {
            MBO(int, self, 0x1CC) = MBO(int, touch, 0x30);
            gm->_playLayer()->queueButton(1, true, false);
        }
       
        if(self->isTwoPlayer() && rightBtnRect.containsPoint(touchPos) && !MBO(bool, gm->_playLayer(), 0x4C1))
        {
            MBO(int, self, 0x1CC) = MBO(int, touch, 0x30);
            gm->_playLayer()->queueButton(1, true, false);
        }
    }

    return ret;
}

FUNCTIONHOOK(void, UILayer_ccTouchEnded, UILayer* self, CCTouch *touch, CCEvent *event) {
    if(MBO(bool, self, 0x206)) {
        auto gm = GameManager::sharedState();

        if (self->isTwoPlayer()) {
            if(touch1ID == MBO(int, touch, 0x30)) {
                touch1ID = -1;
                
                auto dpad2 = (CCSprite*)self->getChildByTag(0xBAE);

                // since virtuals are misaligned im calling the function like this
                auto setDisplayFrame = *(void (__fastcall **)(CCSprite*, CCSpriteFrame*))(*(int *)dpad2 + 572);
                setDisplayFrame(dpad2, CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Dpad_Btn.png"));
                gm->_playLayer()->queueButton(5, false, true);
            }
        }

        if(MBO(int, touch, 0x30) == MBO(int, self, 0x1D0)) {
            MBO(int, self, 0x1D0) = -1;
            gm->_playLayer()->queueButton(1, false, true);
            return;
        }

        if(MBO(int, touch, 0x30) == MBO(int, self, 0x1CC)) {
            MBO(int, self, 0x1CC) = -1;
            gm->_playLayer()->queueButton(1, false, false);
        }
    }
    UILayer_ccTouchEndedO(self, touch, event);
}

FUNCTIONHOOK(void, UILayer_ccTouchMoved, UILayer* self, CCTouch *touch, CCEvent *event) {
    UILayer_ccTouchMovedO(self, touch, event);

    if(MBO(bool, self, 0x206)) {
        if(self->isTwoPlayer()) {
            auto touchPos = touch->getLocation();

            auto dpad2 = (CCSprite*)self->getChildByTag(0xBAE);
            if(dpad2 == nullptr) return;

            if(touch1ID == MBO(int, touch, 0x30)) {
                if(touchPos.x < dpad2->getPositionX()) {
                    if(!whatDoesThisDoTho || thisShitDoesSomething) {
                        whatDoesThisDoTho = true;
                        dpad2->setFlipX(false);
                        thisShitDoesSomething = false;
                        GameManager::sharedState()->_playLayer()->queueButton(2, true, true);
                    }
                }
                else if (whatDoesThisDoTho || thisShitDoesSomething) {
                    whatDoesThisDoTho = false;
                    dpad2->setFlipX(true);
                    thisShitDoesSomething = false;
                    GameManager::sharedState()->_playLayer()->queueButton(3, true, true);
                }
            }
        }
    }
}

// playtest
FUNCTIONHOOK(void, EditorUI_playerTouchBegan, EditorUI *self, CCTouch *touch, CCEvent *event) {
    auto touchPos = touch->getLocation();
    
    auto dir = CCDirector::sharedDirector();
    auto winSize = dir->getWinSize();

    auto gm = GameManager::sharedState();

    if(MBO(bool, MBO(void*, self->_levelEditor(), 0x33C), 0x113)) { // isPlatformer
        if(leftBtnRect.containsPoint(touchPos)) {
            touch1ID = MBO(int, touch, 0x30);

            if(touchPos.x < leftBtnRect.size.width / 2) {
                self->_levelEditor()->queueButton(2, true, false);
            }
            else {
                self->_levelEditor()->queueButton(3, true, false);
            }
            return;
        }
        else if(rightBtnRect.containsPoint(touchPos) && MBO(bool, gm->_editorLayer(), 0x4C1) && gm->_editorLayer()->_lvlSettingsObject()->_isTwoPlayer()) { // dual and 2p enabled
            touch2ID = MBO(int, touch, 0x30);

            if(winSize.width - touchPos.x < rightBtnRect.size.width / 2) {
                self->_levelEditor()->queueButton(3, true, true);
            }
            else {
                self->_levelEditor()->queueButton(2, true, true);
            }
            return;
        }
        else goto PLAYERJUMP;
    }

    /*
        So here I am recreating the player jump function
        Now, you might be asking, why the fuck would you do that?
        Because apparently patching anything in this function is hard
        Its easier to just straight up recreate it
    */
PLAYERJUMP:
    bool p2jump = false;

    // swap jumps
    if(!gm->getGameVariable("100011")) p2jump = touchPos.x < winSize.width / 2;
    else p2jump = touchPos.x > winSize.width / 2;

    if((MBO(bool, self->_levelEditor(), 0x4C1) || gm->getGameVariable("0011")) && p2jump) {
        int &touchId = MBO(int, self, 0x188);
        if(touchId != -1) return;
        touchId = MBO(int, touch, 0x30); 
        self->_levelEditor()->queueButton(1, true, true);
    }
    else {
        int &touchId = MBO(int, self, 0x184);
        if(touchId != -1) return;
        touchId = MBO(int, touch, 0x30);
        self->_levelEditor()->queueButton(1, true, false);
    }
}

FUNCTIONHOOK(void, EditorUI_playerTouchEnded, EditorUI *self, CCTouch *touch, CCEvent *event) {
    EditorUI_playerTouchEndedO(self, touch, event);

    if(MBO(bool, MBO(void*, self->_levelEditor(), 0x33C), 0x113)) {
        if(touch1ID == MBO(int, touch, 0x30)) {
            touch1ID = -1;
            self->_levelEditor()->queueButton(5, false, false);
        }
        if(touch2ID == MBO(int, touch, 0x30)) {
            touch2ID = -1;
            self->_levelEditor()->queueButton(5, false, true);
        }
    }
}


FUNCTIONHOOK(void, PlayLayer_updateVisiblity, PlayLayer* self, float delta)
{
    PlayLayer_updateVisiblityO(self, delta);
    
    auto ui = MBO(UILayer*, self, 0x2CA0);

    if(!MBO(bool, ui, 0x206))
        return;
    
    auto dpad = MEMBERBYOFFSET(CCSprite*, ui, 0x1D8);
    if(dpad) {
        dpad->CCSprite::setOpacity(GDPS->opacityLeft);
        //CCLog("log2");
    }
    if(!ui->isTwoPlayer())
        return;
    

    auto dpad2 = (CCSprite*)ui->getChildByTag(0xBAE);
    if(dpad2) {
        dpad2->CCSprite::setOpacity(GDPS->opacityRight);
      //  CCLog("log4");
    }
}




void DPADHooks::ApplyHooks() {
    HOOK("_ZN7UILayer12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE", UILayer_ccTouchBeganH, UILayer_ccTouchBeganO);
    HOOK("_ZN7UILayer12ccTouchEndedEPN7cocos2d7CCTouchEPNS0_7CCEventE", UILayer_ccTouchEndedH, UILayer_ccTouchEndedO);
    HOOK("_ZN7UILayer12ccTouchMovedEPN7cocos2d7CCTouchEPNS0_7CCEventE", UILayer_ccTouchMovedH, UILayer_ccTouchMovedO);

	HOOK("_ZN9PlayLayer16updateVisibilityEf", PlayLayer_updateVisiblityH, PlayLayer_updateVisiblityO);
    HOOK("_ZN8EditorUI16playerTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE", EditorUI_playerTouchBeganH, EditorUI_playerTouchBeganO);
    HOOK("_ZN8EditorUI16playerTouchEndedEPN7cocos2d7CCTouchEPNS0_7CCEventE", EditorUI_playerTouchEndedH, EditorUI_playerTouchEndedO);
}