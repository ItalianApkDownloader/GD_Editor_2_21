#pragma once

#include "DrawGridLayer.h"
#include "GJGroundLayer.h"
#include "UndoObject.h"
#include "obb2d.h"
#include "EditorUI.h"
#include "GJBaseGameLayer.h"
#include "LevelSettingsDelegate.h"


class EditorUI;

// size 0x5E8
class LevelEditorLayer : public GJBaseGameLayer, LevelSettingsDelegate
{
public:
    bool unk_bool_01;
    bool unk_bool_02;
    bool unk_bool_03;
    bool unk_bool_04;
    bool unk_bool_05;
    bool unk_bool_06;
    bool unk_bool_07;
    bool unk_bool_08;
    bool unk_bool_09;
    bool unk_bool_10;
    bool unk_bool_11;
    bool unk_bool_12;
    bool unk_bool_13;
    CCArray* arr_2791;   //2791
    CCArray* arr_2792;   //2792
    CCArray* arr_2793;   //2793
    CCArray* arr_2794;   //2794
    CCArray* arr_2795;   //2795
    CCArray* arr_2796;   //2796
    CCArray* arr_2797;   //2797
    CCArray* arr_2798;   //2798
    CCArray* arr_2799;   //2799
    CCArray* arr_2800;   //2800
    CCArray* arr_2801;   //2801
    CCArray* arr_2802;   //2802
    CCArray* arr_2803;   //2803
    PAD(4)
    CCArray* arr_2805;   //2805
    PAD(4);
    CCArray* arr_2807;
    PAD(32)
    CCArray* arr_2816;   //2816
    PAD(4)
    CCArray* arr_2818;   //2818
    PAD(8)
    CCArray* arr_2821;
    PAD(12)
    OBB2D* obb2d2_;      //2825
    PAD(4)
    cocos2d::CCSprite* dCross_;
    PAD(12)
    EditorUI* editorUI_;
    CCArray* arr_2832;   //2832
    CCArray* arr_2833;   //2833
    PAD(12)
    DrawGridLayer* gridLayer_;  //2837
    int* weirdAlloc;            //2838
    PAD(4)
    string * levelString_;  //2840
    PAD(4)
    vector<GameObject*> objectsVector_;
    PAD(12)
    CCArray* arr_2848;   //2848
    PAD(16)
    vector<int> vector_2853;
    PAD(8)
    vector<bool> vector_2858;   //2858
    CCArray* arr_2863;   //2863
    PAD(16)
    vector<bool> vector_2868;   //2868
    vector<bool> vector_2873;   //2873
    std::vector<unsigned char> vector_2878;    //2878
    vector<float> vector_2881;   //2881


    CLASS_MEMBER(bool, isPreviewMode, 0x2C5E);
	CLASS_MEMBER(CCArray*, crashArray1, 0x2D18);
    CLASS_MEMBER(CCDictionary*, stickyGroupsDict, 0x45C);

    CLASS_MEMBER(CCArray*, objectsToUpdate, 0x2BD4);

public:
    LevelEditorLayer();
    virtual ~LevelEditorLayer();

    static LevelEditorLayer* create( GJGameLevel* level ,bool);
    static cocos2d::CCScene* scene( GJGameLevel* level );
    
    void updateOptions( );
    void setObjectCount( int count );

    void createObjectsFromSetup( std::string setup );
    void resetToggledGroups( );

    void updateEditorMode( );


    void sortBatchnodeChildren(float);

    void createBackground2( );



    void updateVisibility( float a1 );
    void updateGround( float a1 );

    bool shouldBlend( int id );
    void updateObjectColors(CCArray*);



    void addSpecial( GameObject* gameObj );
    void addToUndoList( UndoObject* undoObj, bool add );
    void updateEditor(float);
	int getNextFreeGroupID(cocos2d::CCArray *a1);
    
    virtual void update( float );
    virtual void draw( void );
    virtual void updateColor( cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccColor3B, int, bool, int, GameObject* );
    virtual void flipGravity( PlayerObject*, bool, bool );
    virtual void calculateColorValues( GameObject*, GameObject*, int, float, ColorActionSprite*, GJEffectManager* );
    virtual void addToGroup( GameObject*, int, bool );
    virtual void timeForXPos( float, int, bool );
    virtual float xPosForTime( float );
    virtual void updateStaticCameraPosToGroup( int, bool, bool, float, int, float );
    virtual void updateStaticCameraPos( cocos2d::CCPoint, bool, bool, float, int, float );
    virtual void exitStaticCamera( bool, bool, float, int, float );
    virtual bool playerReversedDirection( GameObject* );
    virtual cocos2d::CCParticleSystemQuad* createCustomParticle( std::string );
    virtual cocos2d::CCParticleSystemQuad* claimCustomParticle( std::string, int, int );
    virtual void unclaimCustomParticle( std::string, cocos2d::CCParticleSystemQuad* );
    virtual void levelSettingsUpdated( void );
};