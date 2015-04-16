#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "GuiWidget.h"



class OscManager
{
public:
    OscManager();
    ~OscManager();
    
    bool setupSender(string host, int portOut);
    bool setupReceiver(int portIn);
    
    void addWidget(GuiWidget & widget);
    void addParameterToSender(ParameterBase *parameter);
    void removeParameterFromSender(ParameterBase *parameter);
    
    void update();
    
private:

    void receiveOscMessages();
    void sendOscMessages();
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    string host;
    int portIn, portOut;
    bool sending, receiving;
    
    vector<ParameterBase*> sParameters;
    map<string, ParameterBase*> rParameters;
};

