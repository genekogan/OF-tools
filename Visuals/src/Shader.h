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
    Shader(bool colorShader=true);
    
    void setup(int width, int height, bool clearControls=true);
    void enableTextureShaderMenu();
    void enableColorShaderMenu();
    
    void update();
    void draw(int x, int y);

    void setShader(string vert, string frag);

    void addParameter(string name, float min, float max);
    void addParameter(string name, ofVec2f min, ofVec2f max);
    void addParameter(string name, ofVec3f min, ofVec3f max);
    void addParameter(string name, ofFloatColor min, ofFloatColor max);
    
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
    
    void chooseColorShader(GuiElementEventArgs &e)
    {
        if      (e.name == "Blobby") setupBlobby();
        else if (e.name == "Bits") setupBits();
        else if (e.name == "BitsExperimental") setupBitsExperimental();
        else if (e.name == "Electro") setupElectro();
        else if (e.name == "Eye") setupEye();
        else if (e.name == "HerokuBubbles") setupHerokuBubbles();
        else if (e.name == "Landscape") setupLandscape();
        else if (e.name == "Monjori") setupMonjori();
        else if (e.name == "Nebula") setupNebula();
        else if (e.name == "Noisy") setupNoisy();
        else if (e.name == "Ikeda") setupIkeda();
        else if (e.name == "Rain") setupRain();
        else if (e.name == "Sinewave") setupSinewave();
        else if (e.name == "SinewaveExp") setupSinewaveExperimental();
        else if (e.name == "Wave") setupWave();
        else if (e.name == "FractalScope") setupFractalScope();
        else if (e.name == "FractalFlower") setupFractalFlower();
        else if (e.name == "Curtains") setupCurtains();
    }
    
    void chooseTextureShader(GuiElementEventArgs &e)
    {
        if      (e.name == "BrCoSa")    setupBrCoSa();
        else if (e.name == "Pixelate")  setupPixelate();
        else if (e.name == "BilateralFilter") setupBilateralFilter();
        else if (e.name == "Blur") setupBlur();
        else if (e.name == "Channels") setupChannels();
        else if (e.name == "Deform") setupDeform();
        else if (e.name == "Edges") setupEdges();
        else if (e.name == "HalftoneCmyk") setupHalftoneCmyk();
        else if (e.name == "Halftone") setupHalftone();
        else if (e.name == "Hue") setupHue();
        else if (e.name == "Invert") setupInvert();
        else if (e.name == "Neon") setupNeon();
        else if (e.name == "Patches") setupPatches();
        else if (e.name == "PixelRolls") setupPixelRolls();
        else if (e.name == "Grayscale") setupGrayscale();
        else if (e.name == "Threshold") setupThreshold();
        else if (e.name == "Wrap") setupWrap();
    }
    
    
private:
    
    vector<ShaderParameterBase *> shaderParameters;
    ofShader shader;
    ofFbo *fboTex;
    bool hasTexture;
    GuiMenu *menuChoose;
    GuiWidget *paramWidget;
    //GuiPanel *paramWidget;
};
