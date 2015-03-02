#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Control.h"



class AudioUnitPlayer
{
public:
    ~AudioUnitPlayer();
    void setup();
    void clear();
    
    void loadClip(string name, string path);

    void blah() {
        panel.removeElement("particle");
    }
private:
    
    void eventPlay(GuiElementEventArgs &e);
    
    map<string, ofxAudioUnitFilePlayer*> players;

    
    ofxAudioUnitMixer mixer;
    ofxAudioUnit reverb;
	ofxAudioUnitOutput output;
    
    GuiPanel panel;
};