#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Control.h"



class AudioUnitSynthParameter
{
public:
    string name;
    float min, max;
    AudioUnitSynthParameter(string name, float min, float max)
    {
        this->name = name;
        this->min = min;
        this->max = max;
    }
    string getName() {return name;}
};


class AudioUnitSynthParameterGroup
{
public:
    string name;
    map<string, AudioUnitSynthParameter*> parameters;
    AudioUnitSynthParameterGroup(string name) {
        this->name = name;
    }
    void addParameter(AudioUnitSynthParameter *parameter) {
        parameters[parameter->getName()] = parameter;
    }
};



class AudioUnitSynth
{
public:
    
    
    
    map<string, AudioUnitSynthParameterGroup*> groups;
    
    AudioUnitSynth(string name, OSType s1, OSType s2, OSType s3, GuiPanel & panel, ofxAudioUnitMixer * mixer, int idx)
    {
        
        this->mixer = mixer;
        this->idx = idx;
        synth = new ofxAudioUnit(s1, s2, s3);
        
        
        widget = panel.addWidget(name);
        widget->addButton("show ui", this, &AudioUnitSynth::eventShowUI);
        widget->addSlider("length", &length, 0.0f, 1.0f);
        widget->addSlider("volume", &volume, 0.0f, 1.0f, this, &AudioUnitSynth::eventSetVolume);
        widget->addSlider("pan", &pan, -1.0f, 1.0f, this, &AudioUnitSynth::eventSetPan);
        ((GuiToggle *) widget->getElement(name))->setTriggerAll(true);

        
        /*
        
        int clumpID = -1;
        string clumpName;
        
        vector<AudioUnitParameterInfo> parameters = synth->getParameterList();
        for (auto p : parameters)
        {
            AudioUnitSynthParameter *newParameter = new AudioUnitSynthParameter(p.name, p.minValue, p.maxValue);
            if (p.clumpID != clumpID) {
                clumpID = p.clumpID;
                clumpName = newParameter->getName();
                groups[clumpName] = new AudioUnitSynthParameterGroup(newParameter->getName());
                
                
            }
            
            groups[clumpName]->addParameter(newParameter);
            
        }
        
        
        GuiPanel *panel2 = new GuiPanel();//"parameters");
        
        
        map<string, AudioUnitSynthParameterGroup*>::iterator itg = groups.begin();
        while (itg != groups.end()) {
            
            GuiWidget *wGroup = panel2->addWidget(itg->second->name);
            
            map<string, AudioUnitSynthParameter*>::iterator itp = itg->second->parameters.begin();
            while (itp != itg->second->parameters.end()) {
                
                wGroup->addSlider(itp->second->name, new float(), 0.2f, 0.5f);
                
                ++itp;
                
            }
            ++itg;
        }
        
        */
        //panel.addWidget(panel2);
        
    }
    
    ~AudioUnitSynth() {
        delete synth;
        delete widget;
    }
    
    void eventSetVolume(GuiElementEventArgs &e)
    {
        mixer->setInputVolume(volume, idx);
    }
    
    void eventSetPan(GuiElementEventArgs &e)
    {
        mixer->setPan(pan, idx);
    }

    void eventShowUI(GuiElementEventArgs &e)
    {
        synth->showUI();
    }

    
    ofxAudioUnit *synth;
    ofxAudioUnitMixer * mixer;
    int idx;
    GuiWidget *widget;
    float position;
    float length;
    float volume;
    float pan;
};



class AudioUnitPlayerFile
{
public:
    
    AudioUnitPlayerFile(string name, string path, GuiPanel & panel, ofxAudioUnitMixer * mixer, int idx)
    {
        this->mixer = mixer;
        this->idx = idx;
        player = new ofxAudioUnitFilePlayer();
        player->setFile(path);
        player->connectTo(*mixer, idx);

        widget = panel.addWidget(name);
        widget->addToggle(name, this, &AudioUnitPlayerFile::eventPlay);
        widget->addSlider("position", &position, 0.0f, 1.0f);
        widget->addSlider("length", &length, 0.0f, 1.0f);
        widget->addSlider("volume", &volume, 0.0f, 1.0f, this, &AudioUnitPlayerFile::eventSetVolume);
        widget->addSlider("pan", &pan, -1.0f, 1.0f, this, &AudioUnitPlayerFile::eventSetPan);
        ((GuiToggle *) widget->getElement(name))->setTriggerAll(true);
    }
    
    ~AudioUnitPlayerFile() {
        player->stop();
        delete player;
        delete widget;
    }
    
    void eventPlay(GuiElementEventArgs &e)
    {
        e.value == 1.0 ? player->playAtSampleTime(player->getLength() * position) : player->stop();
    }
    
    void eventSetVolume(GuiElementEventArgs &e)
    {
        mixer->setInputVolume(volume, idx);
    }

    void eventSetPan(GuiElementEventArgs &e)
    {
        mixer->setPan(pan, idx);
    }

    
    ofxAudioUnitFilePlayer *player;
    ofxAudioUnitMixer * mixer;
    int idx;
    GuiWidget *widget;
    float position;
    float length;
    float volume;
    float pan;
};


class AudioUnitPlayer
{
public:
    ~AudioUnitPlayer();
    void setup();
    void clear();
    
    void loadClip(string name, string path);
    void loadSynth(string name, OSType s1, OSType s2, OSType s3);

    void blah() {
        mixer.connectTo(*synths["synth"]->synth).connectTo(output);

    }
    
private:
    
    void eventPlay(GuiElementEventArgs &e);
    
    //map<string, ofxAudioUnitFilePlayer*> players;
    map<string, AudioUnitPlayerFile*> players;
    map<string, AudioUnitSynth*> synths;
    
    ofxAudioUnitMixer mixer;
    ofxAudioUnit reverb;
	ofxAudioUnitOutput output;
    
    GuiPanel panel;
    

};