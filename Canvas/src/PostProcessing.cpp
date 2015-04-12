#include "PostProcessing.h"


PostProcessingMod::PostProcessingRenderPass::PostProcessingRenderPass(string name, RenderPass::Ptr pass, GuiPanel & panel)
{
    this->name = name;
    this->pass = pass;
    this->widget = panel.addWidget(name);
    widget->setCollapsible(true);
    setEnabled(false);
}

void PostProcessingMod::PostProcessingRenderPass::setEnabled(bool enabled)
{
    this->enabled = enabled;
    pass->setEnabled(enabled);
    widget->setActive(enabled);
}

void PostProcessingMod::setup(int width, int height)
{
    Modifier::setup(width, height);
    
    panel.setName("PostProcessing");
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
    
    GuiMenu *menu = panel.addMenu("Effects", this, &PostProcessingMod::toggleEffects);
    menu->setMultipleChoice(true);

    passes["alias"] = new PostProcessingRenderPass("alias", alias, panel);
    passes["bloom"] = new PostProcessingRenderPass("bloom", bloom, panel);
    passes["dof"] = new PostProcessingRenderPass("dof", dof, panel);
    passes["kaleidoscope"] = new PostProcessingRenderPass("kaleidoscope", kaleidoscope, panel);
    passes["noiseWarp"] = new PostProcessingRenderPass("noiseWarp", noiseWarp, panel);
    passes["pixelate"] = new PostProcessingRenderPass("pixelate", pixelate, panel);
    passes["edges"] = new PostProcessingRenderPass("edges", edges, panel);
    passes["vTiltShift"] = new PostProcessingRenderPass("vTiltShift", vTiltShift, panel);
    passes["hTiltShift"] = new PostProcessingRenderPass("hTiltShift", hTiltShift, panel);
    passes["godRay"] = new PostProcessingRenderPass("godRay", godRay, panel);
    passes["toon"] = new PostProcessingRenderPass("toon", toon, panel);
    passes["bleachBypass"] = new PostProcessingRenderPass("bleachBypass", bleachBypass, panel);
    passes["convolve"] = new PostProcessingRenderPass("convolve", convolve, panel);
    passes["fakeSSS"] = new PostProcessingRenderPass("fakeSSS", fakeSSS, panel);
    passes["limbDarken"] = new PostProcessingRenderPass("limbDarken", limbDarken, panel);
    passes["shift"] = new PostProcessingRenderPass("shift", shift, panel);
    passes["rimHighlight"] = new PostProcessingRenderPass("rimHighlight", rimHighlight, panel);
    passes["ssao"] = new PostProcessingRenderPass("ssao", ssao, panel);
    passes["zoomBlur"] = new PostProcessingRenderPass("zoomBlur", zoomBlur, panel);
    
    map<string, PostProcessingRenderPass*>::iterator it = passes.begin();
    for (; it != passes.end(); ++it) {
        menu->addToggle(it->first, &it->second->enabled);
    }
    
    passes["dof"]->widget->addSlider("aperture", &dofAperture, 0.0f, 1.0f);
    passes["dof"]->widget->addSlider("focus", &dofFocus, 0.95f, 1.0f);
    passes["dof"]->widget->addSlider("maxBlur", &dofMaxBlur, 0.0f, 1.0f);
    
    passes["kaleidoscope"]->widget->addSlider("segments", &kaleidoscopeSegments, 0.0f, 16.0f);
    
    passes["noiseWarp"]->widget->addSlider("amplitude", &noiseWarpAmplitude, 0.0f, 1.0f);
    passes["noiseWarp"]->widget->addSlider("frequency", &noiseWarpFrequency, 0.0f, 1.0f);
    passes["noiseWarp"]->widget->addSlider("speed", &noiseWarpSpeed, 0.0f, 1.0f);
    
    passes["pixelate"]->widget->addMultiSlider("resolution", &pixelateResolution, ofVec2f(0, 0), ofVec2f(100, 100));
    
    passes["edges"]->widget->addSlider("hue", &edgesHue, 0.0f, 1.0f);
    passes["edges"]->widget->addSlider("saturation", &edgesSaturation, 0.0f, 1.0f);
    
    passes["vTiltShift"]->widget->addSlider("V", &vTiltShiftV, 0.0f, 0.1f);
    passes["vTiltShift"]->widget->addSlider("R", &vTiltShiftR, 0.0f, 0.1f);
    
    passes["hTiltShift"]->widget->addSlider("H", &hTiltShiftH, 0.0f, 0.1f);
    passes["hTiltShift"]->widget->addSlider("R", &hTiltShiftR, 0.0f, 0.1f);
    
    passes["godRay"]->widget->addSlider("lightDirDOTviewDir", &godRayLightDirDOTviewDir, 0.0f, 1.0f);
    passes["godRay"]->widget->addMultiSlider("lightPoisitonOnScreen", &godRayLightPoisitonOnScreen, ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1));
    
    passes["toon"]->widget->addMultiSlider("specularColor", &toonSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    passes["toon"]->widget->addSlider("shinyness", &toonShinyness, 0.0f, 1.0f);
    passes["toon"]->widget->addSlider("level", &toonLevel, 0.0f, 1.0f);
    passes["toon"]->widget->addToggle("enableSpecular", &toonEnableSpecular);
    passes["toon"]->widget->addSlider("edgeThreshold", &toonEdgeThreshold, 0.0f, 1.0f);
    passes["toon"]->widget->addMultiSlider("diffuseColor", &toonDiffuseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    passes["toon"]->widget->addMultiSlider("ambientColor", &toonAmbientColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    
    passes["bleachBypass"]->widget->addSlider("bypassOpacity", &bleachBypassOpacity, 0.0f, 1.0f);
    
    passes["fakeSSS"]->widget->addSlider("attenuationOffset", &fakeSSSAttenuationOffset, 0.0f, 1.0f);
    passes["fakeSSS"]->widget->addMultiSlider("baseColor", &fakeSSSBaseColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    passes["fakeSSS"]->widget->addMultiSlider("extinctionCoefficient", &fakeSSSExtinctionCoefficient, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    passes["fakeSSS"]->widget->addMultiSlider("lightColor", &fakeSSSLightColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    passes["fakeSSS"]->widget->addMultiSlider("lightPosition", &fakeSSSLightPosition, ofPoint(0, 0), ofPoint(1, 1));
    passes["fakeSSS"]->widget->addSlider("materialThickness", &fakeSSSMaterialThickness, 0.0f, 1.0f);
    passes["fakeSSS"]->widget->addSlider("rimScale", &fakeSSSRimScale, 0.0f, 1.0f);
    passes["fakeSSS"]->widget->addSlider("specular", &fakeSSSSpecular, 0.0f, 1.0f);
    passes["fakeSSS"]->widget->addMultiSlider("specularColor", &fakeSSSSpecularColor, ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1));
    
    passes["limbDarken"]->widget->addSlider("brightness", &limbDarkenBrightness, 0.0f, 1.0f);
    passes["limbDarken"]->widget->addMultiSlider("endColor", &limbDarkenEndColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
    passes["limbDarken"]->widget->addSlider("radialScale", &limbDarkenRadialScale, 0.0f, 1.0f);
    passes["limbDarken"]->widget->addMultiSlider("startColor", &limbDarkenStartColor, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
    
    passes["shift"]->widget->addSlider("amount", &shiftAmount, 0.0f, 1.0f);
    passes["shift"]->widget->addSlider("angle", &shiftAngle, 0.0f, 1.0f);
    
    passes["ssao"]->widget->addSlider("cameraNear", &ssaoCameraNear, 0.0f, 1.0f);
    passes["ssao"]->widget->addSlider("cameraFar", &ssaoCameraFar, 0.0f, 1.0f);
    passes["ssao"]->widget->addSlider("fogNear", &ssaoFogNear, 0.0f, 1.0f);
    passes["ssao"]->widget->addSlider("fogFar", &ssaoFogFar, 0.0f, 1.0f);
    passes["ssao"]->widget->addToggle("fogEnabled", &ssaoFogEnabled);
    passes["ssao"]->widget->addToggle("onlyAO", &ssaoOnlyAO);
    passes["ssao"]->widget->addSlider("aoClamp", &ssaoAoClamp, 0.0f, 1.0f);
    passes["ssao"]->widget->addSlider("lumInfluence", &ssaoLumInfluence, 0.0f, 1.0f);
    
    passes["zoomBlur"]->widget->addSlider("centerX", &zoomBlurCenterX, 0.0f, 1.0f);
    passes["zoomBlur"]->widget->addSlider("centerY", &zoomBlurCenterY, 0.0f, 1.0f);
    passes["zoomBlur"]->widget->addSlider("exposure", &zoomBlurExposure, 0.0f, 1.0f);
    passes["zoomBlur"]->widget->addSlider("decay", &zoomBlurDecay, 0.0f, 1.0f);
    passes["zoomBlur"]->widget->addSlider("density", &zoomBlurDensity, 0.0f, 1.0f);
    passes["zoomBlur"]->widget->addSlider("weight", &zoomBlurWeight, 0.0f, 1.0f);
    passes["zoomBlur"]->widget->addSlider("clamp", &zoomBlurClamp, 0.0f, 1.0f);
    
}

void PostProcessingMod::toggleEffects(GuiMenuEventArgs & evt)
{
    passes[evt.toggle->getName()]->setEnabled(evt.value);
}

void PostProcessingMod::updateParameters()
{
    if (passes["alias"]->enabled) {
    }
    if (passes["bloom"]->enabled) {
    }
    if (passes["dof"]->enabled) {
        dof->setAperture(dofAperture);
        dof->setFocus(dofFocus);
        dof->setMaxBlur(dofMaxBlur);
    }
    if (passes["kaleidoscope"]->enabled) {
        kaleidoscope->setSegments(kaleidoscopeSegments);
    }
    if (passes["noiseWarp"]->enabled) {
        noiseWarp->setAmplitude(noiseWarpAmplitude);
        noiseWarp->setFrequency(noiseWarpFrequency);
        noiseWarp->setSpeed(noiseWarpSpeed);
    }
    if (passes["pixelate"]->enabled) {
        pixelate->setResolution(pixelateResolution);
    }
    if (passes["edges"]->enabled) {
        edges->setHue(edgesHue);
        edges->setSaturation(edgesSaturation);
    }
    if (passes["vTiltShift"]->enabled) {
        vTiltShift->setV(vTiltShiftV);
        vTiltShift->setR(vTiltShiftR);
    }
    if (passes["hTiltShift"]->enabled) {
        hTiltShift->setH(hTiltShiftH);
        hTiltShift->setR(hTiltShiftR);
    }
    if (passes["godRay"]->enabled) {
        godRay->setLightDirDOTviewDir(godRayLightDirDOTviewDir);
        godRay->setLightPositionOnScreen(godRayLightPoisitonOnScreen);
    }
    if (passes["toon"]->enabled) {
        toon->setSpecularColor(toonSpecularColor);
        toon->setShinyness(toonShinyness);
        toon->setLevel(toonLevel);
        toon->setEnableSpecular(toonEnableSpecular);
        toon->setEdgeThreshold(toonEdgeThreshold);
        toon->setDiffuseColor(toonDiffuseColor);
        toon->setAmbientColor(toonAmbientColor);
    }
    if (passes["bleachBypass"]->enabled) {
        bleachBypass->setOpacity(bleachBypassOpacity);
    }
    if (passes["convolve"]->enabled) {
    }
    if (passes["fakeSSS"]->enabled) {
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
    if (passes["limbDarken"]->enabled) {
        limbDarken->setBrightness(limbDarkenBrightness);
        limbDarken->setEndColor(limbDarkenEndColor);
        limbDarken->setRadialScale(limbDarkenRadialScale);
        limbDarken->setStartColor(limbDarkenStartColor);
    }
    if (passes["shift"]->enabled) {
        shift->setAmount(shiftAmount);
        shift->setAngle(shiftAngle);
    }
    if (passes["rimHighlight"]->enabled) {
    }
    if (passes["ssao"]->enabled) {
        ssao->setCameraNear(ssaoCameraNear);
        ssao->setCameraFar(ssaoCameraFar);
        ssao->setFogNear(ssaoFogNear);
        ssao->setFogFar(ssaoFogFar);
        ssao->setFogEnabled(ssaoFogEnabled);
        ssao->setOnlyAO(ssaoOnlyAO);
        ssao->setAoClamp(ssaoAoClamp);
        ssao->setLumInfluence(ssaoLumInfluence);
    }
    if (passes["zoomBlur"]->enabled) {
        zoomBlur->setCenterX(zoomBlurCenterX);
        zoomBlur->setCenterY(zoomBlurCenterY);
        zoomBlur->setExposure(zoomBlurExposure);
        zoomBlur->setDecay(zoomBlurDecay);
        zoomBlur->setDensity(zoomBlurDensity);
        zoomBlur->setWeight(zoomBlurWeight);
        zoomBlur->setClamp(zoomBlurClamp);
    }
}

void PostProcessingMod::render(ofFbo *fbo)
{
    updateParameters();
    
    post.begin();
    ofPushMatrix();
    ofTranslate(width, height);
    ofRotate(180);
    fbo->draw(0, 0);
    ofPopMatrix();
    post.end();
}
