#pragma once

#include "ofxPostGlitch.h"
#include "TimeFunction.h"
#include "Control.h"
#include "Shader.h"
#include "Modifier.h"


class PostGlitch : public Modifier
{
public:
    void setup(int width, int height);
    
    
    GuiPanel & getControl()
    {
        return panel;
    }
    
    void render(ofFbo *fbo)
    {
        post.setFbo(fbo);
        
        updateEffects();
        if (delTime != pDelTime)
            noiseChange();
        
        if (customParameters) {
            post.setShaderParameters((int) ofRandom(stepMin, stepMax),
                                     sv0.get(), sv1.get(), sv2.get(), sv3.get(),
                                     rand.get(), m0.get(), m1.get(), b0.get(), b1.get());
        } else {
            post.setShaderParameters();
        }
        
        post.generateFx();
        
        
        fbo->draw(0, 0);
        
        //fbo = *texLayer->getFbo();
    }
    
    
    void setDelTime(float delTime) {this->delTime = delTime;}
    
    bool convergence;
    bool glow;
    bool shaker;
    bool cutslider;
    bool twist;
    bool outline;
    bool noise;
    bool slitscan;
    bool swell;
    bool invert;
    bool crHighContrast;
    bool crBlueRaise;
    bool crRedRaise;
    bool crGreenRaise;
    bool crBlueInvert;
    bool crRedInvert;
    bool crGreenInvert;
    
    
    void updateEffects();
    void noiseChange();
    
    
    
    ofxPostGlitch post;
    
    bool customParameters;
    float stepMin, stepMax;
    float delTime, pDelTime;
    
    TimeFunction<float> sv0, sv1, sv2, sv3;
    TimeFunction<float> rand;
    TimeFunction<float> m0, m1;
    TimeFunction<float> b0, b1;
    
    GuiPanel panel;

};
