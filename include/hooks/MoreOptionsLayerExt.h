
class MoreOptionsLayerExt : public MoreOptionsLayer{
public:


    static inline void (*addToggle_trp)(MoreOptionsLayer * self,const char * title, const char * code, const char * desc);
    void addToggle_hk(const char * title, const char * code, const char * desc);

    static inline void (*onToggle_trp)(MoreOptionsLayer * self,const char * title, const char * code, const char * desc);
    void onToggle_hk(CCObject* sender);

};