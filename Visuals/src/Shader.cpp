#include "Shader.h"

//http://patriciogonzalezvivo.com/2015/thebookofshaders/


//--------
void Shader::setup() {
    setName("Shader");
    width = 1280;
    height = 800;
}

//--------
void Shader::setShader(string vert, string frag) {
    vector<string> fragName = ofSplitString(frag, "/");
    shader.load(vert, frag);
    Scene::setup(width, height);
    setName(fragName[fragName.size()-1]);
}

//--------
void Shader::setTexture(ofFbo *fboTex){
    if (fboTex != NULL) {
        this->fboTex = fboTex;
        hasTexture = true;
    }
}

//--------
void Shader::update(){

}

//--------
void Shader::draw() {
    shader.begin();

    shader.setUniform2f("resolution", width, height);
    shader.setUniform1f("time", ofGetElapsedTimef());
    
    for (int i=0; i<shaderParameters.size(); i++) {
        shaderParameters[i]->update(&shader);
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
}

//--------
void Shader::addParameter(string name, float min, float max){
    float *var = new float();
    control.addParameter(name, var, min, max);
    *var = (min+max)*0.5;
    shaderParameters.push_back(new ShaderParameter<float>(name, var));
}

//--------
void Shader::addParameter(string name, ofVec2f min, ofVec2f max) {
    ofVec2f *var = new ofVec2f((min+max)*0.5);
    control.addParameter(name, var, min, max);
    shaderParameters.push_back(new ShaderParameter<ofVec2f>(name, var));
}

//--------
void Shader::addParameter(string name, ofVec3f min, ofVec3f max){
    ofVec3f *var = new ofVec3f((min+max)*0.5);
    control.addParameter(name, var, min, max);
    shaderParameters.push_back(new ShaderParameter<ofVec3f>(name, var));
}

//--------
void Shader::addParameter(string name, ofColor min, ofColor max){
    ofColor *var = new ofColor((min+max)*0.5);
    control.addColor(name, var);
    shaderParameters.push_back(new ShaderParameter<ofColor>(name, var));
}



/*  COLOR PRESETS */
//------------------

void Shader::setupBlobby(){
    setShader("shaders_color/standard.vert", "shaders_color/blobby.frag");
    addParameter("depth", 0, 2);
    addParameter("rate", 0, 2);
    addParameter("speed", 0, 2);
    addParameter("density", 0, 40);
    addParameter("zoomout", 0, 200);
}
void Shader::setupBits(){
    setShader("shaders_color/standard.vert", "shaders_color/bits.frag");
    addParameter("mult", ofVec2f(0, 0), ofVec2f(100, 10));
    addParameter("grid", ofVec2f(0, 0), ofVec2f(1, 1));
}
void Shader::setupBitsExperimental(){
    setShader("shaders_color/standard.vert", "shaders_color/bits_experimental.frag");
    addParameter("mult", ofVec3f(0, 0, 0), ofVec3f(100, 100, 100));
    addParameter("grid", ofVec2f(0, 0), ofVec2f(1, 1));
    addParameter("fieldsize", 1, 160);
    addParameter("offset", 0, 10);
    addParameter("speed", 0, 1);
    addParameter("combo", ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
}
void Shader::setupElectro(){
    setShader("shaders_color/standard.vert", "shaders_color/electro.frag");
    addParameter("centeredness", ofVec2f(-50, -50), ofVec2f(50, 50));
    addParameter("fieldsize", 0, 20);
    addParameter("rings", 5, 40);
    addParameter("complexity", 1, 60);
    addParameter("espeed", -2, 2);
}
void Shader::setupEye(){
    setShader("shaders_color/standard.vert", "shaders_color/eye.frag");
    addParameter("rad_eye", 0.1, 1.0);
    addParameter("color1", ofColor(0, 0, 0), ofColor(255, 255, 255));
    addParameter("color2", ofColor(0, 0, 0), ofColor(255, 255, 255));
    addParameter("mouse", ofVec2f(0, 0), ofVec2f(width, height));
}
void Shader::setupHerokuBubbles(){
    setShader("shaders_color/standard.vert", "shaders_color/herokububbles.frag");
    addParameter("fRadius", 0, 0.5);
    addParameter("numBubbles", 0, 400);
    addParameter("color1", ofColor(0, 0, 0), ofColor(255, 255, 255));
    addParameter("color2", ofColor(0, 0, 0), ofColor(255, 255, 255));
    addParameter("dir", 0, 100);
    addParameter("speed", ofVec2f(-1,-1), ofVec2f(1, 1));
}
void Shader::setupLandscape(){
    setShader("shaders_color/standard.vert", "shaders_color/landscape.frag");
    addParameter("dir", 1.5, 5);
}
void Shader::setupMonjori(){
    setShader("shaders_color/standard.vert", "shaders_color/monjori.frag");
    addParameter("graininess", 10, 100);
    addParameter("pace", 20, 80);
    addParameter("twist", 0, 100);
}
void Shader::setupNebula(){
    setShader("shaders_color/standard.vert", "shaders_color/nebula.frag");
    addParameter("starspeed", 0, 100);
}
void Shader::setupNoisy(){
    setShader("shaders_color/standard.vert", "shaders_color/noisy.frag");
    addParameter("noiseFactor", ofVec2f(0, 0), ofVec2f(10, 10));
    addParameter("noiseFactorTime", 0, 2);
    addParameter("color", ofColor(0, 0, 0), ofColor(255, 255, 255));
}
void Shader::setupIkeda(){
    setShader("shaders_color/standard.vert", "shaders_color/ikeda.frag");
}
void Shader::setupRain(){
    setShader("shaders_color/standard.vert", "shaders_color/rain.frag");
    addParameter("hue", 0, 0.1);
    addParameter("refresh", 0, 1);
    addParameter("slow", 0.1, 3);
    addParameter("gray", 0, 1);
}
void Shader::setupSinewave(){
    setShader("shaders_color/standard.vert", "shaders_color/sinewave.frag");
    addParameter("colorMult", ofVec2f(0.5, 0.5), ofVec2f(5.0, 2.0));
    addParameter("coeff", ofVec3f(10, 0, 1), ofVec3f(50, 90, 200));
}
void Shader::setupSinewaveExperimental(){
    setShader("shaders_color/standard.vert", "shaders_color/sinewave_experimental.frag");
    addParameter("color", ofColor(0, 0, 0), ofColor(255, 255, 255));
    addParameter("colorFreq", ofVec3f(0, 0, 0), ofVec3f(20, 20, 20));
    addParameter("coeff1", ofVec3f(1, 1, 1), ofVec3f(50, 90, 200));
    addParameter("coeff2", ofVec3f(1, 1, 1), ofVec3f(50, 90, 200));
}
void Shader::setupWave(){
    setShader("shaders_color/standard.vert", "shaders_color/wave.frag");
    addParameter("r", ofVec2f(0, 0), ofVec2f(20, 10));
    addParameter("t", ofVec2f(0, 0), ofVec2f(20, 10));
    addParameter("bright", 0, 2);
    addParameter("mode", ofVec2f(0, 0), ofVec2f(1, 1));
    addParameter("multiplicity", -0.9, 1);
    addParameter("fieldsize", 0, 20);
}
void Shader::setupFractalScope(){
    setShader("shaders_color/standard.vert", "shaders_color/fractal_scope.frag");
    addParameter("angle", 0, PI);
    addParameter("zoom", 0, 1);
    addParameter("iterations", 1, 100);
}
void Shader::setupFractalFlower(){
    setShader("shaders_color/standard.vert", "shaders_color/fractal_flower.frag");
    addParameter("mouse", ofVec2f(0, 0), ofVec2f(1, 1));
    addParameter("iter", 1, 8);
}
void Shader::setupCurtains(){
    setShader("shaders_color/standard.vert", "shaders_color/curtains.frag");
    addParameter("zoom", 0, 1);
    addParameter("shift", 0, 2*TWO_PI);
    addParameter("speed", 0, 1);
    addParameter("fall", 0, 100);
    addParameter("rows", 1, 8);
    addParameter("color", ofColor(0, 0, 0), ofColor(255, 255, 255));
}



/*  TEXTURE PRESETS */
//--------------------

void Shader::setupBrCoSa(){
    setShader("shaders_texture/standard.vert", "shaders_texture/brcosa.frag");
    addParameter("brightness", 0, 2);
    addParameter("saturation", -3, 5);
    addParameter("contrast", -3, 5);
}
void Shader::setupPixelate(){
    setShader("shaders_texture/standard.vert", "shaders_texture/pixelate.frag");
    addParameter("pixelSize", ofVec2f(1, 1), ofVec2f(100, 100));
}
void Shader::setupBilateralFilter(){
	setShader("shaders_texture/standard.vert", "shaders_texture/bilateral_filter.frag");
	addParameter("sigma", 0, 2);
}
void Shader::setupBlur(){
	setShader("shaders_texture/standard.vert", "shaders_texture/blur.frag");
	addParameter("sigma", 0, 10);
	addParameter("blurSize", 0, 30);
	addParameter("texOffset", ofVec2f(0.5, 0.5), ofVec2f(3, 3));
}
void Shader::setupChannels(){
	setShader("shaders_texture/standard.vert", "shaders_texture/channels.frag");
    addParameter("biasx", ofVec3f(-40, -40, -40), ofVec3f(40, 40, 40));
    addParameter("biasy", ofVec3f(-40, -40, -40), ofVec3f(40, 40, 40));
	addParameter("multx", ofVec3f(0.9, 0.9, 0.9), ofVec3f(1.1, 1.1, 1.1));
	addParameter("multy", ofVec3f(0.9, 0.9, 0.9), ofVec3f(1.1, 1.1, 1.1));
}
void Shader::setupDeform(){
	setShader("shaders_texture/standard.vert", "shaders_texture/deform.frag");
	addParameter("mouse", ofVec2f(0, 0), ofVec2f(1, 1));
	addParameter("turns", 2, 10);
    addParameter("speed", 0, 1);
}
void Shader::setupEdges(){
	setShader("shaders_texture/standard.vert", "shaders_texture/edges.frag");
}
void Shader::setupHalftoneCmyk(){
	setShader("shaders_texture/standard.vert", "shaders_texture/halftone_cmyk.frag");
	addParameter("density", 0, 1);
	addParameter("frequency", 0, 100);
}
void Shader::setupHalftone(){
	setShader("shaders_texture/standard.vert", "shaders_texture/halftone.frag");
	addParameter("pixelsPerRow", 2, 100);
    addParameter("sizeDot", 0, 1);
}
void Shader::setupHue(){
	setShader("shaders_texture/standard.vert", "shaders_texture/hue.frag");
	addParameter("hue", 0, TWO_PI);
}
void Shader::setupInvert(){
	setShader("shaders_texture/standard.vert", "shaders_texture/invert.frag");
}
void Shader::setupNeon(){
	setShader("shaders_texture/standard.vert", "shaders_texture/neon.frag");
	addParameter("brt", 0, 0.5);
	addParameter("rad", 0, 3);
}
void Shader::setupPatches(){
	setShader("shaders_texture/standard.vert", "shaders_texture/patches.frag");
	addParameter("row", 0, 1);
	addParameter("col", 0, 1);
    addParameter("speed", ofVec2f(0, 0), ofVec2f(0.1, 0.1));
}
void Shader::setupPixelRolls(){
    setShader("shaders_texture/standard.vert", "shaders_texture/pixelrolls.frag");
	addParameter("pixels", ofVec2f(0.001, 0.001), ofVec2f(1, 1));
	addParameter("margin", 0, 0.3);
	addParameter("rate", 0, 1);
	addParameter("dir", 0, 1);
	addParameter("gate", 0, 1);
}
void Shader::setupGrayscale(){
	setShader("shaders_texture/standard.vert", "shaders_texture/grayscale.frag");
}
void Shader::setupThreshold(){
	setShader("shaders_texture/standard.vert", "shaders_texture/threshold.frag");
    addParameter("threshold", 0, 1);
}
void Shader::setupWrap(){
	setShader("shaders_texture/standard.vert", "shaders_texture/wrap.frag");
	addParameter("radius", 0, 2);
	addParameter("radTwist", 0, 10);
    addParameter("angShift", 0, TWO_PI);
    addParameter("mode", 0, 1);
}
