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
    
    void mousePressed(int mouseX, int mouseY, int button) {
        //cout << "pressed mouse at t " << ofGetElapsedTimef() << endl;
    }

    AudioUnitController au;
    
    
    MidiSequencer *midi;
};
