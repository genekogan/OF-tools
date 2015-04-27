#include "OscManager.h"


OscManager::OscManager()
{
    panel.setName("OSC");
    panel.addToggle("send", &sending, this, &OscManager::eventToggleSending);
    panel.addTextBox("hostOut", &hostOut, this, &OscManager::eventEditHostOut);
    panel.addTextBox("portOut", &portOutS, this, &OscManager::eventEditPortOut);
    panel.addToggle("receive", &receiving, this, &OscManager::eventToggleReceiving);
    panel.addTextBox("portIn", &portInS, this, &OscManager::eventEditPortIn);
    
    portIn = 9000;
    portOut = 9001;
    portInS = "9000";
    portOutS = "9001";
    hostOut = "localhost";
}

OscManager::~OscManager()
{

}

void OscManager::eventToggleSending(GuiButtonEventArgs &evt)
{
    if (sending) {
        setupSender(hostOut, portOut);
    }
}

void OscManager::eventToggleReceiving(GuiButtonEventArgs &evt)
{
    if (receiving) {
        setupReceiver(portIn);
    }
}

void OscManager::eventEditHostOut(GuiTextBoxEventArgs &evt)
{
    sending = false;
}

void OscManager::eventEditPortOut(GuiTextBoxEventArgs &evt)
{
    sending = false;
    portOut = ofToInt(portOutS);
}

void OscManager::eventEditPortIn(GuiTextBoxEventArgs &evt)
{
    receiving = false;
    portIn = ofToInt(portInS);
}


bool OscManager::setupSender(string host, int portOut)
{
    try
    {
        sender.setup(host, portOut);
        this->host = host;
        this->portOut = portOut;
        sending = true;
        ofLog(OF_LOG_NOTICE, "Connect OSC Sender "+host+", port "+ofToString(portOut));
    }
    catch(runtime_error &e)
    {
        sending = false;
        ofLog(OF_LOG_ERROR, ofToString(e.what()));
    }
    return sending;
}

bool OscManager::setupReceiver(int portIn)
{
    try
    {
        receiver.setup(portIn);
        this->portIn = portIn;
        receiving = true;
        ofLog(OF_LOG_NOTICE, "Connect OSC Receiver, port "+ofToString(portIn));
    }
    catch(runtime_error &e)
    {
        receiving = false;
        ofLog(OF_LOG_ERROR, ofToString(e.what()));
    }
    return receiving;
}

void OscManager::addWidget(GuiWidget & widget)
{
    widget.createOscManager(this);
    for (auto element : widget.getElements()) {
        addElementToOscManager(element);
    }
}

void OscManager::addElementToOscManager(GuiElement * element)
{
    if (element->isMultiElement())
    {
        for (auto e : ((GuiMultiElement *) element)->getElements()) {
            addElementToOscManager(e);
        }
    }
    else
    {
        if (rParameters.count(element->getOscAddress()) == 0) {
            rParameters[element->getOscAddress()] = element;
        }
        else {
            ofLog(OF_LOG_WARNING, "Parameter at "+element->getOscAddress()+" already registered with receiver");
        }

    }
}

void OscManager::addElementToSender(GuiElement *element)
{
    vector<GuiElement*>::iterator it = sParameters.begin();
    for (; it != sParameters.end(); ++it)
    {
        if (*it == element)
        {
            ofLog(OF_LOG_WARNING, "Parameter at "+element->getOscAddress()+" already registered with sender");
            return;
        }
    }
    sParameters.push_back(element);
}

void OscManager::removeElementFromSender(GuiElement *element)
{
    vector<GuiElement*>::iterator it = sParameters.begin();
    while (it != sParameters.end())
    {
        if (*it == element) {
            sParameters.erase(it);
        }
        else {
            ++it;
        }
    }
}

void OscManager::update()
{
    if (receiving) {
        receiveOscMessages();
    }
    if (sending) {
        sendOscMessages();
    }
}

void OscManager::receiveOscMessages()
{
    while(receiver.hasWaitingMessages())
    {
        ofxOscMessage msg;
        receiver.getNextMessage(&msg);
        string address = msg.getAddress();
        if (rParameters.count(address) > 0) {
            rParameters[address]->receiveOsc(msg);
        }
    }
}

void OscManager::sendOscMessages()
{
    for (auto p : sParameters)
    {
        if (p->valueChanged())
        {
            ofxOscMessage msg;
            msg.setAddress(p->getOscAddress());
            p->sendOsc(msg);
            sender.sendMessage(msg);
        }
    }
}
