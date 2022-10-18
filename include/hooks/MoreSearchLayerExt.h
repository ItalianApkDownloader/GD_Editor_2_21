
class  MoreSearchLayerExt : public MoreSearchLayer {
public:



    void onPlatformToggle(CCObject *sender);
    void onLegendaryToggle(CCObject *sender);
    void reloadFilters();

    static inline bool (*init_trp)(MoreSearchLayer * self);
    bool init_hk();
    static void ApplyHooks();
    


    void addLayerToggle(int row, int column, string name, SEL_MenuHandler, bool disabled);
};
