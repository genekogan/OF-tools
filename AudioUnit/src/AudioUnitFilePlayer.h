#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "AudioUnitBase.h"
#include "Control.h"


class AudioUnitFilePlayer : public AudioUnitBase
{
public:
    AudioUnitFilePlayer(string name, string path);
    ~AudioUnitFilePlayer();
    
    ofxAudioUnit * getAudioUnit() {return player;}

private:
    
    void setupParameterGui();
    void eventPlay(GuiElementEventArgs &e);
    
    ofxAudioUnitFilePlayer *player;
    float position;
    float length;
};

