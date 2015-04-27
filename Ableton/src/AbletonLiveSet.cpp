#include "AbletonLiveSet.h"

Ableton::Ableton()
{
    connected = false;
    setAutoUpdate(false);
    setAutoDraw(false);
    
    // initialize variables
}

void Ableton::setAutoUpdate(bool autoUpdate)
{
    control.setAutoUpdate(autoUpdate);
}

void Ableton::setAutoDraw(bool autoDraw)
{
    control.setAutoUpdate(true);
}

void Ableton::setConnected(bool connected)
{
    this->connected = connected;
}

void Ableton::setup(string abletonOscHost)
{
    this->abletonOscHost = abletonOscHost;
    
    sender.setup(abletonOscHost, ABLETON_OSC_PORT_OUT);
    receiver.setup(ABLETON_OSC_PORT_IN);
    setConnected(true);
    setAutoUpdate(true);
    setAutoDraw(true);
    
    control.addToggle("Connected", &connected, this, &Ableton::eventSetConnected);      // unusued
    control.addButton("Refresh", this, &Ableton::eventRefresh);

    GuiWidget *widgetGlobal = control.addWidget("Global");
    widgetGlobal->addSlider("Tempo", &tempo, 4.0f, 999.0f, this, &Ableton::eventSetTempo);
    widgetGlobal->addSlider("Time", &time, 4.0f, 999.0f, this, &Ableton::eventSetTime);
    widgetGlobal->addSlider("Volume", &volume, 0.0f, 1.0f, this, &Ableton::eventSetVolume);
    widgetGlobal->addSlider("Pan", &pan, -1.0f, 1.0f, this, &Ableton::eventSetPan);
    widgetGlobal->addToggle("Overdub", &overdub, this, &Ableton::eventSetOverdub);
    
    GuiWidget *widgetPlayback = control.addWidget("Playback");
    widgetPlayback->addButton("Prev cue", this, &Ableton::eventPrevCue);
    widgetPlayback->addButton("Next cue", this, &Ableton::eventNextCue);
    widgetPlayback->addButton("Play", this, &Ableton::eventPlay);
    widgetPlayback->addButton("Continue", this, &Ableton::eventPlayContinue);
    widgetPlayback->addButton("Play selection", this, &Ableton::eventPlaySelection);
    widgetPlayback->addButton("Stop", this, &Ableton::eventStop);
    
    scanLiveSet();
    
    // REFRESH OPTION!
    // -> delete everything
    // -> redo scanLiveSet
    // CONNECTED variable
}

void Ableton::clear()
{
    map<int, AbletonTrack*>::iterator it = tracks.begin();
    while (it != tracks.end())
    {
        delete it->second;
        tracks.erase(it++);
    }
    
    //
    //
    //????
    //sceneSelector->clearToggles();
}

void Ableton::scanLiveSet()
{
    requestNumScenes();
    requestNumTracks();
}

void Ableton::eventSelectScene(GuiMenuEventArgs &evt)
{
    selectScene(evt.index);
}

void Ableton::requestNumScenes()
{
    ofxOscMessage msg;
    msg.setAddress("/live/scenes");
    sender.sendMessage(msg);
}

void Ableton::requestNumTracks()
{
    ofxOscMessage msg;
    msg.setAddress("/live/tracks");
    sender.sendMessage(msg);
}

void Ableton::requestDeviceList(int track)
{
    ofxOscMessage msg;
    msg.setAddress("/live/devicelist");
    msg.addIntArg(track);
    sender.sendMessage(msg);
}

void Ableton::requestDeviceParameters(int track, int device)
{
    ofxOscMessage msg;
    msg.setAddress("/live/device");
    msg.addIntArg(track);
    msg.addIntArg(device);
    sender.sendMessage(msg);
}

void Ableton::requestDeviceParametersRange(int track, int device)
{
    ofxOscMessage msg;
    msg.setAddress("/live/device/range");
    msg.addIntArg(track);
    msg.addIntArg(device);
    sender.sendMessage(msg);
}

void Ableton::selectScene(int scene)
{
    ofxOscMessage msg;
    msg.setAddress("/live/play/scene");
    msg.addIntArg(scene);
    sender.sendMessage(msg);
}

void Ableton::update()
{
    while(receiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        displayOscMessage(m);
        
        if (m.getAddress() == "/live/device/param") {
            getParameterUpdate(m);
        }
        else if (m.getAddress() == "/live/devicelist") {
            getDeviceList(m);
        }
        else if (m.getAddress() == "/live/device/allparam") {
            getDeviceParameters(m);
        }
        else if (m.getAddress() == "/live/device/range") {
            getDeviceParametersRange(m);
        }
        else if (m.getAddress() == "/live/scenes") {
            getNumScenes(m);
        }
        else if (m.getAddress() == "/live/tracks") {
            getNumTracks(m);
        }
        else {
            displayOscMessage(m);
        }
    }
}

void Ableton::getDeviceList(ofxOscMessage &m)
{
    int numArgs = m.getNumArgs();
    int track = m.getArgAsInt32(0);
    
    if (tracks.count(track) == 0)
    {
        AbletonTrack *newTrack = new AbletonTrack("track "+ofToString(track), track, numScenes, &sender);
        tracks[track] = newTrack;
        newTrack->getControl()->setCollapsible(true);
        
        if (track == 0) {
            control.attachWidget(newTrack->getControl());
        }
        else {
            tracks[track-1]->getControl()->attachWidget(newTrack->getControl());
        }
    }
    
    for (int i = 1; i < numArgs; i+=2)
    {
        int device = m.getArgAsInt32(i);
        string name = m.getArgAsString(i+1);
        if (tracks[track]->getDevices().count(device) == 0)
        {
            AbletonDevice *newDevice = new AbletonDevice(name, track, device, &sender);
            tracks[track]->addDevice(device, newDevice);
        }
        requestDeviceParameters(track, device);
        requestDeviceParametersRange(track, device);
    }
}

void Ableton::getDeviceParameters(ofxOscMessage &m)
{
    int track = m.getArgAsInt32(0);
    int device = m.getArgAsInt32(1);
    if (tracks.count(track) != 0 && tracks[track]->getDevices().count(device) != 0) {
        tracks[track]->getDevices()[device]->getParameterValues(m);
    }
}

void Ableton::getDeviceParametersRange(ofxOscMessage &m)
{
    int track = m.getArgAsInt32(0);
    int device = m.getArgAsInt32(1);
    if (tracks.count(track) != 0 && tracks[track]->getDevices().count(device) != 0) {
        tracks[track]->getDevices()[device]->getParameterRanges(m);
    }
}

void Ableton::getParameterUpdate(ofxOscMessage &m)
{
    int track = m.getArgAsInt32(0);
    int device = m.getArgAsInt32(1);
    int parameter = m.getArgAsInt32(2);
    if (tracks.count(track) != 0 &&
        tracks[track]->getDevices().count(device) != 0 &&
        tracks[track]->getDevices()[device]->getParameters().count(parameter))
    {
        tracks[track]->getDevices()[device]->getParameters()[parameter]->setValue(m.getArgAsFloat(3));
    }
}

void Ableton::getNumScenes(ofxOscMessage &m)
{
    numScenes = m.getArgAsInt32(0);
    //    sceneSelector = control.addMenu("Select scene", this, &Ableton::eventSelectScene);
    for (int i=0; i<numScenes; i++) {
        //sceneSelector->addToggle("Scene "+ofToString(i));
    }
}

void Ableton::getNumTracks(ofxOscMessage &m)
{
    numTracks = m.getArgAsInt32(0);
    for (int t = 0; t < numTracks; t++) {
        requestDeviceList(t);
    }
}

void Ableton::prevCue()
{
    ofxOscMessage msg;
    msg.setAddress("/live/prev/cue");
    sender.sendMessage(msg);
}

void Ableton::nextCue()
{
    ofxOscMessage msg;
    msg.setAddress("/live/next/cue");
    sender.sendMessage(msg);
}

void Ableton::play()
{
    ofxOscMessage msg;
    msg.setAddress("/live/play");
    sender.sendMessage(msg);
}

void Ableton::playContinue()
{
    ofxOscMessage msg;
    msg.setAddress("/live/play/continue");
    sender.sendMessage(msg);
}

void Ableton::playSelection()
{
    ofxOscMessage msg;
    msg.setAddress("/live/play/selection");
    sender.sendMessage(msg);
}

void Ableton::stop()
{
    ofxOscMessage msg;
    msg.setAddress("/live/stop");
    sender.sendMessage(msg);
}

void Ableton::setTempo(float tempo)
{
    this->tempo = tempo;
    ofxOscMessage msg;
    msg.setAddress("/live/tempo");
    msg.addFloatArg(tempo);
    sender.sendMessage(msg);
}

void Ableton::setTime(float time)
{
    this->time = time;
    ofxOscMessage msg;
    msg.setAddress("/live/time");
    msg.addFloatArg(time);
    sender.sendMessage(msg);
}

void Ableton::setOverdub(bool overdub)
{
    this->overdub = overdub;
    ofxOscMessage msg;
    msg.setAddress("/live/overdub");
    msg.addIntArg(overdub ? 1 : 0);
    sender.sendMessage(msg);
}

void Ableton::setVolume(float volume)
{
    this->volume = volume;
    ofxOscMessage msg;
    msg.setAddress("/live/master/volume");
    msg.addFloatArg(volume);
    sender.sendMessage(msg);
}

void Ableton::setPan(float pan)
{
    this->pan = pan;
    ofxOscMessage msg;
    msg.setAddress("/live/master/pan");
    msg.addFloatArg(pan);
    sender.sendMessage(msg);
}

void Ableton::displayOscMessage(ofxOscMessage &m)
{
    cout << "address : "<< m.getAddress() << endl;
    for (int i = 0; i < m.getNumArgs(); i++)
    {
        if      (m.getArgType(i) == OFXOSC_TYPE_INT32) {
            cout << " i--> "<<m.getArgAsInt32(i)<<endl;
        }
        else if (m.getArgType(i) == OFXOSC_TYPE_INT64) {
            cout << " j--> "<<m.getArgAsInt64(i)<<endl;
        }
        else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT) {
            cout << " f--> "<<m.getArgAsFloat(i)<<endl;
        }
        else if (m.getArgType(i) == OFXOSC_TYPE_STRING) {
            cout << " s--> "<<m.getArgAsString(i)<<endl;
        }
        else if (m.getArgType(i) == OFXOSC_TYPE_BLOB) {
            cout << " b--> "<<m.getArgAsBlob(i)<<endl;
        }
    }
}

void Ableton::refresh()
{
    clear();
    scanLiveSet();
}