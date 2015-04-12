#pragma once

#include "ofxPostProcessing.h"
#include "ofxPostGlitch.h"
#include "TimeFunction.h"
#include "Control.h"
#include "Shader.h"
#include "Modifier.h"


class PostProcessingMod : public Modifier
{
public:
    
    struct PostProcessingRenderPass
    {
        string name;
        RenderPass::Ptr pass;
        bool enabled;
        GuiWidget *widget;
        
        PostProcessingRenderPass(string name, RenderPass::Ptr pass, GuiPanel & panel);
        void setEnabled(bool enabled);
    };
    
    PostProcessingMod() : Modifier()
    {
        type = "PostProcessing";
    }

    void setActive(bool active)
    {
        this->active = active;
        panel.setActive(active);
    }
    
    void setup(int width, int height);
    void toggleEffects(GuiMenuEventArgs & evt);
    
    GuiPanel & getControl() {return panel;}
    
    void updateParameters();
    void render(ofFbo *fbo);
    
    ofxPostProcessing post;
    map<string, PostProcessingRenderPass*> passes;
    
    FxaaPass::Ptr alias;
    BloomPass::Ptr bloom;
    DofPass::Ptr dof;
    KaleidoscopePass::Ptr kaleidoscope;
    NoiseWarpPass::Ptr noiseWarp;
    PixelatePass::Ptr pixelate;
    EdgePass::Ptr edges;
    VerticalTiltShifPass::Ptr vTiltShift;
    HorizontalTiltShifPass::Ptr hTiltShift;
    GodRaysPass::Ptr godRay;
    ToonPass::Ptr toon;
    BleachBypassPass::Ptr bleachBypass;
    ConvolutionPass::Ptr convolve;
    FakeSSSPass::Ptr fakeSSS;
    LimbDarkeningPass::Ptr limbDarken;
    RGBShiftPass::Ptr shift;
    RimHighlightingPass::Ptr rimHighlight;
    SSAOPass::Ptr ssao;
    ZoomBlurPass::Ptr zoomBlur;

    GuiPanel panel;
    
    float dofAperture;
    float dofFocus;
    float dofMaxBlur;
    
    float kaleidoscopeSegments;
    
    float noiseWarpAmplitude;
    float noiseWarpFrequency;
    float noiseWarpSpeed;
    
    ofVec2f pixelateResolution;
    
    float edgesHue;
    float edgesSaturation;
    
    float vTiltShiftV;
    float vTiltShiftR;
    
    float hTiltShiftH;
    float hTiltShiftR;
    
    float godRayLightDirDOTviewDir;
    ofVec3f godRayLightPoisitonOnScreen;
    
    ofVec4f toonSpecularColor;
    float toonShinyness;
    float toonLevel;
    bool toonEnableSpecular;
    float toonEdgeThreshold;
    ofVec4f toonDiffuseColor;
    ofVec4f toonAmbientColor;
    
    float bleachBypassOpacity;
    
    float fakeSSSAttenuationOffset;
    ofVec4f fakeSSSBaseColor;
    ofVec4f fakeSSSExtinctionCoefficient;
    ofVec4f fakeSSSLightColor;
    ofPoint fakeSSSLightPosition;
    float fakeSSSMaterialThickness;
    float fakeSSSRimScale;
    float fakeSSSSpecular;
    ofVec4f fakeSSSSpecularColor;
    
    float limbDarkenBrightness;
    ofVec3f limbDarkenEndColor;
    float limbDarkenRadialScale;
    ofVec3f limbDarkenStartColor;
    
    float shiftAmount;
    float shiftAngle;
    
    float ssaoCameraNear;
    float ssaoCameraFar;
    float ssaoFogNear;
    float ssaoFogFar;
    bool ssaoFogEnabled;
    bool ssaoOnlyAO;
    float ssaoAoClamp;
    float ssaoLumInfluence;
    
    float zoomBlurCenterX;
    float zoomBlurCenterY;
    float zoomBlurExposure;
    float zoomBlurDecay;
    float zoomBlurDensity;
    float zoomBlurWeight;
    float zoomBlurClamp;
};
