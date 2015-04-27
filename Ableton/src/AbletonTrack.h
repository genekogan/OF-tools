#pragma once

#include "ofMain.h"
#include "Control.h"
#include "AbletonDevice.h"



// TODO
// base class AbletonTrack
// -> AbletonAudioTrack
// -> AbletonMidiTrack
// -> AbletonSend


class AbletonTrack
{
public:
    AbletonTrack(string name, int track, int numScenes, ofxOscSender * sender);
    ~AbletonTrack();

    void selectClip(int clip);
    void stop();
    
    void addDevice(int device, AbletonDevice * newDevice);
    
    void clear();

    map<int, AbletonDevice*> & getDevices() { return devices;}
    GuiWidget * getControl() {return control;}
    string getName() {return name;}

private:
    
    void eventSelectClip(GuiMenuEventArgs &evt) {selectClip(evt.index);}
    void eventArm(GuiButtonEventArgs &evt) {setArmed(arm);}
    void eventMute(GuiButtonEventArgs &evt) {setMute(mute);}
    void eventSolo(GuiButtonEventArgs &evt) {setSolo(solo);}
    void eventVolume(GuiSliderEventArgs<float> &evt) {setVolume(volume);}
    void eventPan(GuiSliderEventArgs<float> &evt) {setPan(pan);}
    void eventSend(GuiSliderEventArgs<float> &evt) {setSend(send);}
    void eventPitchCoarse(GuiSliderEventArgs<int> &evt) {setPitch(pitchCoarse, pitchFine);}
    void eventPitchFine(GuiSliderEventArgs<int> &evt) {setPitch(pitchCoarse, pitchFine);}
    void eventStop(GuiButtonEventArgs &evt) {stop();}
    
    void setArmed(bool arm);
    void setMute(bool mute);
    void setSolo(bool solo);
    void setVolume(float volume);
    void setPan(float pan);
    void setSend(float send);
    void setPitch(int pitchCoarse, int pitchFine);

    map<int, AbletonDevice*> devices;
    string name;
    int track;
    int numScenes;
    
    bool arm;
    bool mute;
    bool solo;
    
    float volume;
    float pan;
    float send;
    
    int pitchCoarse;
    int pitchFine;
    
    GuiPanel *control;
    ofxOscSender * sender;
};

