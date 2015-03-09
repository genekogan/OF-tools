#include "AudioUnitBase.h"


AudioUnitBase::AudioUnitBase(string name)
{
    this->name = name;
}

void AudioUnitBase::connectToMixer(ofxAudioUnitMixer *mixer, int channel, GuiWidget * panelVolume, GuiWidget *panelPan)
{
    this->mixer = mixer;
    this->channel = channel;
    getAudioUnit()->connectTo(*mixer, channel);
    panelVolume->addSlider(name, &volume, 0.0f, 1.0f, this, &AudioUnitBase::eventSetVolume);
    panelPan->addSlider(name, &pan, -1.0f, 1.0f, this, &AudioUnitBase::eventSetPan);
}

void AudioUnitBase::setVolume(float volume)
{
    this->volume = volume;
    mixer->setInputVolume(volume, channel);
}

void AudioUnitBase::setPan(float pan)
{
    this->pan = pan;
    mixer->setPan(pan, channel);
}

void AudioUnitBase::eventSetVolume(GuiElementEventArgs &e)
{
    setVolume(volume);
}

void AudioUnitBase::eventSetPan(GuiElementEventArgs &e)
{
    setPan(pan);
}
