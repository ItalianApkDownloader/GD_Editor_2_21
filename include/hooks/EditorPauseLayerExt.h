

class EditorPauseLayerExt : public EditorPauseLayer {
public:

	
public:

	static inline void (*UnlockAllLayersO)(EditorPauseLayer* self, CCObject* a2);
	void UnlockAllLayersH(CCObject* a2);
	
	
	static inline void (*onResumeO)(EditorPauseLayer* self,CCObject* a1);
	void onResumeH(CCObject* a1);
	
	
	static inline void* (*customSetupO)(EditorPauseLayer* self);
	void* customSetupH();
	
	void onDisableShaders(CCObject* sender);

	static void ApplyHooks();

};