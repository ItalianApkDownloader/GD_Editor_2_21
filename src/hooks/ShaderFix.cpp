#include "ShaderFix.h"
#include <map>
#include <vector>
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"

#include "CCGLProgram.h"

using namespace cocos2d;

#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

#define RESET1F(program, location) program->setUniformLocationWith1f(location, 0)
#define RESET2F(program, location) program->setUniformLocationWith2f(location, 0, 0)

// color change
FUNCTIONHOOK(void, ShaderLayer_preColorChangeShader, CCLayer* self) {
	ShaderLayer_preColorChangeShaderO(self);

	if(
		MBO(float, self, 0x404) == 1.0
		&& MBO(float, self, 0x408) == 1.0
		&& MBO(float, self, 0x40C) == 1.0
		&& MBO(float, self, 0x410) == 0.0
		&& MBO(float, self, 0x414) == 0.0
		&& MBO(float, self, 0x418) == 0.0
	) {
		auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();
		program->setUniformLocationWith3f(MBO(int, self, 0x420), 0, 0, 0);
	}
} 

// shockline
FUNCTIONHOOK(void, ShaderLayer_preShockLineShader, CCLayer* self) {
    ShaderLayer_preShockLineShaderO(self);

    if(!(MBO(float, self, 0x210) > 0)) {
        auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();

        // reset all 1f
        for(uintptr_t i = 0x240; i <= 0x260; i = i + 4) {
            RESET1F(program, MBO(int, self, i));
        }

        // reset 2f
        RESET2F(program, MBO(int, self, 0x264));
        RESET2F(program, MBO(int, self, 0x268));
    }
}

// shockwave
FUNCTIONHOOK(void, ShaderLayer_preShockWaveShader, CCLayer* self) {
    ShaderLayer_preShockWaveShaderO(self);

    if(!(MBO(float, self, 0x1A0) > 0)) {
        auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();

        // reset all 1f
        for(uintptr_t i = 0x1E4; i <= 0x208; i = i + 4) {
            RESET1F(program, MBO(int, self, i));
        }
    }
}

// glitch
FUNCTIONHOOK(void, ShaderLayer_preGlitchShader, CCLayer* self) {
    ShaderLayer_preGlitchShaderO(self);

    if(!MBO(bool, self, 0x26C)) {
        auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();

        // reset all 1f
        for(uintptr_t i = 0x284; i <= 0x294; i = i + 4) {
            if(i != 0x290) RESET1F(program, MBO(int, self, i));
        }

        // reset all 2f
        RESET2F(program, MBO(int, self, 0x290));
    }
}

// chromatic glitch
FUNCTIONHOOK(void, ShaderLayer_preChromaticGlitchShader, CCLayer* self) {
    ShaderLayer_preChromaticGlitchShaderO(self);

    if(
        !MBO(bool, self, 0x2C0) 
        && MBO(float, self, 0x2AC) == 0
        && MBO(float, self, 0x2B0) == 0
        && MBO(float, self, 0x2BC) == 0
    ) {
        auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();

        // reset all 1f
        for(uintptr_t i = 0x2C4; i <= 0x2DC; i = i + 4) {
            RESET1F(program, MBO(int, self, i));
        }
    }
}

void ShaderFix::ApplyHooks() {
    HOOK("_ZN11ShaderLayer20preColorChangeShaderEv", ShaderLayer_preColorChangeShaderH, ShaderLayer_preColorChangeShaderO);
    HOOK("_ZN11ShaderLayer18preShockLineShaderEv", ShaderLayer_preShockLineShaderH, ShaderLayer_preShockLineShaderO);
    HOOK("_ZN11ShaderLayer18preShockWaveShaderEv", ShaderLayer_preShockWaveShaderH, ShaderLayer_preShockWaveShaderO);
    HOOK("_ZN11ShaderLayer15preGlitchShaderEv", ShaderLayer_preGlitchShaderH, ShaderLayer_preGlitchShaderO);
    HOOK("_ZN11ShaderLayer24preChromaticGlitchShaderEv", ShaderLayer_preChromaticGlitchShaderH, ShaderLayer_preChromaticGlitchShaderO);
}