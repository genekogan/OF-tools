#pragma once

#include "ofMain.h"
#include "Control.h"





class ofApp : public ofBaseApp
{
    
public:
    void setup();
    void update();
    void draw();
    
    void menuSelect(GuiElementEventArgs & e);
    void multiChoiceMenuSelect(GuiElementEventArgs & e);

    GuiPanel panel;
    
    bool filled;
    float radius;
    float lineWidth;
    int resolution;
    ofVec2f position;
    ofFloatColor myColor;
    bool b1, b2, b3;
    
    string greeting;
    string city;
    
};
