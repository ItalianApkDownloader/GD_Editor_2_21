
class  AccountRegisterLayerExt : public CCLayerColor {
public:

   static inline bool (*init_trp)(AccountRegisterLayerExt *);
   void onMenu()
	       {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.0f, MenuLayer::scene(false)));
    }
   bool init_hk();
	
	    

};
