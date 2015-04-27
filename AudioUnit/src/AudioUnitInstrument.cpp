#include "AudioUnitInstrument.h"


AudioUnitInstrumentParameter::AudioUnitInstrumentParameter(AudioUnitParameterInfo &parameter, GuiWidget *parentWidget, ofxAudioUnitSampler *instrument, int idx)
{
    this->instrument = instrument;
    this->name = parameter.name;
    this->idx = idx;
    this->clumpId = parameter.clumpID;
    this->min = parameter.minValue;
    this->max = parameter.maxValue;
    pmin = min;
    pmax = max;
    value = new float(parameter.defaultValue);
    parentWidget->addSlider(parameter.name, value, min, max, this, &AudioUnitInstrumentParameter::parameterChanged);
}

void AudioUnitInstrumentParameter::setValue(float value)
{
    *this->value = value;
    instrument->setParameter(idx, 0, value);
}

void AudioUnitInstrumentParameter::parameterChanged(GuiSliderEventArgs<float> &e)
{
    setValue(e.value);
}

AudioUnitInstrument::AudioUnitInstrument(string name, OSType s1, OSType s2, OSType s3, GuiPanel & mainPanel) : AudioUnitBase(name)
{
    instrument = new ofxAudioUnitSampler(s1, s2, s3);
    hasMidiSequencer = false;
    midiVisible = false;
    setupParameterGui();
}

void AudioUnitInstrument::setupParameterGui()
{
    panel.setName(name);
    panel.addButton("show ui", this, &AudioUnitInstrument::eventShowUI);
    panel.addToggle("midi", &midiVisible, this, &AudioUnitInstrument::eventShowMidi);
    
    GuiWidget *wGroup;
    int clumpID = -1;
    vector<AudioUnitParameterInfo> synthParameters = instrument->getParameterList();
    for (int p = 0; p < synthParameters.size(); p++)
    {
        if (synthParameters[p].clumpID != clumpID)
        {
            if (clumpID != -1) {
                wGroup->setCollapsed(true);
            }
            clumpID = synthParameters[p].clumpID;
            wGroup = panel.addWidget(synthParameters[p].name);
        }
        parameters.push_back(new AudioUnitInstrumentParameter(synthParameters[p], wGroup, instrument, p));
    }
    wGroup->setCollapsed(true);
    
    
    
    /////////////

//    CFRunLoopRef runLoop = (CFRunLoopRef)GetCFRunLoopFromEventLoop(GetCurrentEventLoop());
    
    AUEventListenerCreate(&AudioUnitInstrument::audioUnitParameterChanged,
                          this,
                          CFRunLoopGetCurrent(),
                          kCFRunLoopDefaultMode,
                          0.005, // minimum callback interval (seconds)
                          0.005, // callback granularity (for rate-limiting)
                          &auEventListener);
    
    for (int p=0; p<parameters.size(); p++)
    {
        AudioUnitParameter param = {
            .mAudioUnit = *instrument,
            .mParameterID = p, //kLowPassParam_CutoffFrequency,
            .mScope = kAudioUnitScope_Global,
            .mElement = 0
        };        
        AUListenerAddParameter(auEventListener, this, &param);
    }
    /////////////
    

}

void AudioUnitInstrument::audioUnitParameterChanged(void *context, void *object, const AudioUnitEvent *event, UInt64 hostTime, AudioUnitParameterValue parameterValue)
{
    *((AudioUnitInstrument *) context)->parameters[event->mArgument.mParameter.mParameterID]->value = parameterValue;
}


void AudioUnitInstrument::setMidiSequencer(MidiSequencer * midi_)
{
    if (hasMidiSequencer) {
        delete midi;
    }
    this->midi = midi_;
    hasMidiSequencer = true;
    midi->setPosition(panel.getRectangle().x + panel.getRectangle().width + panel.getMarginX(), panel.getRectangle().y);
    midi->setSize(MIDISEQUENCER_DEFAULT_WIDTH, MIDISEQUENCER_DEFAULT_HEIGHT);
    ofAddListener(midi->midiEvent, this, &AudioUnitInstrument::midiEvent);
    panel.attachWidget(midi);
}

void AudioUnitInstrument::createMidiSequencer()
{
    setMidiSequencer(new MidiSequencer("midi-"+name));
}

void AudioUnitInstrument::midiEvent(MidiEventArgs & e)
{
    if (e.type == 1) {
        instrument->midiNoteOn(e.note, e.velocity);
    }
    else {
        instrument->midiNoteOff(e.note, e.velocity);
    }
}

void AudioUnitInstrument::eventShowUI(GuiButtonEventArgs &e)
{
    instrument->showUI();
}

void AudioUnitInstrument::eventShowMidi(GuiButtonEventArgs &e)
{
    if (midiVisible && !hasMidiSequencer) {
        createMidiSequencer();
    }
    midi->setAutoUpdate(midiVisible);
    midi->setAutoDraw(midiVisible);
}

AudioUnitInstrument::~AudioUnitInstrument()
{
    delete instrument;
}