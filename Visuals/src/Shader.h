#pragma once

#include "ofMain.h"
#include "Scene.h"

//http://patriciogonzalezvivo.com/2015/thebookofshaders/


struct ShaderParameterBase
{
    virtual void update(ofShader *shader) {};
};

template<typename T>
struct ShaderParameter : public ShaderParameterBase
{
    ShaderParameter(string name, T *value)
    {
        this->name = name;
        this->value = value;
    }
    void update(ofShader *shader);
    T *value;
    string name;
};

template<> inline
void ShaderParameter<int>::update(ofShader *shader)
{
    shader->setUniform1f(name, *value);
}

template<> inline
void ShaderParameter<float>::update(ofShader *shader)
{
    shader->setUniform1f(name, *value);
}

template<> inline
void ShaderParameter<ofVec2f>::update(ofShader *shader)
{
    shader->setUniform2f(name, value->x, value->y);
}

template<> inline
void ShaderParameter<ofVec3f>::update(ofShader *shader)
{
    shader->setUniform3f(name, value->x, value->y, value->z);
}

template<> inline
void ShaderParameter<ofFloatColor>::update(ofShader *shader)
{
    shader->setUniform3f(name, value->r, value->g, value->b);
}



class Shader : public Scene
{
public:
    
    struct ShaderData
    {
        ShaderData(string name, string vert, string frag, GuiPanel &control);

        void addParameter(string name, float min, float max);
        void addParameter(string name, ofVec2f min, ofVec2f max);
        void addParameter(string name, ofVec3f min, ofVec3f max);
        void addParameter(string name, ofFloatColor min, ofFloatColor max);
        
        vector<ShaderParameterBase *> shaderParameters;
        GuiWidget *widget;
        string name;
        string vert;
        string frag;
    };
    
    Shader(bool colorShader=true);
    
    void setup(int width, int height, bool clearControls=true);
    void enableTextureShaderMenu();
    void enableColorShaderMenu();
    
    void update();
    void draw(int x, int y);
    
    void selectShader(string shaderName);
    void setTexture(ofFbo *fboTex);
    
    // color presets
    void setupBlobby();
    void setupBits();
    void setupBitsExperimental();
    void setupElectro();
    void setupEye();
    void setupHerokuBubbles();
    void setupLandscape();
    void setupMonjori();
    void setupNebula();
    void setupNoisy();
    void setupIkeda();
    void setupRain();
    void setupSinewave();
    void setupSinewaveExperimental();
    void setupWave();
    void setupFractalScope();
    void setupFractalFlower();
    void setupCurtains();
    
    // texture presets
    void setupBrCoSa();
    void setupPixelate();
    void setupBilateralFilter();
    void setupBlur();
    void setupChannels();
    void setupDeform();
    void setupEdges();
    void setupHalftoneCmyk();
    void setupHalftone();
    void setupHue();
    void setupInvert();
    void setupNeon();
    void setupPatches();
    void setupPixelRolls();
    void setupGrayscale();
    void setupThreshold();
    void setupWrap();
    
private:

    void chooseShader(GuiMenuEventArgs &e);
    ShaderData * createShader(string shaderName, string vert, string frag);

    map<string, ShaderData*> shaders;
    ShaderData *activeShader;
    ofShader shader;
    ofFbo *fboTex;
    bool hasTexture;
    GuiMenu *menuChoose;
    string nextShader;
    bool toSetNextShader;
};
