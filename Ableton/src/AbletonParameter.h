#pragma once

#include "Control.h"


class AbletonParameter
{
public:
    AbletonParameter(string name, int track, int device, int parameter, float value_, ofxOscSender * sender);
    ~AbletonParameter();
    
    void setRange(float min, float max);
    void setValue(float value_);
    
    void createSlider(GuiWidget *widget);
    void eventSlider(GuiSliderEventArgs<float> &evt);
    
    GuiSlider<float> * getControl() {return slider;}
    string getName() {return name;}
    
private:

    GuiSlider<float> *slider;
    ofxOscSender * sender;
    
    string name;
    int track;
    int device;
    int parameter;
    float min, max;
    float *value;
    
};
