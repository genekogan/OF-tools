#pragma once
/*
#include "ofMain.h"
#include "ofxOsc.h"
#include "Parameter.h"
#include "GuiButton.h"
#include "GuiTextBox.h"
#include "GuiWidget.h"



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
        
        void toggleOscIn(GuiButtonEventArgs & e);
        void toggleOscOut(GuiButtonEventArgs & e);
        void editOscAddress(GuiTextBoxEventArgs & e);
        
        ofEvent<ParameterOscPairEventArgs> parameterOscEvent;
        
    private:
        
        ParameterBase *parameter;
        GuiToggle *tOscIn, *tOscOut;
        GuiTextBox *tOscAddress;
    };
    
    
    void addParameters(vector<ParameterBase*> & parameters)
    {
        
    }
    
    void addParameter(ParameterBase* parameter)
    {
        
    }
    
    vector<ParameterBase*> sendParameters;
    map<string, ParameterBase*> receiveParameters;

    
};
*/

/*
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
        
        void toggleOscIn(GuiButtonEventArgs & e);
        void toggleOscOut(GuiButtonEventArgs & e);
        void editOscAddress(GuiTextBoxEventArgs & e);
        
        ofEvent<ParameterOscPairEventArgs> parameterOscEvent;
        
    private:
        
        ParameterBase *parameter;
        GuiToggle *tOscIn, *tOscOut;
        GuiTextBox *tOscAddress;
    };
    
    class OscPanel : public GuiWidget
    {
    public:
        OscPanel(string name);
        ~OscPanel();
        
        void addParameter(ParameterBase *parameter);
        
    private:
        
        void setupGuiPositions();
        void toggleReceiveAll(GuiButtonEventArgs & e);
        void toggleSendAll(GuiButtonEventArgs & e);
        
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
*/