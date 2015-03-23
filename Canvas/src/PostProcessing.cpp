//
//  PostGlitch.cpp
//  template_simple
//
//  Created by Eugene Kogan on 3/23/15.
//
//

#include "PostProcessing.h"


void PostProcessingMod::setup(int width, int height)
{
    Modifier::setup(width, height);
    
    
    post.init(width, height);
    
    alias = post.createPass<FxaaPass>();
    bloom = post.createPass<BloomPass>();
    dof = post.createPass<DofPass>();
    kaleidoscope = post.createPass<KaleidoscopePass>();
    noiseWarp = post.createPass<NoiseWarpPass>();
    pixelate = post.createPass<PixelatePass>();
    edges = post.createPass<EdgePass>();
    vTiltShift = post.createPass<VerticalTiltShifPass>();
    hTiltShift = post.createPass<HorizontalTiltShifPass>();
    godRay = post.createPass<GodRaysPass>();
    toon = post.createPass<ToonPass>();
    bleachBypass = post.createPass<BleachBypassPass>();
    convolve = post.createPass<ConvolutionPass>();
    fakeSSS = post.createPass<FakeSSSPass>();
    limbDarken = post.createPass<LimbDarkeningPass>();
    shift = post.createPass<RGBShiftPass>();
    rimHighlight = post.createPass<RimHighlightingPass>();
    ssao = post.createPass<SSAOPass>();
    zoomBlur = post.createPass<ZoomBlurPass>();
    
    //control.setName("PostProcessingMod");
    panel.addToggle("alias", &aliasEnabled);
    panel.addToggle("bloom", &bloomEnabled);
    panel.addToggle("dof", &dofEnabled);
    panel.addToggle("kaleidoscope", &kaleidoscopeEnabled);
    panel.addToggle("noiseWarp", &noiseWarpEnabled);
    panel.addToggle("pixelate", &pixelateEnabled);
    panel.addToggle("edges", &edgesEnabled);
    panel.addToggle("vTiltShift", &vTiltShiftEnabled);
    panel.addToggle("hTiltShift", &hTiltShiftEnabled);
    panel.addToggle("godRay", &godRayEnabled);
    panel.addToggle("toon", &toonEnabled);
    panel.addToggle("bleachBypass", &bleachBypassEnabled);
    panel.addToggle("convolve", &convolveEnabled);
    panel.addToggle("fakeSSS", &fakeSSSEnabled);
    panel.addToggle("limbDarken", &limbDarkenEnabled);
    panel.addToggle("shift", &shiftEnabled);
    panel.addToggle("rimHighlight", &rimHighlightEnabled);
    panel.addToggle("ssao", &ssaoEnabled);
    panel.addToggle("zoomBlur", &zoomBlurEnabled);
    
    /* add all parameters to control, but make them invisible in main gui */
    /* they are accessible instead in the secondary gui */
    panel.addSlider("aperture", &dofAperture, 0.0f, 1.0f);
    panel.addSlider("focus", &dofFocus, 0.95f, 1.0f);
    panel.addSlider("maxBlur", &dofMaxBlur, 0.0f, 1.0f);
    panel.addSlider("segments", &kaleidoscopeSegments, 0.0f, 16.0f);
    panel.addSlider("amplitude", &noiseWarpAmplitude, 0.0f, 1.0f);
    panel.addSlider("frequency", &noiseWarpFrequency, 0.0f, 1.0f);
    panel.addSlider("speed", &noiseWarpSpeed, 0.0f, 1.0f);
    panel.addSlider("resolution", &pixelateResolution, ofVec2f(0, 0), ofVec2f(100, 100));
    panel.addSlider("hue", &edgesHue, 0.0f, 1.0f);
    panel.addSlider("saturation", &edgesSaturation, 0.0f, 1.0f);
    panel.addSlider("V", &vTiltShiftV, 0.0f, 0.1f);
    panel.addSlider("R", &vTiltShiftR, 0.0f, 0.1f);
    panel.addSlider("H", &hTiltShiftH, 0.0f, 0.1f);
    panel.addSlider("R", &hTiltShiftR, 0.0f, 0.1f);
    panel.addSlider("lightDirDOTviewDir", &godRayLightDirDOTviewDir, 0.0f, 1.0f);
    panel.addSlider("lightPoisitonOnScreen", &godRayLightPoisitonOnScreen, ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1));
    panel.addSlider("specularColor", &toonSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    panel.addSlider("shinyness", &toonShinyness, 0.0f, 1.0f);
    panel.addSlider("level", &toonLevel, 0.0f, 1.0f);

    panel.addToggle("enableSpecular", &toonEnableSpecular);
    panel.addSlider("edgeThreshold", &toonEdgeThreshold, 0.0f, 1.0f);
    panel.addSlider("diffuseColor", &toonDiffuseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    panel.addSlider("ambientColor", &toonAmbientColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    panel.addSlider("bypassOpacity", &bleachBypassOpacity, 0.0f, 1.0f);
    panel.addSlider("attenuationOffset", &fakeSSSAttenuationOffset, 0.0f, 1.0f);
    panel.addSlider("baseColor", &fakeSSSBaseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    panel.addSlider("extinctionCoefficient", &fakeSSSExtinctionCoefficient, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    panel.addSlider("lightColor", &fakeSSSLightColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    panel.addSlider("lightPosition", &fakeSSSLightPosition, ofPoint(0, 0), ofPoint(1, 1));
    panel.addSlider("materialThickness", &fakeSSSMaterialThickness, 0.0f, 1.0f);
    panel.addSlider("rimScale", &fakeSSSRimScale, 0.0f, 1.0f);
    panel.addSlider("specular", &fakeSSSSpecular, 0.0f, 1.0f);
    panel.addSlider("specularColor", &fakeSSSSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    panel.addSlider("brightness", &limbDarkenBrightness, 0.0f, 1.0f);
    panel.addSlider("endColor", &limbDarkenEndColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
    panel.addSlider("radialScale", &limbDarkenRadialScale, 0.0f, 1.0f);
    panel.addSlider("startColor", &limbDarkenStartColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
    panel.addSlider("amount", &shiftAmount, 0.0f, 1.0f);
    panel.addSlider("angle", &shiftAngle, 0.0f, 1.0f);
    panel.addSlider("cameraNear", &ssaoCameraNear, 0.0f, 1.0f);
    panel.addSlider("cameraFar", &ssaoCameraFar, 0.0f, 1.0f);
    panel.addSlider("fogNear", &ssaoFogNear, 0.0f, 1.0f);
    panel.addSlider("fogFar", &ssaoFogFar, 0.0f, 1.0f);
    panel.addToggle("fogEnabled", &ssaoFogEnabled);
    panel.addToggle("onlyAO", &ssaoOnlyAO);
    panel.addSlider("aoClamp", &ssaoAoClamp, 0.0f, 1.0f);
    panel.addSlider("lumInfluence", &ssaoLumInfluence, 0.0f, 1.0f);
    panel.addSlider("centerX", &zoomBlurCenterX, 0.0f, 1.0f);
    panel.addSlider("centerY", &zoomBlurCenterY, 0.0f, 1.0f);
    panel.addSlider("exposure", &zoomBlurExposure, 0.0f, 1.0f);
    panel.addSlider("decay", &zoomBlurDecay, 0.0f, 1.0f);
    panel.addSlider("density", &zoomBlurDensity, 0.0f, 1.0f);
    panel.addSlider("weight", &zoomBlurWeight, 0.0f, 1.0f);
    panel.addSlider("clamp", &zoomBlurClamp, 0.0f, 1.0f);

    /*
     // if to use menu instead of radio list
     string pp[] = {"alias", "bloom", "dof", "kaleidoscope", "noiseWarp",
     "pixelate", "edges", "vTiltShift", "hTiltShift", "godRay",
     "toon", "bleachBypass", "convolve", "fakeSSS",
     "limbDarken", "shift", "rimHighlights", "ssao", "zoomBlur"};
     vector<string> items = vector<string>(pp, pp + sizeof(pp) / sizeof(pp[0]));
     control.setName("PostProcessingMod");
     control.addMenu("Effects", items, this, &PostProcessingModLayer::enableEffect);
     */
    
    aliasEnabled = false;
    bloomEnabled = false;
    dofEnabled = false;
    kaleidoscopeEnabled = false;
    noiseWarpEnabled = false;
    pixelateEnabled = false;
    edgesEnabled = false;
    vTiltShiftEnabled = false;
    hTiltShiftEnabled = false;
    godRayEnabled = false;
    toonEnabled = false;
    bleachBypassEnabled = false;
    convolveEnabled = false;
    fakeSSSEnabled = false;
    limbDarkenEnabled = false;
    shiftEnabled = false;
    rimHighlightEnabled = false;
    ssaoEnabled = false;
    zoomBlurEnabled = false;
    
    
    
 /*
        gui.addParameter("aperture", &dofAperture, 0.0f, 1.0f);
        gui.addParameter("focus", &dofFocus, 0.95f, 1.0f);
        gui.addParameter("maxBlur", &dofMaxBlur, 0.0f, 1.0f);
 
    
        gui.addParameter("segments", &kaleidoscopeSegments, 0.0f, 16.0f);

        gui.addParameter("amplitude", &noiseWarpAmplitude, 0.0f, 1.0f);
        gui.addParameter("frequency", &noiseWarpFrequency, 0.0f, 1.0f);
        gui.addParameter("speed", &noiseWarpSpeed, 0.0f, 1.0f);

        gui.addParameter("resolution", &pixelateResolution, ofVec2f(0, 0), ofVec2f(100, 100));

        gui.addParameter("hue", &edgesHue, 0.0f, 1.0f);
        gui.addParameter("saturation", &edgesSaturation, 0.0f, 1.0f);

        gui.addParameter("V", &vTiltShiftV, 0.0f, 1.0f);
        gui.addParameter("R", &vTiltShiftR, 0.0f, 1.0f);

        gui.addParameter("H", &hTiltShiftH, 0.0f, 0.1f);
        gui.addParameter("R", &hTiltShiftR, 0.0f, 0.1f);

    
        gui.addParameter("lightDirDOTviewDir", &godRayLightDirDOTviewDir, 0.0f, 1.0f);
        gui.addParameter("lightPoisitonOnScreen", &godRayLightPoisitonOnScreen, ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1));

    
        gui.addParameter("specularColor", &toonSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("shinyness", &toonShinyness, 0.0f, 1.0f);
        gui.addParameter("level", &toonLevel, 0.0f, 1.0f);
        gui.addParameter("enableSpecular", &toonEnableSpecular);
        gui.addParameter("edgeThreshold", &toonEdgeThreshold, 0.0f, 1.0f);
        gui.addParameter("diffuseColor", &toonDiffuseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("ambientColor", &toonAmbientColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    
        gui.addParameter("bypassOpacity", &bleachBypassOpacity, 0.0f, 1.0f);
    
        gui.addParameter("attenuationOffset", &fakeSSSAttenuationOffset, 0.0f, 1.0f);
        gui.addParameter("baseColor", &fakeSSSBaseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("extinctionCoefficient", &fakeSSSExtinctionCoefficient, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("lightColor", &fakeSSSLightColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
        gui.addParameter("lightPosition", &fakeSSSLightPosition, ofPoint(0, 0), ofPoint(1, 1));
        gui.addParameter("materialThickness", &fakeSSSMaterialThickness, 0.0f, 1.0f);
        gui.addParameter("rimScale", &fakeSSSRimScale, 0.0f, 1.0f);
        gui.addParameter("specular", &fakeSSSSpecular, 0.0f, 1.0f);
        gui.addParameter("specularColor", &fakeSSSSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    
        gui.addParameter("brightness", &limbDarkenBrightness, 0.0f, 1.0f);
        gui.addParameter("endColor", &limbDarkenEndColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
        gui.addParameter("radialScale", &limbDarkenRadialScale, 0.0f, 1.0f);
        gui.addParameter("startColor", &limbDarkenStartColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
    
        gui.addParameter("amount", &shiftAmount, 0.0f, 1.0f);
        gui.addParameter("angle", &shiftAngle, 0.0f, 1.0f);
    
        gui.addParameter("cameraNear", &ssaoCameraNear, 0.0f, 1.0f);
        gui.addParameter("cameraFar", &ssaoCameraFar, 0.0f, 1.0f);
        gui.addParameter("fogNear", &ssaoFogNear, 0.0f, 1.0f);
        gui.addParameter("fogFar", &ssaoFogFar, 0.0f, 1.0f);
        gui.addParameter("fogEnabled", &ssaoFogEnabled);
        gui.addParameter("onlyAO", &ssaoOnlyAO);
        gui.addParameter("aoClamp", &ssaoAoClamp, 0.0f, 1.0f);
        gui.addParameter("lumInfluence", &ssaoLumInfluence, 0.0f, 1.0f);

        gui.addParameter("centerX", &zoomBlurCenterX, 0.0f, 1.0f);
        gui.addParameter("centerY", &zoomBlurCenterY, 0.0f, 1.0f);
        gui.addParameter("exposure", &zoomBlurExposure, 0.0f, 1.0f);
        gui.addParameter("decay", &zoomBlurDecay, 0.0f, 1.0f);
        gui.addParameter("density", &zoomBlurDensity, 0.0f, 1.0f);
        gui.addParameter("weight", &zoomBlurWeight, 0.0f, 1.0f);
        gui.addParameter("clamp", &zoomBlurClamp, 0.0f, 1.0f);
*/

    
}
