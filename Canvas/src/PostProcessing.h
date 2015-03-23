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
    void setup(int width, int height);
    
    void checkEffectActive(shared_ptr<RenderPass> pass, bool isEnabled)
    {
        bool wasEnabled = pass->getEnabled();
        pass->setEnabled(isEnabled);
        if (wasEnabled != isEnabled) {
            //setupParameterGui();
        }

    }
    
    GuiPanel & getControl()
    {
        return panel;
    }
    
    void render(ofFbo *fbo)
    {
        checkEffectActive(alias, aliasEnabled);
        checkEffectActive(bloom, bloomEnabled);
        checkEffectActive(dof, dofEnabled);
        checkEffectActive(kaleidoscope, kaleidoscopeEnabled);
        checkEffectActive(noiseWarp, noiseWarpEnabled);
        checkEffectActive(pixelate, pixelateEnabled);
        checkEffectActive(edges, edgesEnabled);
        checkEffectActive(vTiltShift, vTiltShiftEnabled);
        checkEffectActive(hTiltShift, hTiltShiftEnabled);
        checkEffectActive(godRay, godRayEnabled);
        checkEffectActive(toon, toonEnabled);
        checkEffectActive(bleachBypass, bleachBypassEnabled);
        checkEffectActive(convolve, convolveEnabled);
        checkEffectActive(fakeSSS, fakeSSSEnabled);
        checkEffectActive(limbDarken, limbDarkenEnabled);
        checkEffectActive(shift, shiftEnabled);
        checkEffectActive(rimHighlight, rimHighlightEnabled);
        checkEffectActive(ssao, ssaoEnabled);
        checkEffectActive(zoomBlur, zoomBlurEnabled);
        
        if (alias->getEnabled()) {
        }
        if (bloom->getEnabled()) {
        }
        if (dof->getEnabled()) {
            dof->setAperture(dofAperture);
            dof->setFocus(dofFocus);
            dof->setMaxBlur(dofMaxBlur);
        }
        if (kaleidoscope->getEnabled()) {
            kaleidoscope->setSegments(kaleidoscopeSegments);
        }
        if (noiseWarp->getEnabled()) {
            noiseWarp->setAmplitude(noiseWarpAmplitude);
            noiseWarp->setFrequency(noiseWarpFrequency);
            noiseWarp->setSpeed(noiseWarpSpeed);
        }
        if (pixelate->getEnabled()) {
            pixelate->setResolution(pixelateResolution);
        }
        if (edges->getEnabled()) {
            edges->setHue(edgesHue);
            edges->setSaturation(edgesSaturation);
        }
        if (vTiltShift->getEnabled()) {
            vTiltShift->setV(vTiltShiftV);
            vTiltShift->setR(vTiltShiftR);
        }
        if (hTiltShift->getEnabled()) {
            hTiltShift->setH(hTiltShiftH);
            hTiltShift->setR(hTiltShiftR);
        }
        if (godRay->getEnabled()) {
            godRay->setLightDirDOTviewDir(godRayLightDirDOTviewDir);
            godRay->setLightPositionOnScreen(godRayLightPoisitonOnScreen);
        }
        if (toon->getEnabled()) {
            toon->setSpecularColor(toonSpecularColor);
            toon->setShinyness(toonShinyness);
            toon->setLevel(toonLevel);
            toon->setEnableSpecular(toonEnableSpecular);
            toon->setEdgeThreshold(toonEdgeThreshold);
            toon->setDiffuseColor(toonDiffuseColor);
            toon->setAmbientColor(toonAmbientColor);
        }
        if (bleachBypass->getEnabled()) {
            bleachBypass->setOpacity(bleachBypassOpacity);
        }
        if (convolve->getEnabled()) {
        }
        if (fakeSSS->getEnabled()) {
            fakeSSS->setAttenuationOffset(fakeSSSAttenuationOffset);
            fakeSSS->setBaseColor(fakeSSSBaseColor);
            fakeSSS->setExtinctionCoefficient(fakeSSSExtinctionCoefficient);
            fakeSSS->setLightColor(fakeSSSLightColor);
            fakeSSS->setLightPosition(fakeSSSLightPosition);
            fakeSSS->setMaterialThickness(fakeSSSMaterialThickness);
            fakeSSS->setRimScale(fakeSSSRimScale);
            fakeSSS->setSpecular(fakeSSSSpecular);
            fakeSSS->setSpecularColor(fakeSSSSpecularColor);
        }
        if (limbDarken->getEnabled()) {
            limbDarken->setBrightness(limbDarkenBrightness);
            limbDarken->setEndColor(limbDarkenEndColor);
            limbDarken->setRadialScale(limbDarkenRadialScale);
            limbDarken->setStartColor(limbDarkenStartColor);
        }
        if (shift->getEnabled()) {
            shift->setAmount(shiftAmount);
            shift->setAngle(shiftAngle);
        }
        if (rimHighlight->getEnabled()) {
        }
        if (ssao->getEnabled()) {
            ssao->setCameraNear(ssaoCameraNear);
            ssao->setCameraFar(ssaoCameraFar);
            ssao->setFogNear(ssaoFogNear);
            ssao->setFogFar(ssaoFogFar);
            ssao->setFogEnabled(ssaoFogEnabled);
            ssao->setOnlyAO(ssaoOnlyAO);
            ssao->setAoClamp(ssaoAoClamp);
            ssao->setLumInfluence(ssaoLumInfluence);
        }
        if (zoomBlur->getEnabled()) {
            zoomBlur->setCenterX(zoomBlurCenterX);
            zoomBlur->setCenterY(zoomBlurCenterY);
            zoomBlur->setExposure(zoomBlurExposure);
            zoomBlur->setDecay(zoomBlurDecay);
            zoomBlur->setDensity(zoomBlurDensity);
            zoomBlur->setWeight(zoomBlurWeight);
            zoomBlur->setClamp(zoomBlurClamp);
        }
        
        //fbo = *texLayer->getFbo();
        
        post.begin();
        ofPushMatrix();
        ofTranslate(width, height);
        ofRotate(180);
        fbo->draw(0, 0);
        ofPopMatrix();
        post.end();
    }
    
    
    ofxPostProcessing post;
    
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
    

    
    /* toggles */
    bool aliasEnabled;
    bool bloomEnabled;
    bool dofEnabled;
    bool kaleidoscopeEnabled;
    bool noiseWarpEnabled;
    bool pixelateEnabled;
    bool edgesEnabled;
    bool vTiltShiftEnabled;
    bool hTiltShiftEnabled;
    bool godRayEnabled;
    bool toonEnabled;
    bool bleachBypassEnabled;
    bool convolveEnabled;
    bool fakeSSSEnabled;
    bool limbDarkenEnabled;
    bool shiftEnabled;
    bool rimHighlightEnabled;
    bool ssaoEnabled;
    bool zoomBlurEnabled;
    
    
    /* parameters */
    
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
    
    
    GuiPanel panel;
    
};
