
class  MultiplayerLayerExt : public MultiplayerLayer {
public:
    static inline  bool (*init_trp)(LevelEditorLayer *,GJGameLevel *);
    bool init_hk(GJGameLevel *);
};
