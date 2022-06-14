

class LevelEditorLayerExt : public LevelEditorLayer{
public:
    static inline  bool (*init_trp)(LevelEditorLayer *,GJGameLevel *);
    bool init_hk(GJGameLevel *);

};