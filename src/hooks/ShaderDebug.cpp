#include "ShaderDebug.h"
#include <map>
#include <vector>
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"

using namespace cocos2d;


std::vector<const char*> shaderTriggerUniforms;
std::map<const char*, float> shaderTriggerUniformValues1f;
std::map<const char*, std::vector<float>> shaderTriggerUniformValues2f;
std::map<const char*, std::vector<float>> shaderTriggerUniformValues3f;
std::map<const char*, std::vector<float>> shaderTriggerUniformValues4f;

void UILayerDebug::doInit() {
	CCLog("do init");
    shaderTriggerUniforms.clear();

	shaderTriggerUniformValues1f.clear();
	shaderTriggerUniformValues2f.clear();
	shaderTriggerUniformValues3f.clear();
	shaderTriggerUniformValues4f.clear();

	auto dir = CCDirector::sharedDirector();
	auto winSize = dir->getWinSize();

	auto label = CCLabelBMFont::create(" ", "chatFont.fnt");
	label->setAnchorPoint(CCPoint(0, 1));
	label->setScale(.5);
	label->setPosition(5, winSize.height - 2);
	label->setTag(1337);
	
	addChild(label);

	schedule(schedule_selector(UILayerDebug::scheduled), .05);
}

void UILayerDebug::scheduled(float delta) {
    auto label = (CCLabelBMFont*)this->getChildByTag(1337);

	if(label) {
		if(shaderTriggerUniforms.size() < 1) return;
		//if(shaderTriggerUniformValues1f.size() < 1) return;

		std::string mainStr;

		for(auto it : shaderTriggerUniforms) {
			// 1f
			float value = 0;
			int uniformLoc = std::distance(shaderTriggerUniforms.begin(), std::find(shaderTriggerUniforms.begin(), shaderTriggerUniforms.end(), it));
			if(uniformLoc < 1) continue;
			if(shaderTriggerUniformValues1f.find(it) != shaderTriggerUniformValues1f.end()) {
				value = shaderTriggerUniformValues1f[it];
				auto thangStr = CCString::createWithFormat("%s (%i): %f\n", it, uniformLoc, value)->getCString();
				mainStr.append(thangStr);
			}
			else {
				// 2f
				if(shaderTriggerUniformValues2f.find(it) != shaderTriggerUniformValues2f.end()) {
					auto values = shaderTriggerUniformValues2f[it];
					auto thangStr = CCString::createWithFormat("%s (%i): (%f, %f)\n", it, uniformLoc, values[0], values[1])->getCString();
					mainStr.append(thangStr);
				}
				else {
					// 3f
                    if(shaderTriggerUniformValues3f.find(it) != shaderTriggerUniformValues3f.end()) {
					    auto values = shaderTriggerUniformValues3f[it];
					    auto thangStr = CCString::createWithFormat("%s (%i): (%f, %f, %f)\n", it, uniformLoc, values[0], values[1], values[2])->getCString();
					    mainStr.append(thangStr);
				    }
                    else {
                        // 4f
                        if(shaderTriggerUniformValues4f.find(it) != shaderTriggerUniformValues4f.end()) {
                            auto values = shaderTriggerUniformValues4f[it];
                            auto thangStr = CCString::createWithFormat("%s (%i): (%f, %f, %f, %f)\n", it, uniformLoc, values[0], values[1], values[2], values[3])->getCString();
                            mainStr.append(thangStr);
                        }
                    }
				}
			}
			label->setString(mainStr.c_str(), true);
		}
		shaderTriggerUniformValues1f.clear();
		shaderTriggerUniformValues2f.clear();
		shaderTriggerUniformValues3f.clear();
		shaderTriggerUniformValues4f.clear();
	}
}

FUNCTIONHOOK(int, glGetUniformLocation, void* program, const char* uniformName) {
	auto loc = glGetUniformLocationO(program, uniformName);

	if(loc == -1 || loc == 0) return loc;

	if(shaderTriggerUniforms.size() < loc) shaderTriggerUniforms.resize(loc + 1);
	shaderTriggerUniforms[loc] = uniformName;

	//CCLog("UNIFORM NAME %s AT LOCATION %i", uniformName, loc);

	return loc;
}

FUNCTIONHOOK(void, CCGLProgram_setUniformLocationWith1f, void* self, int location, float value) {
	CCGLProgram_setUniformLocationWith1fO(self, location, value);
	
	if(location == -1) return;
	
	if(shaderTriggerUniforms.size() < location) return;

	auto uniformName = shaderTriggerUniforms[location];

	shaderTriggerUniformValues1f.insert(std::make_pair(uniformName, value));

	//CCLog("UNIFORM %s (%i): %f", uniformName, location, value);
}

FUNCTIONHOOK(void, CCGLProgram_setUniformLocationWith2f, void* self, int location, float value1, float value2) {
	CCGLProgram_setUniformLocationWith2fO(self, location, value1, value2);

	if(location == -1) return;
	
	if(shaderTriggerUniforms.size() < location) return;

	auto uniformName = shaderTriggerUniforms[location];

	std::vector<float> values = {value1, value2};
	shaderTriggerUniformValues2f.insert(std::make_pair(uniformName, values));
}

FUNCTIONHOOK(void, CCGLProgram_setUniformLocationWith3f, void* self, int location, float value1, float value2, float value3) {
	CCGLProgram_setUniformLocationWith3fO(self, location, value1, value2, value3);

	if(location == -1) return;
	
	if(shaderTriggerUniforms.size() < location) return;

	auto uniformName = shaderTriggerUniforms[location];

	std::vector<float> values = {value1, value2, value3};
	shaderTriggerUniformValues3f.insert(std::make_pair(uniformName, values));
}

FUNCTIONHOOK(void, CCGLProgram_setUniformLocationWith4f, void* self, int location, float value1, float value2, float value3, float value4) {
	CCGLProgram_setUniformLocationWith4fO(self, location, value1, value2, value3, value4);

	if(location == -1) return;
	
	if(shaderTriggerUniforms.size() < location) return;

	auto uniformName = shaderTriggerUniforms[location];

	std::vector<float> values = {value1, value2, value3, value4};
	shaderTriggerUniformValues4f.insert(std::make_pair(uniformName, values));
}

void DevDebugHooks::ApplyHooks() {
    HOOK("glGetUniformLocation", glGetUniformLocationH, glGetUniformLocationO);
	HOOK("_ZN7cocos2d11CCGLProgram24setUniformLocationWith1fEif", CCGLProgram_setUniformLocationWith1fH, CCGLProgram_setUniformLocationWith1fO);
	HOOK("_ZN7cocos2d11CCGLProgram24setUniformLocationWith2fEiff", CCGLProgram_setUniformLocationWith2fH, CCGLProgram_setUniformLocationWith2fO);
	HOOK("_ZN7cocos2d11CCGLProgram24setUniformLocationWith3fEifff", CCGLProgram_setUniformLocationWith3fH, CCGLProgram_setUniformLocationWith3fO);
	HOOK("_ZN7cocos2d11CCGLProgram24setUniformLocationWith4fEiffff", CCGLProgram_setUniformLocationWith4fH, CCGLProgram_setUniformLocationWith4fO);
}