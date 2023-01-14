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

#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

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
    bool grayscale = (MBO(float, self, 0x3BC) > 0.0f);
    bool sepia = (MBO(float, self, 0x3D8) > 0.0f);
    bool invertColor = (MBO(float, self, 0x3E0) > 0.0f);
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
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_grayscaleOn"), grayscale);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_sepiaOn"), sepia);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_invertColorOn"), invertColor);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_hueShiftOn"), hueShift);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_colorChangeOn"), colorChange);
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_splitScreenOn"), splitScreen);

    //CCLog("ShaderDebug:\nshockWave:%i\nshockLine:%i\nglitch:%i\nchroma glitch:%i\nlens circle:%i\nbulge:%i\ngrayscale:%i\nsepia:%i\ninvertColor:%i\nhueShift:%i\ncolorChange:%i\nsplitScreen:%i\n",
    //shockWave, shockLine, glitch, cglitch, lensCircle, bulge, grayscale, sepia, invertColor, hueShift, colorChange, splitScreen);
}

// compileShader func from latest cocos2dx v2 lmao
FUNCTIONHOOK(bool, CCGLProgram_compileShader, CCGLProgram *self,  GLuint * shader, GLenum type, const GLchar* source)
{
    GLint status;
 
    if (!source)
    {
        return false;
    }
    
    const GLchar *sources[] = {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#if CC_TARGET_PLATFORM == CC_PLATFORM_NACL
        "precision highp float;\n"
#else
        (type == GL_VERTEX_SHADER ? "precision highp float;\n" : "precision mediump float;\n"),
#endif
#endif
        "uniform mat4 CC_PMatrix;\n"
        "uniform mat4 CC_MVMatrix;\n"
        "uniform mat4 CC_MVPMatrix;\n"
        "uniform vec4 CC_Time;\n"
        "uniform vec4 CC_SinTime;\n"
        "uniform vec4 CC_CosTime;\n"
        "uniform vec4 CC_Random01;\n"
        "//CC INCLUDES END\n\n",
        source,
    };

    *shader = glCreateShader(type);
    glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (! status)
    {
        GLsizei length;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);
		
		glGetShaderSource(*shader, length, NULL, src);
		CCLog("cocos2d: ERROR: Failed to compile shader:\n%s", src);
        
        if (type == GL_VERTEX_SHADER)
        {
            CCLog("cocos2d: %s", self->vertexShaderLog());
        }
        else
        {
            CCLog("cocos2d: %s", self->fragmentShaderLog());
        }
        free(src);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        return false;
#else
		abort();
#endif
    }
    return (status == GL_TRUE);
}

void ShaderFix::ApplyHooks() 
{
    HOOK("_ZN7cocos2d11CCGLProgram13compileShaderEPjjPKc", CCGLProgram_compileShaderH, CCGLProgram_compileShaderO);

    HOOK("_ZN11ShaderLayer19performCalculationsEv", ShaderLayer_performCalculationsH, ShaderLayer_performCalculationsO);
}