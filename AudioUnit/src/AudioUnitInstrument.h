#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "AudioUnitBase.h"
#include "Control.h"



class AudioUnitInstrument : public AudioUnitBase
{
public:
    AudioUnitInstrument(string name, OSType s1, OSType s2, OSType s3, GuiPanel & mainPanel);
    ~AudioUnitInstrument();
    
    ofxAudioUnit * getAudioUnit() {return instrument;}
    ofxAudioUnitSampler * getAudioUnitSampler() {return instrument;}
    
    void setMidiSequencer(MidiSequencer * midi_);
    void createMidiSequencer();
    
private:
    
    struct AudioUnitInstrumentParameter
    {
        ofxAudioUnitSampler *instrument;
        int idx;
        AudioUnitInstrumentParameter(AudioUnitParameterInfo &parameter, GuiWidget *parentWidget, ofxAudioUnitSampler *instrument, int idx);
        void parameterChanged(GuiElementEventArgs &e);
    };
    
    void midiEvent(MidiEventArgs & e);

    void eventShowUI(GuiElementEventArgs &e);
    void eventShowMidi(GuiElementEventArgs &e);
    void setupParameterGui();

    vector<AudioUnitInstrumentParameter*> parameters;
    ofxAudioUnitSampler *instrument;
    MidiSequencer *midi;
    bool hasMidiSequencer;
    bool midiVisible;
};

