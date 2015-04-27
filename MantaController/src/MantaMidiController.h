#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "MantaController.h"


class MantaMidiController : public MantaController
{
public:
    MantaMidiController();
    ~MantaMidiController();
    

    
    ofxMidiOut midiOut;
    int channel;
    
    unsigned int currentPgm;
    int note, velocity;
    int pan, bend, touch, polytouch;
    
    
    map<Parameter<float>*, int> midiMap;

};