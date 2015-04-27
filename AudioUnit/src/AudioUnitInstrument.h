#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "AudioUnitBase.h"
#include "Control.h"


struct AudioUnitInstrumentParameter
{
    ofxAudioUnitSampler *instrument;
    string name;
    int idx, clumpId;
    float *value;
    float min, max;
    float pmin, pmax;
    AudioUnitInstrumentParameter(AudioUnitParameterInfo &parameter, GuiWidget *parentWidget, ofxAudioUnitSampler *instrument, int idx);
    void setValue(float value);
    void parameterChanged(GuiSliderEventArgs<float> &e);
};


class AudioUnitInstrument : public AudioUnitBase
{
public:
    
    ////////////
    
    AUEventListenerRef auEventListener;
    static void audioUnitParameterChanged(void * context,
                                          void * object,
                                          const AudioUnitEvent * event,
                                          UInt64 hostTime,
                                          AudioUnitParameterValue value);
    
    
    ////////////
    
    
    
    AudioUnitInstrument(string name, OSType s1, OSType s2, OSType s3, GuiPanel & mainPanel);
    ~AudioUnitInstrument();
    
    ofxAudioUnit * getAudioUnit() {return instrument;}
    ofxAudioUnitSampler * getAudioUnitSampler() {return instrument;}
    vector<AudioUnitInstrumentParameter*> & getParameters() {return parameters;}
    
    void setMidiSequencer(MidiSequencer * midi_);
    void createMidiSequencer();
    

private:
    
    void midiEvent(MidiEventArgs & e);

    void eventShowUI(GuiButtonEventArgs &e);
    void eventShowMidi(GuiButtonEventArgs &e);
    void setupParameterGui();

    vector<AudioUnitInstrumentParameter*> parameters;
    ofxAudioUnitSampler *instrument;
    MidiSequencer *midi;
    bool hasMidiSequencer;
    bool midiVisible;
};

