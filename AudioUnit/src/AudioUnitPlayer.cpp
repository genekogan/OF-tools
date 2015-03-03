#include "AudioUnitPlayer.h"

AudioUnitPlayer::~AudioUnitPlayer()
{
    clear();
}

void AudioUnitPlayer::setup()
{
    panel.setName("audio");
    
	//reverb = ofxAudioUnit('aufx', 'mrev', 'appl');
	//mixer.connectTo(reverb).connectTo(output);
    mixer.connectTo(output);
	output.start();
}

void AudioUnitPlayer::clear()
{
    map<string, AudioUnitPlayerFile*>::iterator it = players.begin();
    for (; it != players.end(); ++it) {
        delete it->second;
    }
    players.clear();

    map<string, AudioUnitSynth*>::iterator its = synths.begin();
    for (; its != synths.end(); ++its) {
        delete its->second;
    }
    synths.clear();

}

void AudioUnitPlayer::loadClip(string name, string path)
{
    players[name] = new AudioUnitPlayerFile(name, path, panel, &mixer, players.size());
}

void AudioUnitPlayer::loadSynth(string name, OSType s1, OSType s2, OSType s3)
{
    synths[name] = new AudioUnitSynth(name, s1, s2, s3, panel, &mixer, players.size());
}
