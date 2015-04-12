#pragma once

#include "ofMain.h"
#include "Control.h"


class ofApp : public ofBaseApp
{
public:
    ~ofApp();
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key) {
       // widget.blah();
    }
    
    void toggleNotifier(GuiButtonEventArgs & s);
    void buttonNotifier(GuiButtonEventArgs & s);
    void sliderNotifier(GuiSliderEventArgs<double> & s);
    
    void vec3SliderNotifier(GuiMultiSliderEventArgs<ofVec3f> & s);
    
    void menuNotifier(GuiButtonEventArgs & s);
    void menuNotifierMultiChoice(GuiButtonEventArgs & s);
    
    
    GuiButton *button;
    
    GuiToggle *toggle;
    GuiToggle *toggleNotify;
    
    
    
    Gui2dPad *pad;
    ofPoint mypt;
    
    
    GuiWidget widget;
    
    
    GuiSlider<float> *floatSlider;
    GuiSlider<int> *intSlider;
    GuiSlider<double> *doubleSlider;
    
    GuiRangeSlider<float> *rangeFloatSlider;
    GuiRangeSlider<int> *rangeIntSlider;
    
    
    GuiMultiSlider<ofPoint> *vec3Slider;

    /*
    Gui2dPad *pad;
    
    GuiColor *colorPicker;
    
    GuiMenu *menu, *menuMultiChoice;
*/
    ofPoint vec3val;
    
    
    bool myToggle;
    float floatValue;
    int intValue;
    double doubleValue;
    float rangeLow, rangeHigh;
    ofPoint padValue, padValue2;
    ofFloatColor bgColor;
};
