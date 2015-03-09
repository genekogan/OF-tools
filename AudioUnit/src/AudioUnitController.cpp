#include "AudioUnitController.h"

AudioUnitController::~AudioUnitController()
{
    clearInstruments();
}

AudioUnitController::AudioUnitController()
{
    panel.setName("audio");
    mixer.connectTo(output);
	output.start();
    
    panelVolume = panel.addWidget("volume");
    panelPan = panel.addWidget("pan");
}

void AudioUnitController::clearInstruments()
{
    map<string, AudioUnitBase*>::iterator its = instruments.begin();
    for (; its != instruments.end(); ++its) {
        delete its->second;
    }
    instruments.clear();
}

AudioUnitFilePlayer * AudioUnitController::loadFilePlayer(string name, string path)
{
    AudioUnitFilePlayer * player = new AudioUnitFilePlayer(name, path);
    instruments[name] = player;
    player->setGuiPosition(210 + (instruments.size() - 1) * 210, 0);
    return player;
}

AudioUnitInstrument * AudioUnitController::loadInstrument(string name, OSType s1, OSType s2, OSType s3)
{
    AudioUnitInstrument *instrument = new AudioUnitInstrument(name, s1, s2, s3, panel);
    instruments[name] = instrument;
    instrument->setGuiPosition(210 + (instruments.size() - 1) * 210, 0);
    return instrument;
}

void AudioUnitController::connectToMixer(AudioUnitBase * unit)
{
    int channel = 0;
    for (int channel = 0; channel < 8; channel++)
    {
        if (mixerInputs.count(channel) == 0)
        {
            unit->connectToMixer(&mixer, channel, panelVolume, panelPan);
            mixerInputs[channel] = unit;
            return;
        }
    }
}
