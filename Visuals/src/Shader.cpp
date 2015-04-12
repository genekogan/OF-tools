#include "Shader.h"



Shader::ShaderData::ShaderData(string name, string vert, string frag, GuiPanel &control)
{
    this->name = name;
    this->vert = vert;
    this->frag = frag;
    widget = control.addWidget(name);
    widget->setCollapsible(true);
    widget->setActive(false);
}

void Shader::ShaderData::addParameter(string name, float min, float max)
{
    float *var = new float((min+max)*0.5);
    shaderParameters.push_back(new ShaderParameter<float>(name, var));
    widget->addSlider(name, var, min, max);
}

void Shader::ShaderData::addParameter(string name, ofVec2f min, ofVec2f max)
{
    ofVec2f *var = new ofVec2f((min+max)*0.5);
    shaderParameters.push_back(new ShaderParameter<ofVec2f>(name, var));
    widget->addMultiSlider(name, var, min, max);
}

void Shader::ShaderData::addParameter(string name, ofVec3f min, ofVec3f max)
{
    ofVec3f *var = new ofVec3f((min+max)*0.5);
    shaderParameters.push_back(new ShaderParameter<ofVec3f>(name, var));
    widget->addMultiSlider(name, var, min, max);
}

void Shader::ShaderData::addParameter(string name, ofFloatColor min, ofFloatColor max)
{
    ofFloatColor *var = new ofFloatColor((min+max)*0.5);
    shaderParameters.push_back(new ShaderParameter<ofFloatColor>(name, var));
    widget->addColor(name, var);
}

Shader::Shader(bool colorShader)
{
    setName("Shader");
    hasTexture = false;
    toSetNextShader = false;
    
    menuChoose = control.addMenu("choose shader", this, &Shader::chooseShader);
    
    if (colorShader) {
        enableColorShaderMenu();
    }
    else {
        enableTextureShaderMenu();
    }
    
    activeShader = NULL;
}

void Shader::setup(int width, int height, bool clearControls)
{
    Scene::setup(width, height, clearControls);
}

void Shader::enableTextureShaderMenu()
{
    setupBrCoSa();
    setupPixelate();
    setupBilateralFilter();
    setupBlur();
    setupChannels();
    setupDeform();
    setupEdges();
    setupHalftoneCmyk();
    setupHalftone();
    setupHue();
    setupInvert();
    setupNeon();
    setupPatches();
    setupPixelRolls();
    setupGrayscale();
    setupThreshold();
    setupWrap();
}

void Shader::enableColorShaderMenu()
{
    setupBlobby();
    setupBits();
    setupBitsExperimental();
    setupElectro();
    setupEye();
    setupHerokuBubbles();
    setupLandscape();
    setupMonjori();
    setupNebula();
    setupNoisy();
    setupIkeda();
    setupRain();
    setupSinewave();
    setupSinewaveExperimental();
    setupWave();
    setupFractalScope();
    setupFractalFlower();
    setupCurtains();
}

void Shader::chooseShader(GuiMenuEventArgs &e)
{
    toSetNextShader = true;
    nextShader = e.toggle->getName();
}

void Shader::selectShader(string shaderName)
{
    if (shaders.count(shaderName) != 0)
    {
        if (activeShader != NULL) {
            activeShader->widget->setActive(false);
        }
        else if (activeShader == shaders[shaderName]) {
            return;
        }
        
        activeShader = shaders[shaderName];
        activeShader->widget->setActive(true);

        shader.end();
        shader.unload();
        shader.load(activeShader->vert, activeShader->frag);
        
        //vector<string> fragName = ofSplitString(activeShader->frag, "/");
        //setName(fragName[fragName.size()-1]);
    }
    else {
        ofLog(OF_LOG_ERROR, "No shader named "+shaderName+" found.");
    }
}

void Shader::setTexture(ofFbo *fboTex)
{
    if (fboTex != NULL)
    {
        this->fboTex = fboTex;
        hasTexture = true;
    }
}

void Shader::update()
{
    if (toSetNextShader)
    {
        selectShader(nextShader);
        toSetNextShader = false;
    }
}

void Shader::draw(int x, int y)
{
    Scene::beginDraw(x, y);
 
    if (activeShader == NULL)
    {
        Scene::endDraw();
        return;
    }
        
    shader.begin();
    
    shader.setUniform2f("resolution", width, height);
    shader.setUniform1f("time", ofGetElapsedTimef());
    
    for (auto s : activeShader->shaderParameters)
    {
        s->update(&shader);
    }

    if (hasTexture) {
        fboTex->draw(0, 0, width, height);
    }
    else {
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);                     glVertex3f(0, 0, 0);
        glTexCoord2f(width, 0);                 glVertex3f(width, 0, 0);
        glTexCoord2f(width, height);            glVertex3f(width, height, 0);
        glTexCoord2f(0, height);                glVertex3f(0, height, 0);
        glEnd();
    }
    
    shader.end();
    
    Scene::endDraw();
}

Shader::ShaderData * Shader::createShader(string shaderName, string vert, string frag)
{
    ShaderData *newShader = new ShaderData(shaderName, vert, frag, control);
    shaders[newShader->name] = newShader;
    menuChoose->addToggle(newShader->name);
    if (shaders.size() == 1) {
        menuChoose->setToggle(shaderName, true);
    }
    return newShader;
}


//  COLOR PRESETS
//------------------

void Shader::setupBlobby() {
    ShaderData *newShader = createShader("Blobby", "shaders_color/standard.vert", "shaders_color/blobby.frag");
    newShader->addParameter("depth", 0, 2);
    newShader->addParameter("rate", 0, 2);
    newShader->addParameter("speed", 0, 2);
    newShader->addParameter("density", 0, 40);
    newShader->addParameter("zoomout", 0, 200);
}
void Shader::setupBits(){
    ShaderData *newShader = createShader("Bits", "shaders_color/standard.vert", "shaders_color/bits.frag");
    newShader->addParameter("mult", ofVec2f(0, 0), ofVec2f(100, 10));
    newShader->addParameter("grid", ofVec2f(0, 0), ofVec2f(1, 1));
}
void Shader::setupBitsExperimental(){
    ShaderData *newShader = createShader("BitsExperimental", "shaders_color/standard.vert", "shaders_color/bits_experimental.frag");
    newShader->addParameter("mult", ofVec3f(0, 0, 0), ofVec3f(100, 100, 100));
    newShader->addParameter("grid", ofVec2f(0, 0), ofVec2f(1, 1));
    newShader->addParameter("fieldsize", 1, 160);
    newShader->addParameter("offset", 0, 10);
    newShader->addParameter("speed", 0, 1);
    newShader->addParameter("combo", ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
}
void Shader::setupElectro(){
    ShaderData *newShader = createShader("Electro", "shaders_color/standard.vert", "shaders_color/electro.frag");
    newShader->addParameter("centeredness", ofVec2f(-50, -50), ofVec2f(50, 50));
    newShader->addParameter("fieldsize", 0, 20);
    newShader->addParameter("rings", 5, 40);
    newShader->addParameter("complexity", 1, 60);
    newShader->addParameter("espeed", -2, 2);
}
void Shader::setupEye(){
    ShaderData *newShader = createShader("Eye", "shaders_color/standard.vert", "shaders_color/eye.frag");
    newShader->addParameter("rad_eye", 0.1, 1.0);
    newShader->addParameter("color1", ofColor(0, 0, 0), ofColor(255, 255, 255));
    newShader->addParameter("color2", ofColor(0, 0, 0), ofColor(255, 255, 255));
    newShader->addParameter("mouse", ofVec2f(0, 0), ofVec2f(width, height));
}
void Shader::setupHerokuBubbles(){
    ShaderData *newShader = createShader("HerokuBubbles", "shaders_color/standard.vert", "shaders_color/herokububbles.frag");
    newShader->addParameter("fRadius", 0, 0.5);
    newShader->addParameter("numBubbles", 0, 400);
    newShader->addParameter("color1", ofColor(0, 0, 0), ofColor(255, 255, 255));
    newShader->addParameter("color2", ofColor(0, 0, 0), ofColor(255, 255, 255));
    newShader->addParameter("dir", 0, 100);
    newShader->addParameter("speed", ofVec2f(-1,-1), ofVec2f(1, 1));
}
void Shader::setupLandscape(){
    ShaderData *newShader = createShader("Landscape", "shaders_color/standard.vert", "shaders_color/landscape.frag");
    newShader->addParameter("dir", 1.5, 5);
}
void Shader::setupMonjori(){
    ShaderData *newShader = createShader("Monjori", "shaders_color/standard.vert", "shaders_color/monjori.frag");
    newShader->addParameter("graininess", 10, 100);
    newShader->addParameter("pace", 20, 80);
    newShader->addParameter("twist", 0, 100);
}
void Shader::setupNebula(){
    ShaderData *newShader = createShader("Nebula", "shaders_color/standard.vert", "shaders_color/nebula.frag");
    newShader->addParameter("starspeed", 0, 100);
}
void Shader::setupNoisy(){
    ShaderData *newShader = createShader("Noisy", "shaders_color/standard.vert", "shaders_color/noisy.frag");
    newShader->addParameter("noiseFactor", ofVec2f(0, 0), ofVec2f(10, 10));
    newShader->addParameter("noiseFactorTime", 0, 2);
    newShader->addParameter("color", ofColor(0, 0, 0), ofColor(255, 255, 255));
}
void Shader::setupIkeda(){
    ShaderData *newShader = createShader("Ikeda", "shaders_color/standard.vert", "shaders_color/ikeda.frag");
}
void Shader::setupRain(){
    ShaderData *newShader = createShader("Rain", "shaders_color/standard.vert", "shaders_color/rain.frag");
    newShader->addParameter("hue", 0, 0.1);
    newShader->addParameter("refresh", 0, 1);
    newShader->addParameter("slow", 0.1, 3);
    newShader->addParameter("gray", 0, 1);
}
void Shader::setupSinewave(){
    ShaderData *newShader = createShader("Sinewave", "shaders_color/standard.vert", "shaders_color/sinewave.frag");
    newShader->addParameter("colorMult", ofVec2f(0.5, 0.5), ofVec2f(5.0, 2.0));
    newShader->addParameter("coeff", ofVec3f(10, 0, 1), ofVec3f(50, 90, 200));
}
void Shader::setupSinewaveExperimental(){
    ShaderData *newShader = createShader("SinewaveExperimental", "shaders_color/standard.vert", "shaders_color/sinewave_experimental.frag");
    newShader->addParameter("color", ofColor(0, 0, 0), ofColor(255, 255, 255));
    newShader->addParameter("colorFreq", ofVec3f(0, 0, 0), ofVec3f(20, 20, 20));
    newShader->addParameter("coeff1", ofVec3f(1, 1, 1), ofVec3f(50, 90, 200));
    newShader->addParameter("coeff2", ofVec3f(1, 1, 1), ofVec3f(50, 90, 200));
}
void Shader::setupWave(){
    ShaderData *newShader = createShader("Wave", "shaders_color/standard.vert", "shaders_color/wave.frag");
    newShader->addParameter("r", ofVec2f(0, 0), ofVec2f(20, 10));
    newShader->addParameter("t", ofVec2f(0, 0), ofVec2f(20, 10));
    newShader->addParameter("bright", 0, 2);
    newShader->addParameter("mode", ofVec2f(0, 0), ofVec2f(1, 1));
    newShader->addParameter("multiplicity", -0.9, 1);
    newShader->addParameter("fieldsize", 0, 20);
}
void Shader::setupFractalScope(){
    ShaderData *newShader = createShader("FractalScope", "shaders_color/standard.vert", "shaders_color/fractal_scope.frag");
    newShader->addParameter("angle", 0, PI);
    newShader->addParameter("zoom", 0, 1);
    newShader->addParameter("iterations", 1, 100);
}
void Shader::setupFractalFlower(){
    ShaderData *newShader = createShader("FractalFlower", "shaders_color/standard.vert", "shaders_color/fractal_flower.frag");
    newShader->addParameter("mouse", ofVec2f(0, 0), ofVec2f(1, 1));
    newShader->addParameter("iter", 1, 8);
}
void Shader::setupCurtains(){
    ShaderData *newShader = createShader("Curtains", "shaders_color/standard.vert", "shaders_color/curtains.frag");
    newShader->addParameter("zoom", 0, 1);
    newShader->addParameter("shift", 0, 2.0 * TWO_PI);
    newShader->addParameter("speed", 0, 1);
    newShader->addParameter("fall", 0, 100);
    newShader->addParameter("rows", 1, 8);
    newShader->addParameter("color", ofColor(0, 0, 0), ofColor(255, 255, 255));
}



//  TEXTURE PRESETS
//------------------

void Shader::setupBrCoSa(){
    ShaderData *newShader = createShader("BrCoSa", "shaders_texture/standard.vert", "shaders_texture/brcosa.frag");
    newShader->addParameter("brightness", 0, 2);
    newShader->addParameter("saturation", -3, 5);
    newShader->addParameter("contrast", -3, 5);
}
void Shader::setupPixelate(){
    ShaderData *newShader = createShader("Pixelate", "shaders_texture/standard.vert", "shaders_texture/pixelate.frag");
    newShader->addParameter("pixelSize", ofVec2f(1, 1), ofVec2f(100, 100));
}
void Shader::setupBilateralFilter(){
	ShaderData *newShader = createShader("BilateralFilter", "shaders_texture/standard.vert", "shaders_texture/bilateral_filter.frag");
	newShader->addParameter("sigma", 0, 2);
}
void Shader::setupBlur(){
	ShaderData *newShader = createShader("Blur", "shaders_texture/standard.vert", "shaders_texture/blur.frag");
	newShader->addParameter("sigma", 0, 10);
	newShader->addParameter("blurSize", 0, 30);
	newShader->addParameter("texOffset", ofVec2f(0.5, 0.5), ofVec2f(3, 3));
}
void Shader::setupChannels(){
	ShaderData *newShader = createShader("Channels", "shaders_texture/standard.vert", "shaders_texture/channels.frag");
    newShader->addParameter("biasx", ofVec3f(-40, -40, -40), ofVec3f(40, 40, 40));
    newShader->addParameter("biasy", ofVec3f(-40, -40, -40), ofVec3f(40, 40, 40));
	newShader->addParameter("multx", ofVec3f(0.9, 0.9, 0.9), ofVec3f(1.1, 1.1, 1.1));
	newShader->addParameter("multy", ofVec3f(0.9, 0.9, 0.9), ofVec3f(1.1, 1.1, 1.1));
}
void Shader::setupDeform(){
	ShaderData *newShader = createShader("Deform", "shaders_texture/standard.vert", "shaders_texture/deform.frag");
	newShader->addParameter("mouse", ofVec2f(0, 0), ofVec2f(1, 1));
	newShader->addParameter("turns", 2, 10);
    newShader->addParameter("speed", 0, 1);
}
void Shader::setupEdges(){
	ShaderData *newShader = createShader("Edges", "shaders_texture/standard.vert", "shaders_texture/edges.frag");
}
void Shader::setupHalftoneCmyk(){
	ShaderData *newShader = createShader("HalftoneCMYK", "shaders_texture/standard.vert", "shaders_texture/halftone_cmyk.frag");
	newShader->addParameter("density", 0, 1);
	newShader->addParameter("frequency", 0, 100);
}
void Shader::setupHalftone(){
	ShaderData *newShader = createShader("Halftone", "shaders_texture/standard.vert", "shaders_texture/halftone.frag");
	newShader->addParameter("pixelsPerRow", 2, 100);
    newShader->addParameter("sizeDot", 0, 1);
}
void Shader::setupHue(){
	ShaderData *newShader = createShader("Hue", "shaders_texture/standard.vert", "shaders_texture/hue.frag");
	newShader->addParameter("hue", 0, TWO_PI);
}
void Shader::setupInvert(){
	ShaderData *newShader = createShader("Invert", "shaders_texture/standard.vert", "shaders_texture/invert.frag");
}
void Shader::setupNeon(){
	ShaderData *newShader = createShader("Neon", "shaders_texture/standard.vert", "shaders_texture/neon.frag");
	newShader->addParameter("brt", 0, 0.5);
	newShader->addParameter("rad", 0, 3);
}
void Shader::setupPatches(){
	ShaderData *newShader = createShader("Patches", "shaders_texture/standard.vert", "shaders_texture/patches.frag");
	newShader->addParameter("row", 0, 1);
	newShader->addParameter("col", 0, 1);
    newShader->addParameter("speed", ofVec2f(0, 0), ofVec2f(0.1, 0.1));
}
void Shader::setupPixelRolls(){
    ShaderData *newShader = createShader("PixelRolls", "shaders_texture/standard.vert", "shaders_texture/pixelrolls.frag");
	newShader->addParameter("pixels", ofVec2f(0.001, 0.001), ofVec2f(1, 1));
	newShader->addParameter("margin", 0, 0.3);
	newShader->addParameter("rate", 0, 1);
	newShader->addParameter("dir", 0, 1);
	newShader->addParameter("gate", 0, 1);
}
void Shader::setupGrayscale(){
	ShaderData *newShader = createShader("Grayscale", "shaders_texture/standard.vert", "shaders_texture/grayscale.frag");
}
void Shader::setupThreshold(){
	ShaderData *newShader = createShader("Threshold", "shaders_texture/standard.vert", "shaders_texture/threshold.frag");
    newShader->addParameter("threshold", 0, 1);
}
void Shader::setupWrap(){
	ShaderData *newShader = createShader("Wrap", "shaders_texture/standard.vert", "shaders_texture/wrap.frag");
	newShader->addParameter("radius", 0, 2);
	newShader->addParameter("radTwist", 0, 10);
    newShader->addParameter("angShift", 0, TWO_PI);
    newShader->addParameter("mode", 0, 1);
}