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
    void setup(int width, int height);
    
    //void setLutLookup(vector<ofxLUT*> lut) {this->lut=lut;}
    
    void chooseFx(GuiElementEventArgs &e);
    void chooseLut(GuiElementEventArgs &e);

    
    GuiPanel & getControl()
    {
        return panel;
    }
    
    void render(ofFbo *fbo)
    {
        if (!pass) {
            lut[active]->setTexture(fbo->getTextureReference());
            lut[active]->update();
        }
        
//        fbo.begin();
        
        if (selection == "none") {
            if (pass)   fbo->draw(0, 0);
            else        lut[active]->draw();
        }
        else if (selection == "bloom"){
            if (pass)   bloom << fbo->getTextureReference();
            else        bloom << *lut[active];
            bloom.update();
            bloom.draw();
        }
        else if (selection == "gaussian_blur"){
            gaussianBlur.setRadius(radius);
            if (pass)   gaussianBlur << fbo->getTextureReference();
            else        gaussianBlur << *lut[active];
            gaussianBlur.update();
            gaussianBlur.draw();
        }
        else if (selection == "bokeh"){
            bokeh.setRadius(radius);
            if (pass)   bokeh << fbo->getTextureReference();
            else        bokeh << *lut[active];
            bokeh.update();
            bokeh.draw();
        }
        else if (selection == "glow"){
            glow.setRadius(radius);
            if (pass)   glow << fbo->getTextureReference();
            else        glow << *lut[active];
            glow.update();
            glow.draw();
        }
        else if (selection == "blur"){
            blur.setFade(fade);
            if (pass)   blur << fbo->getTextureReference();
            else        blur << *lut[active];
            blur.update();
            blur.draw();
        }
        else if (selection == "median"){
            if (pass)   median << fbo->getTextureReference();
            else        median << *lut[active];
            median.update();
            median.draw();
        }
        else if (selection == "oldtv"){
            if (pass)   oldtv << fbo->getTextureReference();
            else        oldtv << *lut[active];
            oldtv.update();
            oldtv.draw();
        }
        else if (selection == "inverse"){
            if (pass)   inverse << fbo->getTextureReference();
            else        inverse << *lut[active];
            inverse.update();
            inverse.draw();
        }
        else if (selection == "barrelChromaAb"){
            if (pass)   barrelChromaAb << fbo->getTextureReference();
            else        barrelChromaAb << *lut[active];
            barrelChromaAb.update();
            barrelChromaAb.draw();
        }
        else if (selection == "chromaAb"){
            if (pass)   chromaAb << fbo->getTextureReference();
            else        chromaAb << *lut[active];
            chromaAb.update();
            chromaAb.draw();
        }
        else if (selection == "chromaGlitch"){
            if (pass)   chromaGlitch << fbo->getTextureReference();
            else        chromaGlitch << *lut[active];
            chromaGlitch.update();
            chromaGlitch.draw();
        }
        else if (selection == "grayscale"){
            if (pass)   grayscale << fbo->getTextureReference();
            else        grayscale << *lut[active];
            grayscale.update();
            grayscale.draw();
        }
        
//        fbo.end();
    }
    
    string selection;
    ofDirectory dir;
    
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
