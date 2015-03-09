#include "AudioUnitInstrument.h"


AudioUnitInstrument::AudioUnitInstrumentParameter::AudioUnitInstrumentParameter(AudioUnitParameterInfo &parameter, GuiWidget *parentWidget, ofxAudioUnitSampler *instrument, int idx)
{
    this->instrument = instrument;
    this->idx = idx;
    parentWidget->addSlider(parameter.name, &parameter.defaultValue, parameter.minValue, parameter.maxValue, this, &AudioUnitInstrumentParameter::parameterChanged);
}

void AudioUnitInstrument::AudioUnitInstrumentParameter::parameterChanged(GuiElementEventArgs &e)
{
    instrument->setParameter(idx, 0, e.value);
}

AudioUnitInstrument::AudioUnitInstrument(string name, OSType s1, OSType s2, OSType s3, GuiPanel & mainPanel) : AudioUnitBase(name)
{
    instrument = new ofxAudioUnitSampler(s1, s2, s3);
    setupParameterGui();
}

void AudioUnitInstrument::setupParameterGui()
{
    panel.setName(name);
    panel.addButton("show ui", this, &AudioUnitInstrument::eventShowUI);
    
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

void AudioUnitInstrument::eventShowUI(GuiElementEventArgs &e)
{
    instrument->showUI();
}

AudioUnitInstrument::~AudioUnitInstrument()
{
    delete instrument;
}