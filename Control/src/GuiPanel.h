#pragma once

#include "GuiWidget.h"
#include "Sequencer.h"


class GuiPanel : public GuiWidget
{
public:
    GuiPanel();
    ~GuiPanel();
    
    void setPosition(int x, int y);
    
    GuiWidget * addWidget(string name);
    void addWidget(GuiWidget *widget);
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);

    void update();
    void draw();
    
    void enableControlRow();
    void disableControlRow();
    
    Sequencer * getSequencer() {return sequencer;}
    
    
    
    ///////////////
    
    void savePresetPrompt(GuiElementEventArgs &e)
    {
        savePreset("/Users/Gene/Desktop/testXml.xml");
    }

    void loadPresetPrompt(GuiElementEventArgs &e)
    {
        loadPreset("/Users/Gene/Desktop/testXml.xml");
    }

    void savePreset(string path)
    {
        ofXml xml;
        xml.addChild("Preset");
        xml.setTo("Preset");
        getXml(xml);
        saveSequencerToXml(xml);
        xml.save(path);
    }
    
    void loadPreset(string path)
    {
        ofXml xml;
        xml.load(path);
        xml.setTo("Preset");
        setFromXml(xml);
        loadSequencerFromXml(xml);
    }
    
    void saveSequencerToXml(ofXml &xml)
    {
        if (controlRow)
        {
            ofXml xmlSequencer;
            xmlSequencer.addChild("Sequencer");
            xmlSequencer.setTo("Sequencer");
            sequencer->getXml(xmlSequencer);
            xml.addXml(xmlSequencer);
        }
    }
    
    void loadSequencerFromXml(ofXml &xml)
    {
        if (xml.exists("Sequencer"))
        {
            xml.setTo("Sequencer");
            sequencer->setFromXml(xml);
            xml.setToParent();
        }
        else {
            ofLog(OF_LOG_ERROR, "No sequencer found in preset");
        }
    }
    
    
protected:
    
    void eventToggleSequencer(GuiElementEventArgs &e);
    void setupGuiPositions();
    void eventWidgetChanged(string & s);
    
    GuiToggle *tOsc, *tSeq, *tXml;
    bool bOsc, bSeq, bXml;
    bool controlRow;
    
    GuiWidget *meta;
    Sequencer *sequencer;
    bool sequencerMade;
};
