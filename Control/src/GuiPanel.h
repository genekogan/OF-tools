#pragma once

#include "GuiWidget.h"
#include "Sequencer.h"
#include "OscManagerPanel.h"


class GuiPanel : public GuiWidget
{
public:
    GuiPanel();
    ~GuiPanel();
    
    void setPosition(int x, int y);
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void update();
    void draw();
    
    void enableControlRow();
    void disableControlRow();
    
    void createSequencer();
    void createOscManager(OscManager *osc);

    bool getHasSequencer() {return sequencerMade;}
    Sequencer * getSequencer() {return sequencer;}

    bool getHasOscManagerPanel() {return oscManagerMade;}
    OscManagerPanel * getOscManagerPanel() {return oscManager;}
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
    void savePreset(string path);
    void loadPreset(string path);

    void setupGuiPositions();
    void addElementToTouchOscLayout(TouchOscPage *page, float *y);
    
protected:
    
    void eventToggleSequencer(GuiButtonEventArgs &e);
    void eventToggleOscManager(GuiButtonEventArgs &e);

    void savePresetPrompt(GuiButtonEventArgs &e);
    void loadPresetPrompt(GuiButtonEventArgs &e);
    
    void saveSequencerToXml(ofXml &xml);
    void loadSequencerFromXml(ofXml &xml);

    GuiToggle *tOsc, *tSeq, *tXml;
    bool bOsc, bSeq, bXml;
    bool controlRow;
    
    GuiWidget *meta;

    Sequencer *sequencer;
    bool sequencerMade;
    
    OscManagerPanel *oscManager;
    bool oscManagerMade;
};
