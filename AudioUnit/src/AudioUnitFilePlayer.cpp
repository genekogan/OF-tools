#include "AudioUnitFilePlayer.h"



AudioUnitFilePlayer::AudioUnitFilePlayer(string name, string path) : AudioUnitBase(name)
{
    player = new ofxAudioUnitFilePlayer();
    player->setFile(path);
    setupParameterGui();
}

void AudioUnitFilePlayer::setupParameterGui()
{
    panel.setName(name);
    panel.addToggle(name, this, &AudioUnitFilePlayer::eventPlay);
    panel.addSlider("position", &position, 0.0f, 1.0f);
    panel.addSlider("length", &length, 0.0f, 1.0f);
    panel.disableControlRow();
}

void AudioUnitFilePlayer::eventPlay(GuiButtonEventArgs &e)
{
    e.value == 1.0 ? player->playAtSampleTime(player->getLength() * position) : player->stop();
}

AudioUnitFilePlayer::~AudioUnitFilePlayer()
{
    player->stop();
    delete player;
}
