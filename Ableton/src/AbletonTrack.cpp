#include "AbletonTrack.h"


AbletonTrack::AbletonTrack(string name, int track, int numScenes, ofxOscSender * sender)
{
    this->name = name;
    this->track = track;
    this->numScenes = numScenes;
    this->sender = sender;
    
    control = new GuiPanel();
    control->setName(name);
    
    vector<string> clips;
    for (int i=0; i<numScenes; i++) {
        clips.push_back("Clip "+ofToString(i));
    }
    control->addMenu("Select scene", clips, this, &AbletonTrack::eventSelectClip);
    
    GuiWidget *widgetPlayback = control->addWidget("Playback");
    widgetPlayback->addButton("Stop", this, &AbletonTrack::eventStop);
    widgetPlayback->addToggle("Arm", &arm, this, &AbletonTrack::eventArm);
    widgetPlayback->addToggle("Mute", &mute, this, &AbletonTrack::eventMute);
    widgetPlayback->addToggle("Solo", &solo, this, &AbletonTrack::eventSolo);
    widgetPlayback->addSlider("Volume", &volume, 0.0f, 1.0f, this, &AbletonTrack::eventVolume);
    widgetPlayback->addSlider("Pan", &pan, -1.0f, 1.0f, this, &AbletonTrack::eventPan);
    widgetPlayback->addSlider("Send", &send, 0.0f, 1.0f, this, &AbletonTrack::eventSend);
    widgetPlayback->addSlider("Pitch coarse", &pitchCoarse, -48, 48, this, &AbletonTrack::eventPitchCoarse);
    widgetPlayback->addSlider("Pitch fine", &pitchFine, -50, 50, this, &AbletonTrack::eventPitchFine);
}

AbletonTrack::~AbletonTrack()
{
    clear();
    control->removeElement("Select scene");
    delete control;
}

void AbletonTrack::clear()
{
    map<int, AbletonDevice*>::iterator it = devices.begin();
    while (it != devices.end())
    {
        control->removeElement(it->second->getName());
        delete it->second;
        devices.erase(it++);
    }
}

void AbletonTrack::selectClip(int clip)
{
    ofxOscMessage msg;
    msg.setAddress("/live/play/clipslot");
    msg.addIntArg(track);
    msg.addIntArg(clip);
    sender->sendMessage(msg);
}

void AbletonTrack::setArmed(bool arm)
{
    this->arm = arm;
    ofxOscMessage msg;
    msg.setAddress("/live/arm");
    msg.addIntArg(track);
    msg.addIntArg(arm ? 1 : 0);
    sender->sendMessage(msg);
}

void AbletonTrack::setMute(bool mute)
{
    this->mute = mute;
    ofxOscMessage msg;
    msg.setAddress("/live/mute");
    msg.addIntArg(track);
    msg.addIntArg(mute ? 1 : 0);
    sender->sendMessage(msg);
}

void AbletonTrack::setSolo(bool solo)
{
    this->solo = solo;
    ofxOscMessage msg;
    msg.setAddress("/live/solo");
    msg.addIntArg(track);
    msg.addIntArg(arm ? 1 : 0);
    sender->sendMessage(msg);
}

void AbletonTrack::setVolume(float volume)
{
    this->volume = volume;
    ofxOscMessage msg;
    msg.setAddress("/live/volume");
    msg.addIntArg(track);
    msg.addFloatArg(volume);
    sender->sendMessage(msg);
}

void AbletonTrack::setPan(float pan)
{
    this->pan = pan;
    ofxOscMessage msg;
    msg.setAddress("/live/pan");
    msg.addIntArg(track);
    msg.addFloatArg(pan);
    sender->sendMessage(msg);
}

void AbletonTrack::setSend(float send)
{
    this->send = send;
    ofxOscMessage msg;
    msg.setAddress("/live/send");
    msg.addIntArg(track);
    msg.addIntArg(0);
    msg.addFloatArg(send);
    sender->sendMessage(msg);
}

void AbletonTrack::setPitch(int pitchCoarse, int pitchFine)
{
    this->pitchCoarse = pitchCoarse;
    this->pitchFine = pitchFine;
    ofxOscMessage msg;
    msg.setAddress("/live/pitch");
    msg.addIntArg(track);
    msg.addIntArg(0);
    msg.addIntArg(pitchCoarse);
    msg.addIntArg(pitchFine);
    sender->sendMessage(msg);
}

void AbletonTrack::stop()
{
    ofxOscMessage msg;
    msg.setAddress("/live/stop");
    msg.addIntArg(track);
    sender->sendMessage(msg);
}

void AbletonTrack::addDevice(int device, AbletonDevice * newDevice)
{
    if (devices.count(device) == 0)
    {
        devices[device] = newDevice;
        control->addWidget(newDevice->getControl());
    }
    else {
        ofLog(OF_LOG_WARNING, "Device "+ofToString(device)+" already registered");
    }
}
