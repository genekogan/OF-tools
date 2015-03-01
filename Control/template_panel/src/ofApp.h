#pragma once

#include "ofMain.h"
#include "Control.h"


class ofApp : public ofBaseApp
{
    
public:
    void setup();
    void update();
    void draw();
    
    
    void buttonEvent(GuiElementEventArgs & e);
    void menuSelect(GuiElementEventArgs & e);
    void sliderEvent(GuiElementEventArgs & e);
    void colorEvent(GuiElementEventArgs & e);
    void panelToggleEvent(GuiElementEventArgs & e);
    void multiChoiceMenuSelect(GuiElementEventArgs & e);
    

    GuiPanel panel;
    GuiWidget widget;
    
    bool button, toggle;
    float floatSlider;
    int intSlider;
    double doubleSlider;
    ofVec4f vec4slider;
    float rangeLow, rangeHigh;
    ofVec2f vec2RangeLow, vec2RangeHigh;
    ofPoint padValue;
    ofFloatColor color, color2;
    ofVec2f vec2slider;
    
};
