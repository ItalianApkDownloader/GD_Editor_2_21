

class LevelEditorLayerExt : public LevelEditorLayer{
public:
    static inline  bool (*initO)(LevelEditorLayer *,GJGameLevel *);
    bool initH(GJGameLevel *);
	
	static inline void (*updateVisibilityO)(LevelEditorLayer*,float);
	void updateVisibilityH(float dt);
	
	static inline void (*removeO)(LevelEditorLayer* self,GameObject *,bool);
	void removeH(GameObject * obj,bool a1);
	
	static void ApplyHooks();

};