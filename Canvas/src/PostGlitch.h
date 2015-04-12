#pragma once

#include "ofxPostGlitch.h"
#include "TimeFunction.h"
#include "Control.h"
#include "Shader.h"
#include "Modifier.h"


class PostGlitch : public Modifier
{
public:
    PostGlitch() : Modifier()
    {
        type = "PostGlitch";
    }

    void setActive(bool active)
    {
        this->active = active;
        panel.setActive(active);
    }

    void setup(int width, int height);
    void render(ofFbo *fbo);
    
    void toggleEffects(GuiMenuEventArgs &evt);
    void setDelayTime(GuiSliderEventArgs<float> &evt);

    GuiPanel & getControl() {return panel;}

    ofxPostGlitch post;
    
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
    
    bool customParameters;
    float stepMin, stepMax;
    float delTime;
    
    TimeFunction<float> sv0, sv1, sv2, sv3;
    TimeFunction<float> rand;
    TimeFunction<float> m0, m1;
    TimeFunction<float> b0, b1;
    
    GuiPanel panel;
};
