#pragma once

#include "ofMain.h"
#include "GuiWidget.h"
#include "OscManager.h"


class OscManagerPanel : public GuiElement
{
public:
    OscManagerPanel(string name, GuiWidget *panel, OscManager *osc);
    ~OscManagerPanel();
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void update();
    void draw();
    
    void getXml(ofXml &xml) { }
    void setFromXml(ofXml &xml) { }
    
    void setupGuiPositions();
    
protected:
    
    struct ElementOscGroup
    {
        GuiElement *element;
        OscManager *osc;
        
        GuiTextBox *tAddress;
        GuiToggle *tSend;
        string address;
        bool sending;

        ElementOscGroup(GuiElement *element, OscManager *osc);
        void eventSetAddress(GuiTextBoxEventArgs &evt);
        void eventSetSending(GuiButtonEventArgs &evt);
        void updateAddress();
    };
    
    void setupOscManagerPanel();
    void addElement(GuiElement* & newElement);
    void removeElement(GuiElement* & element);
    
    map<GuiElement*,ElementOscGroup*> groups;   // needs to be ptr?
    GuiWidget * panel;
    OscManager * osc;
    int oscManagerWidth;
    
    GuiButton *tTouchOsc;
    
    void eventMakeTouchOscLayout(GuiButtonEventArgs &evt) {
        
        panel->makeTouchOscLayout("myTouchOscLayout");

    }
};


