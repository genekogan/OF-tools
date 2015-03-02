#include "AudioUnitPlayer.h"

AudioUnitPlayer::~AudioUnitPlayer()
{
    clear();
}

void AudioUnitPlayer::setup()
{
    panel.setName("audio");
    
	reverb = ofxAudioUnit('aufx', 'mrev', 'appl');
	//mixer.connectTo(reverb).connectTo(output);
    mixer.connectTo(output);
	output.start();
}

void AudioUnitPlayer::clear()
{
    map<string, ofxAudioUnitFilePlayer*>::iterator it = players.begin();
    for (; it != players.end(); ++it) {
        delete it->second;
    }
    players.clear();
}

void AudioUnitPlayer::loadClip(string name, string path)
{
    ofxAudioUnitFilePlayer *player = new ofxAudioUnitFilePlayer();
    players[name] = player;
    player->setFile(path);
    player->connectTo(mixer, players.size());
    
    
    panel.addToggle(name, this, &AudioUnitPlayer::eventPlay);
    
    ((GuiToggle *) panel.getElement(name))->setTriggerAll(true);
}

void AudioUnitPlayer::eventPlay(GuiElementEventArgs &e)
{
    e.value == 1.0 ? players[e.name]->play() : players[e.name]->stop();
}