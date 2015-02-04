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
    
    Sequencer seq;
};
