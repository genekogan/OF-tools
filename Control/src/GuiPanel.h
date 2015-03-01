#pragma once

#include "GuiWidget.h"
#include "Sequencer.h"


class GuiPanel : public GuiWidget
{
public:
    GuiPanel();
    ~GuiPanel();
    
    GuiWidget * addWidget(string name);
    void addWidget(GuiWidget *widget);
    
    void enableSequencer();
    void disableSequencer();

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);

    void update();
    void draw();
    
private:
    
    void setupGuiComponents();
    void eventToggleSequencer(GuiElementEventArgs &e);
    void eventWidgetChanged(string & s);
    
    GuiToggle *tOsc, *tSeq, *tXml;
    bool bOsc, bSeq, bXml;
    
    Sequencer *sequencer;
};
