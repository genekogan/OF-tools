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
    
    struct ElementOsc
    {
        GuiElement *element;
        OscManager *osc;
        
        GuiTextBox *tAddress;
        GuiToggle *tSend;
        string address;
        bool sending;
        
        ElementOsc(GuiElement *element, OscManager *osc);
        void eventSetAddress(GuiTextBoxEventArgs &evt);
        void eventSetSending(GuiButtonEventArgs &evt);
        void updateAddress();
    };
    
    struct ElementOscGroup
    {
        GuiElement *element;
        OscManager *osc;
        
        vector<ElementOsc*> elements;
        ElementOscGroup(GuiElement *element, OscManager *osc);
    };
    
    void setupOscManagerPanel();
    void addElement(GuiElement* & newElement);
    void removeElement(GuiElement* & element);
    void eventMakeTouchOscLayout(GuiButtonEventArgs &evt);

    map<GuiElement*,ElementOscGroup*> groups;
    GuiWidget * panel;
    OscManager * osc;
    int oscManagerWidth;
    GuiButton *tTouchOsc;
};


