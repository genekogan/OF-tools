#include "OscManager.h"


OscManager::OscManager()
{
    // initiate vars
}

OscManager::~OscManager()
{

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
    catch(runtime_error &e) {
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
    catch(runtime_error &e) {
        ofLog(OF_LOG_ERROR, ofToString(e.what()));
    }
    return receiving;
}

void OscManager::addWidget(GuiWidget & widget)
{
    widget.createOscManager(this);
    vector<ParameterBase*> parameters;
    widget.getParameters(parameters);
    for (auto p : parameters)
    {
        if (rParameters.count(p->getOscAddress()) == 0) {
            rParameters[p->getOscAddress()] = p;
        }
        else {
            ofLog(OF_LOG_WARNING, "Parameter at "+p->getOscAddress()+" already registered with receiver");
        }
    }
}

void OscManager::addParameterToSender(ParameterBase *parameter)
{
    vector<ParameterBase*>::iterator it = sParameters.begin();
    for (; it != sParameters.end(); ++it)
    {
        if (*it == parameter)
        {
            ofLog(OF_LOG_WARNING, "Parameter at "+parameter->getOscAddress()+" already registered with sender");
            return;
        }
    }
    sParameters.push_back(parameter);
}

void OscManager::removeParameterFromSender(ParameterBase *parameter)
{
    vector<ParameterBase*>::iterator it = sParameters.begin();
    while (it != sParameters.end())
    {
        if (*it == parameter) {
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
        cout << "address:: " << address << endl;
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
            p->sendOsc(msg);
            sender.sendMessage(msg);
        }
    }
}
