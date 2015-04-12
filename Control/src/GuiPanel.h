#pragma once

#include "GuiWidget.h"
#include "Sequencer.h"


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
    
    
    bool getHasSequencer() {return sequencerMade;}
    Sequencer * getSequencer() {return sequencer;}
    
    /////
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
    void createSequencer()
    {
        sequencer = new Sequencer(getName()+" sequencer", this, GUI_DEFAULT_SEQUENCER_NUMCOLS);
        sequencer->setParent(this);
        sequencer->setAutoUpdate(false);
        sequencer->setAutoDraw(false);
        sequencerMade = true;
    }
    

    
    
    
    ///////////////
    
    void savePresetPrompt(GuiButtonEventArgs &e)
    {
        savePreset("/Users/Gene/Desktop/testXml.xml");
    }
    
    void loadPresetPrompt(GuiButtonEventArgs &e)
    {
        loadPreset("/Users/Gene/Desktop/testXml.xml");
    }
    
    void savePreset(string path)
    {
        ofXml xml;
        xml.addChild("Preset");
        xml.setTo("Preset");
        getXml(xml);
        //saveSequencerToXml(xml);
        xml.save(path);
    }
    
    void loadPreset(string path)
    {
        ofXml xml;
        xml.load(path);
        xml.setTo("Preset");
        setFromXml(xml);
        //loadSequencerFromXml(xml);
    }
    
    void saveSequencerToXml(ofXml &xml)
    {
        if (sequencerMade)
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
            if (!sequencerMade) {
                createSequencer();
            }
            sequencer->setFromXml(xml);
            xml.setToParent();
        }
        else {
            ofLog(OF_LOG_ERROR, "No sequencer found in preset");
        }
    }
    
    void setupGuiPositions();
    
protected:
    
    void eventToggleSequencer(GuiButtonEventArgs &e);
    
    GuiToggle *tOsc, *tSeq, *tXml;
    bool bOsc, bSeq, bXml;
    bool controlRow;
    
    GuiWidget *meta;
    Sequencer *sequencer;
    bool sequencerMade;
};
