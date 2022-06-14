#include "gd.h"
class PlayLayerExt : public PlayLayer{

public:

    static inline void (*update_trp)(PlayLayer*,float);
    static inline void (*resetLevel_trp)(PlayLayer*);
    static inline void (*destroyPlayer_trp)(PlayerObject*, int);
    static inline void (*levelCompleted_trp)(PlayLayer*);
    static inline void (*togglePracticeMode_trp)(PlayLayer*, bool);
    static inline void (*resume_trp)(PlayLayer*);

	std::string formatSecond(float value);
    void update_hk(float a1);
    void resetLevel_hk();
    void resume_hk();
    void togglePracticeMode_hk(bool);
	void levelCompleted_hk();
    void destroyPlayer_hk(PlayerObject*, int a2);
	void onUpdateTimer(float dt);
};