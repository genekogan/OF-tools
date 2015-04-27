#pragma once

#include "Control.h"

#include "AbletonParameter.h"
#include "AbletonDevice.h"
#include "AbletonTrack.h"


#define ABLETON_OSC_HOST_DEFAULT "localhost"
#define ABLETON_OSC_PORT_OUT 9000
#define ABLETON_OSC_PORT_IN 9001


class Ableton
{
public:
    Ableton();
    
    void setAutoUpdate(bool autoUpdate);
    void setAutoDraw(bool autoDraw);
    
    void setup(string abletonOscHost = ABLETON_OSC_HOST_DEFAULT);
    void update();
    
    void scanLiveSet();
    void clear();
    void refresh();
    void setConnected(bool connected);
    
private:
    
    void requestNumScenes();
    void requestNumTracks();
    void requestDeviceList(int track);
    void requestDeviceParameters(int track, int device);
    void requestDeviceParametersRange(int track, int device);
    
    void getDeviceList(ofxOscMessage &m);
    void getDeviceParameters(ofxOscMessage &m);
    void getDeviceParametersRange(ofxOscMessage &m);
    void getParameterUpdate(ofxOscMessage &m);
    void getNumScenes(ofxOscMessage &m);
    void getNumTracks(ofxOscMessage &m);

    void selectScene(int scene);
    void prevCue();
    void nextCue();
    void play();
    void playContinue();
    void playSelection();
    void stop();
    
    void setTempo(float tempo);
    void setTime(float time);
    void setOverdub(bool overdub);
    void setVolume(float volume);
    void setPan(float pan);

    void eventRefresh(GuiButtonEventArgs &evt) {refresh();}
    void eventSelectScene(GuiMenuEventArgs &evt);
    void eventSetTempo(GuiSliderEventArgs<float> &evt) {setTempo(tempo);}
    void eventSetTime(GuiSliderEventArgs<float> &evt) {setTime(time);}
    void eventSetOverdub(GuiButtonEventArgs &evt) {setOverdub(overdub);}
    void eventSetVolume(GuiSliderEventArgs<float> &evt) {setVolume(volume);}
    void eventSetPan(GuiSliderEventArgs<float> &evt) {setPan(pan);}
    void eventSetConnected(GuiButtonEventArgs &evt) {setConnected(connected);}
    
    void eventPrevCue(GuiButtonEventArgs &evt) {prevCue();}
    void eventNextCue(GuiButtonEventArgs &evt) {nextCue();}
    void eventPlay(GuiButtonEventArgs &evt) {play();}
    void eventPlayContinue(GuiButtonEventArgs &evt) {playContinue();}
    void eventPlaySelection(GuiButtonEventArgs &evt) {playSelection();}
    void eventStop(GuiButtonEventArgs &evt) {stop();}

    void displayOscMessage(ofxOscMessage &m);
    
    map<int, AbletonTrack*> tracks;

    GuiPanel control;
    GuiMenu *sceneSelector;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    string abletonOscHost;
    bool connected;
    
    float tempo;
    float time;
    bool overdub;
    float volume;
    float pan;
    int numTracks;
    int numScenes;
};