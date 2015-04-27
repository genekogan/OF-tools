#pragma once

#include "ofMain.h"
#include "Control.h"
#include "AudioUnits.h"


class ofApp : public ofBaseApp
{
    
public:
    void setup();
    void update();
    void draw();

    AudioUnitController au;
    
    //MidiSequencer *midi;
};
