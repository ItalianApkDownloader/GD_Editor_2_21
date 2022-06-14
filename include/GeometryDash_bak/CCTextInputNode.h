#include <string>

using namespace cocos2d;

class CCTextInputNode : public CCLayer {
public:
	~CCTextInputNode();
	
	static CCTextInputNode *create(float width, float height, const char *placeholder, const char *labelPlaceholderFont, int labelPlaceholderFontSize, const char *labelNormalFont);
	const char * getString();

	void setMaxLabelScale(float );
	void setLabelPlaceholderColor( ccColor3B );
	void setString( std::string );

};

