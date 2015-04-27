#include "AbletonDevice.h"


AbletonDevice::AbletonDevice(string name, int track, int device, ofxOscSender * sender)
{
    this->sender = sender;
    this->name = name;
    this->track = track;
    this->device = device;
    control = new GuiWidget(name);
}

AbletonDevice::~AbletonDevice()
{
    clear();
}

void AbletonDevice::clear()
{
    map<int, AbletonParameter*>::iterator it = parameters.begin();
    while (it != parameters.end())
    {
        control->removeElement(it->second->getName());
        delete it->second;
        parameters.erase(it++);
    }
}

void AbletonDevice::addParameter(int index, string name, float value)
{
    if (parameters.count(index) == 0) {
        parameters[index] = new AbletonParameter(name, track, device, index, value, sender);
    }
}

void AbletonDevice::getParameterValues(ofxOscMessage &m)
{
    for (int i = 2; i < m.getNumArgs(); i+=3)
    {
        int id = m.getArgAsInt32(i);
        float value = m.getArgAsFloat(i+1);
        string name = m.getArgAsString(i+2);
        if (parameters.count(id) == 0) {
            addParameter(id, name, value);
        }
    }
}

void AbletonDevice::getParameterRanges(ofxOscMessage &m)
{
    for (int i = 2; i < m.getNumArgs(); i+=3)
    {
        int id = m.getArgAsInt32(i);
        float min = m.getArgAsFloat(i+1);
        float max = m.getArgAsFloat(i+2);
        if (parameters.count(id) != 0)
        {
            parameters[id]->setRange(min, max);
            parameters[id]->createSlider(control);
        }
    }
}