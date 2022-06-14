

class GDPSSettings : public CCLayerColor{
public:




    void show();

    bool init();

    static GDPSSettings* create();

    virtual void keyBackClicked();
    void close();

    virtual bool ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
    virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
    virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);


    virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *){};
    virtual void ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*){};
    virtual void ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*){};
    virtual void ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*){};
    virtual void ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*){};
    virtual void didAccelerate(cocos2d::CCAcceleration*){};
    virtual void keyMenuClicked(){};
    virtual void  keyDown(cocos2d::enumKeyCodes){};
    virtual void  setColor(cocos2d::_ccColor3B const&){};
    virtual const cocos2d::ccColor3B &  getColor(){};
    virtual const cocos2d::ccColor3B &  getDisplayedColor(){};
    virtual unsigned char getDisplayedOpacity(){};
    virtual unsigned char  getOpacity(){};
    virtual void  setOpacity(unsigned char){};
    virtual bool  isCascadeColorEnabled(){};
    virtual void  setCascadeColorEnabled(bool){};
    virtual void  updateDisplayedColor(cocos2d::_ccColor3B const&){};
    virtual bool  isCascadeOpacityEnabled(){};
    virtual void  setCascadeOpacityEnabled(bool){};
    virtual void  updateDisplayedOpacity(unsigned char){};
    virtual void  setBlendFunc(cocos2d::_ccBlendFunc){};
    virtual cocos2d::_ccBlendFunc  getBlendFunc(){};

};