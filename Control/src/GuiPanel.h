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
    void setName(string name);
    
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
    
    void savePreset(string name);
    void loadPreset(string name);

    void setupGuiPositions();
    
protected:
    
    void eventToggleSequencer(GuiButtonEventArgs &e);
    void eventToggleOscManager(GuiButtonEventArgs &e);
    void eventTogglePresets(GuiButtonEventArgs &e);

    void savePresetPrompt(GuiButtonEventArgs &e);
    void loadPresetPrompt(GuiMenuEventArgs &e);
    void refreshPresetMenu();
    
    void saveSequencerToXml(ofXml &xml);
    void loadSequencerFromXml(ofXml &xml);

    GuiToggle *tOsc, *tSeq, *tXml;
    bool bOsc, bSeq, bXml;
    bool controlRow;
    
    GuiWidget *meta;
    GuiMenu *menuPresets;

    Sequencer *sequencer;
    bool sequencerMade;
    
    OscManagerPanel *oscManager;
    bool oscManagerMade;
};
