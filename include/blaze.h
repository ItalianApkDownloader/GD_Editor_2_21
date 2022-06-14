#include <gd.h>

void lib_entry(void);


class gdmk{
public:
    void get_proc_addr(const char *a2);
};

extern void (*old)(EditLevelLayer *,cocos2d::CCObject *);
void onEdit_hk(EditLevelLayer *,cocos2d::CCObject *);


extern char const* (*loading)(cocos2d::CCLayer *);
char const* loading_hk(cocos2d::CCLayer *);

extern GameObject * (*old4)(int);
extern bool (*old3)(LevelEditorLayer *,GJGameLevel *);



extern bool (*menu)(cocos2d::CCLayer *);
bool menu_hk(cocos2d::CCLayer *);
extern bool (*cctouch)(UILayer *,cocos2d::CCTouch *,cocos2d::CCEvent *);
extern bool (*touchend)(UILayer *,cocos2d::CCTouch *,cocos2d::CCEvent *);
extern ObjectToolbox * (*toolbox)(void);
extern void (*clippingRect)(int,int,int,int);
//void cctouch_hk(UILayer *,cocos2d::CCTouch *,cocos2d::CCEvent *);
//void touchend_hk(UILayer *,cocos2d::CCTouch *,cocos2d::CCEvent *);
void exitEdit_hk(PauseLayer *);
bool init_hk(LevelEditorLayer *,GJGameLevel *);
//void create_hk(int);
extern bool (*unlocked)(void *,int,int);
//void unlocked_hk(void *,int,int);

extern void (*pause1)(PauseLayer *);

extern void (*dict)(cocos2d::CCDictionary *,cocos2d::CCObject *,int);
extern void (*exitEdit)(PauseLayer *);
void dict_hk(cocos2d::CCDictionary *,cocos2d::CCObject *,int);
void pause_hk(PauseLayer *);
//extern bool (*levelinfoinit)(LevelInfoLayer *,GJGameLevel *,bool);
//void levelinfoinit_hk(LevelInfoLayer *,GJGameLevel *,bool);

//extern bool (*world)(CreatorLayer *);
//bool world_hk(CreatorLayer *);

extern void (*updateOptions)(EditorPauseLayer *,cocos2d::CCObject *);
void updateOptions_hk(EditorPauseLayer *,cocos2d::CCObject *);
void toolbox_hk(void);
void buttonBar_hk(EditorUI *,int,cocos2d::CCArray *);
void clippingRect_hk(int, int, int, int);






/*
 * HOOKS
2021-11-10 22:36:41.077 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN16GameStatsManager14isItemUnlockedE10UnlockTypei
2021-11-10 22:36:41.083 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN16EditorPauseLayer8onResumeEPN7cocos2d8CCObjectE
2021-11-10 22:36:41.087 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN12LoadingLayer16getLoadingStringEv
2021-11-10 22:36:41.089 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN14EditLevelLayer6onEditEPN7cocos2d8CCObjectE
2021-11-10 22:36:41.094 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN16LevelEditorLayer4initEP11GJGameLevel
2021-11-10 22:36:41.097 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN10GameObject13createWithKeyEi
2021-11-10 22:36:41.100 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN7cocos2d18CCSpriteFrameCache17spriteFrameByNameEPKc
2021-11-10 22:36:41.105 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN10PauseLayer11customSetupEv
2021-11-10 22:36:41.113 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN14LevelInfoLayer4initEP11GJGameLevelb
2021-11-10 22:36:41.119 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN7cocos2d12CCDictionary9setObjectEPNS_8CCObjectEi
2021-11-10 22:36:41.126 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN12CreatorLayer4initEv
2021-11-10 22:36:41.130 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN10PauseLayer6onEditEPN7cocos2d8CCObjectE
2021-11-10 22:36:41.136 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN9MenuLayer4initEv
2021-11-10 22:36:41.142 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN7UILayer12ccTouchBeganEPN7cocos2d7CCTouchEPNS0_7CCEventE
2021-11-10 22:36:41.148 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN7UILayer12ccTouchEndedEPN7cocos2d7CCTouchEPNS0_7CCEventE
2021-11-10 22:36:41.155 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: _ZN13ObjectToolbox11sharedStateEv
2021-11-10 22:36:41.163 7780-7800/com.gdpsedi.geometryjumplite D/GDPS: glScissor
 */