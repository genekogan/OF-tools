#pragma once

#include "ofMain.h"
#include "Control.h"
#include "AbletonParameter.h"


class AbletonDevice
{
public:
    AbletonDevice(string name, int track, int device, ofxOscSender * sender);
    ~AbletonDevice();
    
    void addParameter(int index, string name, float value);
    void clear();

    void getParameterValues(ofxOscMessage &m);
    void getParameterRanges(ofxOscMessage &m);

    map<int, AbletonParameter*> & getParameters() {return parameters;}
    GuiWidget * getControl() {return control;}
    string getName() {return name;}
    
private:

    map<int, AbletonParameter*> parameters;
    string name;
    int track;
    int device;
    ofxOscSender * sender;
    GuiWidget *control;
};


