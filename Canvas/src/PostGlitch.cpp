//
//  PostGlitch.cpp
//  template_simple
//
//  Created by Eugene Kogan on 3/23/15.
//
//

#include "PostGlitch.h"


void PostGlitch::setup(int width, int height)
{
    Modifier::setup(width, height);
    
    
    panel.setName("PostGlitch");
    
    panel.addToggle("convergence", &convergence);
    panel.addToggle("glow", &glow);
    panel.addToggle("shaker", &shaker);
    panel.addToggle("cutslider", &cutslider);
    panel.addToggle("twist", &twist);
    panel.addToggle("outline", &outline);
    panel.addToggle("noise", &noise);
    panel.addToggle("slitscan", &slitscan);
    panel.addToggle("swell", &swell);
    panel.addToggle("invert", &invert);
    panel.addToggle("crHighContrast", &crHighContrast);
    panel.addToggle("crBlueRaise", &crBlueRaise);
    panel.addToggle("crRedRaise", &crRedRaise);
    panel.addToggle("crGreenRaise", &crGreenRaise);
    panel.addToggle("crBlueInvert", &crBlueInvert);
    panel.addToggle("crRedInvert", &crRedInvert);
    panel.addToggle("crGreenInvert", &crGreenInvert);
    
    panel.addToggle("customParameters", &customParameters);
    panel.addSlider("delTime", &delTime, 0.0f, 2.0f);
    panel.addSlider("stepMin", &stepMin, 2.0f, 100.0f);
    panel.addSlider("stepMax", &stepMax, 10.0f, 160.0f);
    
    convergence = false;
    glow = false;
    shaker = false;
    cutslider = false;
    twist = false;
    outline = false;
    noise = false;
    slitscan = false;
    swell = false;
    invert = false;
    crHighContrast = false;
    crBlueRaise = false;
    crRedRaise = false;
    crGreenRaise = false;
    crBlueInvert = false;
    crRedInvert = false;
    crGreenInvert = false;
    
    customParameters = true;
    delTime = 1.2;
    stepMin = 4;
    stepMax = 16;
    
    sv0.setFunctionNoise(-25, 125, 0.1);
    sv1.setFunctionNoise(-25, 125, 0.1);
    sv2.setFunctionNoise(-25, 125, 0.1);
    sv3.setFunctionNoise(-25, 125, 0.1);
    rand.setFunctionNoise(-0.2, 1.2, 0.1);
    m0.setFunctionNoise(-150, ofGetWidth()+150, 0.1);
    m1.setFunctionNoise(-150, ofGetHeight()+150, 0.1);
    b0.setFunctionNoise(-0.5, 3.5, 0.1);
    b1.setFunctionNoise(-0.5, 3.5, 0.1);
    
}

void PostGlitch::updateEffects()
{
    post.setFx(OFXPOSTGLITCH_CONVERGENCE, convergence);
    post.setFx(OFXPOSTGLITCH_GLOW, glow);
    post.setFx(OFXPOSTGLITCH_SHAKER, shaker);
    post.setFx(OFXPOSTGLITCH_CUTSLIDER, cutslider);
    post.setFx(OFXPOSTGLITCH_TWIST, twist);
    post.setFx(OFXPOSTGLITCH_OUTLINE, outline);
    post.setFx(OFXPOSTGLITCH_NOISE, noise);
    post.setFx(OFXPOSTGLITCH_SLITSCAN, slitscan);
    post.setFx(OFXPOSTGLITCH_SWELL, swell);
    post.setFx(OFXPOSTGLITCH_INVERT, invert);
    post.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, crHighContrast);
    post.setFx(OFXPOSTGLITCH_CR_BLUERAISE, crBlueRaise);
    post.setFx(OFXPOSTGLITCH_CR_REDRAISE, crRedRaise);
    post.setFx(OFXPOSTGLITCH_CR_GREENRAISE, crGreenRaise);
    post.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, crBlueInvert);
    post.setFx(OFXPOSTGLITCH_CR_REDINVERT, crRedInvert);
    post.setFx(OFXPOSTGLITCH_CR_GREENINVERT, crGreenInvert);
}

void PostGlitch::noiseChange() {
    sv0.setDelTime(delTime);
    sv1.setDelTime(delTime);
    sv2.setDelTime(delTime);
    sv3.setDelTime(delTime);
    rand.setDelTime(delTime);
    m0.setDelTime(delTime);
    m1.setDelTime(delTime);
    b0.setDelTime(delTime);
    b1.setDelTime(delTime);
    pDelTime = delTime;
}

