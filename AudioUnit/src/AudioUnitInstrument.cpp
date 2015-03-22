#include "AudioUnitInstrument.h"


AudioUnitInstrument::AudioUnitInstrumentParameter::AudioUnitInstrumentParameter(AudioUnitParameterInfo &parameter, GuiWidget *parentWidget, ofxAudioUnitSampler *instrument, int idx)
{
    this->instrument = instrument;
    this->idx = idx;
    float *value = new float(parameter.defaultValue);
    parentWidget->addSlider(parameter.name, value, parameter.minValue, parameter.maxValue, this, &AudioUnitInstrumentParameter::parameterChanged);
}

void AudioUnitInstrument::AudioUnitInstrumentParameter::parameterChanged(GuiElementEventArgs &e)
{
    instrument->setParameter(idx, 0, e.value);
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
}

void AudioUnitInstrument::setMidiSequencer(MidiSequencer * midi_)
{
    if (hasMidiSequencer) {
        delete midi;
    }
    this->midi = midi_;
    hasMidiSequencer = true;
    midi->setRectangle(panel.getRectangle().x + panel.getRectangle().width + panel.getMarginOuterX(), panel.getRectangle().y, MIDISEQUENCER_DEFAULT_WIDTH, MIDISEQUENCER_DEFAULT_HEIGHT);
    ofAddListener(midi->midiEvent, this, &AudioUnitInstrument::midiEvent);
    panel.addBoundWidget(midi);
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

void AudioUnitInstrument::eventShowUI(GuiElementEventArgs &e)
{
    instrument->showUI();
}

void AudioUnitInstrument::eventShowMidi(GuiElementEventArgs &e)
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