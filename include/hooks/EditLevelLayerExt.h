
#include <gd.h>
#include "cocos2d.h"
#include "EditLevelLayer.h"

class EditLevelLayerExt : public EditLevelLayer {
	
    public:

        static inline bool (*initO)(EditLevelLayer*, GJGameLevel* level);
        bool initH(GJGameLevel* level);
		
		void onClick(CCObject* sender);
		
		static void ApplyHooks();

};