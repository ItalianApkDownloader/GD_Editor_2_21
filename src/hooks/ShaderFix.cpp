#include "ShaderFix.h"
#include <map>
#include <vector>
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"

#include "CCDirector.h"
#include "CCGLProgram.h"
#include "ccGLStateCache.h"

using namespace cocos2d;


#define RESET1F(program, location) program->setUniformLocationWith1f(location, 0)
#define RESET2F(program, location) program->setUniformLocationWith2f(location, 0, 0)
#define RESET3F(program, location) program->setUniformLocationWith3f(location, 0, 0, 0)
#define RESET4F(program, location) program->setUniformLocationWith4f(location, 0, 0, 0, 0)

CCGLProgram* getGLProgram(CCLayer* shaderLayer)
{
    auto spr = MBO(CCSprite*, shaderLayer, 0x154);
	return spr->getShaderProgram();
}
FUNCTIONHOOK(void, ShaderLayer_performCalculations, CCLayer* self)
{
    ShaderLayer_performCalculationsO(self);


    CCGLProgram* glProgram = getGLProgram(self);
    
    bool shockWave = (MBO(double, self, 0x1A0) > 0.0);
    bool shockLine = (MBO(double, self, 0x210) > 0.0);
    bool glitch = (MBO(bool, self, 0x26C));
    bool cglitch = !(!MBO(bool, self, 0x2C0) || MBO(float, self, 0x2AC) == 0 && MBO(float, self, 0x2B0) == 0 && MBO(float, self, 0x2BC) == 0);
    bool lensCircle = (MBO(float, self, 0x2F4) > 0.0f);
    bool bulge = (MBO(float, self, 0x37C) > 0.0f);
    bool hueShift = (MBO(float, self, 0x3F8) != 0.0f);
    bool colorChange = !(MBO(float, self, 0x404) == 1.0
		&& MBO(float, self, 0x408) == 1.0
		&& MBO(float, self, 0x40C) == 1.0
		&& MBO(float, self, 0x410) == 0.0
		&& MBO(float, self, 0x414) == 0.0
		&& MBO(float, self, 0x418) == 0.0);
    bool splitScreen = (MBO(float, self, 0x424) != 0.0f || MBO(float, self, 0x428) != 0.0f);

    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_shockWaveOn"), shockWave);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_shockLineOn"), shockLine);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_glitchOn"), glitch);
    // fuj chromatic. always on.
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_cGOn"), cglitch);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_lensCircleOn"), lensCircle);
    // fuj blurs. always on.
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_bulgeOn"), bulge);
    // fuj pinch. always on.
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_hueShiftOn"), hueShift);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_colorChangeOn"), colorChange);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_splitScreenOn"), splitScreen);

    //CCLog("ShaderDebug:\nshockWave:%i\nshockLine:%i\nglitch:%i\nchroma glitch:%i\nlens circle:%i\nbulge:%i\ngrayscale:%i\nsepia:%i\ninvertColor:%i\nhueShift:%i\ncolorChange:%i\nsplitScreen:%i\n",
    //shockWave, shockLine, glitch, cglitch, lensCircle, bulge, grayscale, sepia, invertColor, hueShift, colorChange, splitScreen);
}

void ShaderFix::ApplyHooks() 
{
    HOOK("_ZN11ShaderLayer19performCalculationsEv", ShaderLayer_performCalculationsH, ShaderLayer_performCalculationsO);
}