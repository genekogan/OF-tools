#pragma once

#include "ofMain.h"
#include "Control.h"
#include "AudioUnitPlayer.h"


class ofApp : public ofBaseApp
{
    
public:
    void setup();
    void update();
    void draw();

    AudioUnitPlayer player;
};
