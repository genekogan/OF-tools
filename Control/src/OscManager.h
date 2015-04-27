#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "GuiWidget.h"


class OscManager
{
public:
    OscManager();
    ~OscManager();
    
    GuiWidget & getControl() {return panel;}
    
    bool setupSender(string host, int portOut);
    bool setupReceiver(int portIn);
    
    void addWidget(GuiWidget & widget);
    void addElementToOscManager(GuiElement * element);
    void addElementToSender(GuiElement *element);
    void removeElementFromSender(GuiElement *element);
    
    void update();
    
    
    void blah() {
        
        ofxOscMessage msg;
        msg.setAddress("/live/state");
        sender.sendMessage(msg);
        cout << "AWNS SNSNS " << endl;
    }
    
private:

    void eventToggleSending(GuiButtonEventArgs &evt);
    void eventToggleReceiving(GuiButtonEventArgs &evt);
    void eventEditHostOut(GuiTextBoxEventArgs &evt);
    void eventEditPortOut(GuiTextBoxEventArgs &evt);
    void eventEditPortIn(GuiTextBoxEventArgs &evt);
    
    void receiveOscMessages();
    void sendOscMessages();
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    string host;
    int portIn, portOut;
    bool sending, receiving;
    
    vector<GuiElement*> sParameters;
    map<string,GuiElement*> rParameters;
    
    GuiWidget panel;
    string hostOut;
    string portInS, portOutS;
};

