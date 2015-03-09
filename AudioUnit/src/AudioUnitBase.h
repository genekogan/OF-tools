#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "Control.h"


class AudioUnitBase
{
public:
    AudioUnitBase(string name);

    void connectToMixer(ofxAudioUnitMixer *mixer, int channel, GuiWidget * panelVolume, GuiWidget *panelPan);

    void setVolume(float volume);
    void setPan(float pan);

    float & getVolume() {return volume;}
    float & getPan() {return pan;}
    
    virtual ofxAudioUnit * getAudioUnit() { }

    GuiPanel & getGuiPanel() {return panel;}
    void setGuiPosition(int x, int y) {panel.setPosition(x, y);}

protected:

    virtual void setupParameterGui() { }
    
    void eventSetVolume(GuiElementEventArgs &e);
    void eventSetPan(GuiElementEventArgs &e);

    string name;
    float volume;
    float pan;
    int channel;
    
    ofxAudioUnitMixer *mixer;
    
    GuiPanel panel;
};