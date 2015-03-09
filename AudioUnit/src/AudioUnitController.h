#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Control.h"
#include "AudioUnitInstrument.h"
#include "AudioUnitFilePlayer.h"



class AudioUnitController
{
public:
    AudioUnitController();
    ~AudioUnitController();
    
    AudioUnitFilePlayer * loadFilePlayer(string name, string path);
    AudioUnitInstrument * loadInstrument(string name, OSType s1, OSType s2, OSType s3);
    
    AudioUnitBase * getInstrument(string name) {return instruments[name];}
    ofxAudioUnitMixer & getMixer() {return mixer;}
    
    void connectToMixer(AudioUnitBase * unit);
    
    GuiPanel & getPanel() {return panel;}

    void clearInstruments();

private:
    
    map<string, AudioUnitBase*> instruments;
    map<int, AudioUnitBase*> mixerInputs;
    
    ofxAudioUnitMixer mixer;
	ofxAudioUnitOutput output;
    
    GuiPanel panel;
    GuiWidget *panelVolume, *panelPan;
};