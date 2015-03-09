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
    
    
    
    
    
    MidiSequencer *midi;
    void setMidiSequencer(MidiSequencer * midi) {
        this->midi = midi;
        ofAddListener(midi->midiEvent, this, &AudioUnitInstrument::midiEvent);
    }
    
    
    
    void midiEvent(GuiMidiEventArgs & e) {
        cout << e.type << " "<< e.note << " " << e.velocity << endl;
        
        if (e.type == 1)
            instrument->midiNoteOn(4*e.note + 60, 120);
        else
            instrument->midiNoteOff(4*e.note + 60, 120);
    }
    
    
    
private:
    
    struct AudioUnitInstrumentParameter
    {
        AudioUnitInstrumentParameter(AudioUnitParameterInfo &parameter, GuiWidget *parentWidget, ofxAudioUnitSampler *instrument, int idx);
        void parameterChanged(GuiElementEventArgs &e);
    private:
        int idx;
        ofxAudioUnitSampler *instrument;
    };

    void eventShowUI(GuiElementEventArgs &e);
    void setupParameterGui();

    vector<AudioUnitInstrumentParameter*> parameters;
    ofxAudioUnitSampler *instrument;
};



