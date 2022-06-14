#include "cocos2d.h"

using namespace cocos2d;

class  GManager : public CCNode {
public:
	std::string m_sFileName;
	bool m_bSetup;
	bool m_bSaved;

public:

		std::string getSaveString();
		void save();
		void saveGMTo(std::string path);
		void saveData(DS_Dictionary *,std::string path);

		virtual void setup();
        virtual void encodeDataTo(DS_Dictionary* data){}
        virtual void dataLoaded(DS_Dictionary* data){}
        virtual void firstLoad(){}
};