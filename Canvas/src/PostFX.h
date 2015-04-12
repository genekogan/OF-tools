#pragma once

#include "ofxBloom.h"
#include "ofxGaussianBlur.h"
#include "ofxBokeh.h"
#include "ofxBlur.h"
#include "ofxLUT.h"
#include "ofxMultiTexture.h"
#include "ofxGlow.h"
#include "ofxMedian.h"
#include "ofxInverse.h"
#include "ofxOldTv.h"
#include "ofxBarrelChromaAb.h"
#include "ofxChromaAb.h"
#include "ofxChromaGlitch.h"
#include "ofxGrayscale.h"

#include "TimeFunction.h"
#include "Control.h"
#include "Shader.h"
#include "Modifier.h"


class PostFX : public Modifier
{
public:
    PostFX() : Modifier()
    {
        type = "PostFX";
    }
    
    void setActive(bool active)
    {
        this->active = active;
        panel.setActive(active);
    }

    void setup(int width, int height);
    
    void loadLutLookup();
    void setLutLookup(vector<ofxLUT*> lut);

    void render(ofFbo *fbo);

    GuiPanel & getControl() {return panel;}

private:
    
    void chooseFx(GuiMenuEventArgs &e);
    void chooseLut(GuiMenuEventArgs &e);
    
    string selection;
    
    vector<string> lutChoices;
    vector<ofxLUT*> lut;
    int active;
    
    ofxBloom bloom;
    ofxGaussianBlur gaussianBlur;
    ofxBokeh bokeh;
    ofxGlow glow;
    ofxBlur blur;
    ofxMedian median;
    ofxOldTv oldtv;
    ofxInverse inverse;
    ofxBarrelChromaAb barrelChromaAb;
    ofxChromaAb chromaAb;
    ofxChromaGlitch chromaGlitch;
    ofxGrayscale grayscale;
    
    float radius, fade;
    bool pass;
    
    GuiPanel panel;    
};
