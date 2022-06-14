
class  GJGarageLayerExt : public GJGarageLayer {
public:
    void onPalette(CCObject * sender);
	
    static inline  bool (*init_trp)(GJGarageLayer *);
    bool init_hk();
};
