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
    
    void toggleNotifier(GuiElementEventArgs & s);
    void buttonNotifier(GuiElementEventArgs & s);
    void sliderNotifier(GuiElementEventArgs & s);
    void menuNotifier(GuiElementEventArgs & s);
    void menuNotifierMultiChoice(GuiElementEventArgs & s);
    
    
    GuiButton *button;
    
    GuiToggle *toggle;
    GuiToggle *toggleNotify;
    
    GuiSlider<float> *floatSlider;
    GuiSlider<int> *intSlider;
    GuiSlider<double> *doubleSlider;
    
    GuiRangeSlider<float> *rangeFloatSlider;
    GuiRangeSlider<int> *rangeIntSlider;
    
    Gui2dPad *pad;
    
    GuiColor *colorPicker;
    
    GuiMenu *menu, *menuMultiChoice;

    
    
    bool myToggle;
    float floatValue;
    int intValue;
    double doubleValue;
    float rangeLow, rangeHigh;
    ofPoint padValue, padValue2;
    ofFloatColor bgColor;
};
