#include <gd.h>

using namespace cocos2d;


class MoreOptionsLayer : public FLAlertLayer
{
    public:
		/*
        textInputReturn(CCTextInputNode *);
        textInputShouldOffset(CCTextInputNode *,float);
        keyBackClicked(void);
        ~MoreOptionsLayer();
        ~MoreOptionsLayer();
        create(void);
        pageKey(int);
        countForPage(int);
        nextPosition(int);
        incrementCountForPage(int);
        objectKey(int);
        objectsForPage(int);
        layerKey(int);
        layerForPage(int);
        infoKey(int);
		goToPage(int);
        init(void);
        toggleGP(void);
		*/
		static MoreOptionsLayer* create();
        void addToggle(char const*,char const*,char const*){};
		virtual bool init(void){};
		virtual void keyBackClicked(){};
		virtual void textInputShouldOffset(CCTextInputNode *,float){};
		virtual void textInputReturn(CCTextInputNode *){};
		virtual void googlePlaySignedIn(void){};

};