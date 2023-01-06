#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"
#include "patch.h"
#include "GDPSHelper.h"
#include "FunctionHelper.h"
#include "gd.h"


namespace std {
	std::string to_string(const int& n) {
		return fmt::format("{}", n);
	}
}

std::string thousandSeparator(int n)
{
	using namespace std;
    string ans = "";
  
    // Convert the given integer
    // to equivalent string
    string num = fmt::format("{}", n);
  
    // Initialise count
    int count = 0;
  
    // Traverse the string in reverse
    for (int i = num.size() - 1;
         i >= 0; i--) {
        count++;
        ans.push_back(num[i]);
  
        // If three characters
        // are traversed
        if (count == 3) {
            ans.push_back('.');
            count = 0;
        }
    }
  
    // Reverse the string to get
    // the desired output
    reverse(ans.begin(), ans.end());
  
    // If the given string is
    // less than 1000
    if (ans.size() % 4 == 0) {
  
        // Remove ','
        ans.erase(ans.begin());
    }
  
    return ans;
}

std::string intToFormatString(int n, float& scaleMult) {
	
	
    if(n < 1000 || n > 100000000)
    return fmt::format("{}", n);

    std::string str = thousandSeparator(n);
    str = str.substr(0, str.find(".") + 2);

    char sufix;

    if(n < 1000000) { sufix = 'K'; scaleMult = 1.1f; }
    else if(n < 100000000) { sufix = 'M'; scaleMult = 1.5f; }
	
	
	return str += sufix ;
}


FUNCTIONHOOK(void, LevelCell_loadCustomLevelCell, CCLayer* self) {
	
	LevelCell_loadCustomLevelCellO(self);
	if(GM->getGameVariable("1000013")) return;
	
	
	auto layer = (CCLayer*)self->getChildren()->objectAtIndex(1);
	
	
	auto level = MBO(void*, self, 0x198);
	
	int likes = MBO(int, level, 0x200) - MBO(int, level, 0x204);
	int downloads = MBO(int, level, 0x178);
	int count = layer->getChildrenCount();
	
	bool checkDownload = downloads > 1000;
	bool checkLikes = likes > 1000;
	
	if(!checkDownload && !checkLikes)
		return;
	
	for(int i = 0; i < count; i++)
	{
		auto node = (CCNode*)layer->getChildren()->objectAtIndex(i);

		if(auto label = dynamic_cast<CCLabelBMFont*>(node))
		{
			if(checkDownload && atoi(label->CCLabelBMFont::getString()) == downloads)
			{
				float scaleMult = 1.0f;
				label->CCLabelBMFont::setString(intToFormatString(downloads, scaleMult).c_str());
				float oldScale = label->getScale();
				label->setScale(oldScale * scaleMult);
				
				continue;
			}
			else if(checkLikes && atoi(label->CCLabelBMFont::getString()) == likes)
			{
				float scaleMult = 1.0f;
				label->CCLabelBMFont::setString(intToFormatString(likes, scaleMult).c_str());
				float oldScale = label->getScale();
				label->setScale(oldScale * scaleMult);
				
			}
		}
	}
}


namespace AbbreviatedLabels {
	void ApplyHooks() {
		HOOK("_ZN9LevelCell19loadCustomLevelCellEv", LevelCell_loadCustomLevelCellH, LevelCell_loadCustomLevelCellO);
	}
}