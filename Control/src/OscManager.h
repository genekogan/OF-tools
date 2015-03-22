#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Parameter.h"
#include "GuiButton.h"
#include "GuiTextBox.h"
#include "GuiWidgetBase.h"
#include "GuiPanel.h"



class OscManager : public GuiWidget
{
public:
    
    class ParameterOscPair
    {
    public:
        
        struct ParameterOscPairEventArgs
        {
            ParameterBase *parameter;
            bool sender;
            bool toAdd;
            ParameterOscPairEventArgs(ParameterBase *parameter, bool sender, bool toAdd);
        };
        
        ParameterOscPair(ParameterBase *parameter);
        ~ParameterOscPair();
        
        GuiToggle * getToggleIn() {return tOscIn;}
        GuiToggle * getToggleOut() {return tOscOut;}
        GuiTextBox * getToggleAddress() {return tOscAddress;}
        ParameterBase * getParameter() {return parameter;}
        
        void toggleOscIn(GuiElementEventArgs & e);
        void toggleOscOut(GuiElementEventArgs & e);
        void editOscAddress(GuiElementEventArgs & e);
        
        ofEvent<ParameterOscPairEventArgs> parameterOscEvent;
        
    private:
        
        ParameterBase *parameter;
        GuiToggle *tOscIn, *tOscOut;
        GuiTextBox *tOscAddress;
    };
    
    class OscPanel : public GuiWidgetBase
    {
    public:
        OscPanel(string name);
        ~OscPanel();
        
        void addParameter(ParameterBase *parameter);
        
    private:
        
        void setupGuiPositions();
        void toggleReceiveAll(GuiElementEventArgs & e);
        void toggleSendAll(GuiElementEventArgs & e);
        
        GuiToggle *allIn, *allOut;
        vector<ParameterOscPair*> parameters;
    };
    
    OscManager();
    ~OscManager();
    
    void setupSender(string host, int portIn);
    void setupReceiver(int portOut);
    
    OscPanel * addOscGroup(string name);
    void addOscGroup(OscPanel *oscGroup);
    void addParameters(string groupName, vector<ParameterBase*> parameter);
    void addParameter(ParameterBase *parameter);
    
    void addParameterToSender(ParameterBase *parameter);
    void addParameterToReceiver(ParameterBase *parameter);
    void removeParameterFromSender(ParameterBase *parameter);
    void removeParameterFromReceiver(ParameterBase *parameter);

    void update();
    
protected:
    
    void eventParameterOscAction(ParameterOscPair::ParameterOscPairEventArgs & p);
    void eventWidgetChanged(string & s);
    
    ofxOscSender send;
    ofxOscReceiver receive;
    string host;
    int portIn, portOut;
    
    vector<ParameterBase*> sendParameters;
    map<string, ParameterBase*> receiveParameters;

    OscPanel *individualParameters;
};
