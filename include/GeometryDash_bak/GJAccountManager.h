#include "GeometryDash.h"
#include "cocos2d.h"



using namespace std;

class GD_DLL GJAccountManager : public CCNode {
	public:
		static GJAccountManager *sharedState();
		
		inline string getName(){return name;};
		
		const char* val1;
		const char* val2;
		
		
		
		
		string name;


};