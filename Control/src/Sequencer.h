#pragma once

#include "ofMain.h"
#include "GuiSlider.h"
#include "GuiWidgetBase.h"
#include "GuiMenu.h"
#include "Bpm.h"
#include "Sequence.h"



/*
 TODO : xml bugs
  - integer slider quantizing wrong
  - replace names with addresses
    - weird sequence addressing name+name
  - if no seq, then xml makes seq
  - when to rest sequence elements list
*/




class Sequencer : public GuiWidgetBase
{
public:
    Sequencer(string name, GuiWidgetBase * panel, int numCols=8);
    ~Sequencer();
    
    void resetSequencerElements();
    
    void setActive(bool active);
    void setSmooth(bool smooth);
    void setBpm(int bpm);
    void setNumberColumns(int numCols);
    
    bool getActive() {return active;}
    bool getSmooth() {return smooth;}
    int getBpm() {return bpm;}
    int getNumberColumns() {return numCols;}

    void saveSequencerSet(string sequencerName="");
    void loadSequencerSet(string sequencerName);

    void update();
    void draw();

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);

    bool keyPressed(int key);
    
    
    
    
    
//////////////
    void getXml(ofXml &xml)
    {
        ofXml xmlSequencerSets;
        xmlSequencerSets.addChild("SequencerSets");
        xmlSequencerSets.setTo("SequencerSets");
        
        map<string, SequencerSavedSet>::iterator it = sequencerSets.begin();
        for (; it != sequencerSets.end(); ++it)
        {
            ofXml xmlSequencerSet;
            xmlSequencerSet.addChild("SequencerSet");
            xmlSequencerSet.setTo("SequencerSet");
            xmlSequencerSet.addValue<string>("Name", it->second.name);
            xmlSequencerSet.addValue<int>("Bpm", it->second.bpm);
            xmlSequencerSet.addValue<bool>("Smooth", it->second.smooth);
            xmlSequencerSet.addValue<int>("Lerp", it->second.lerpNumFrames);
            xmlSequencerSet.addValue<int>("NumCols", it->second.numCols);
            
            map<string, vector<float> >::iterator its = it->second.sequences.begin();
            for (; its != it->second.sequences.end(); ++its)
            {
                ofXml xmlSequence;
                xmlSequence.addChild("Sequence");
                xmlSequence.setTo("Sequence");
                xmlSequence.addValue("Name", its->first);
                xmlSequence.addValue<vector<float> >("Values", its->second);
                xmlSequence.addValue<bool>("Active", it->second.sequencesActive.count(its->first) > 0 ? it->second.sequencesActive[its->first] : true);
                xmlSequencerSet.addXml(xmlSequence);
            }
            xmlSequencerSets.addXml(xmlSequencerSet);
        }
        
        ofXml xmlCurrent;
        xmlCurrent.addChild("Current");
        xmlCurrent.setTo("Current");
        xmlCurrent.addValue<string>("Name", getName());
        xmlCurrent.addValue<int>("Bpm", bpm);
        xmlCurrent.addValue<bool>("Smooth", smooth);
        xmlCurrent.addValue<int>("Lerp", lerpNumFrames);
        xmlCurrent.addValue<int>("NumCols", numCols);
        
        for (auto pair : sequencePairs)
        {
            for (auto e : pair->getElementPairs())
            {
                ofXml xmlSequence;
                xmlSequence.addChild("Sequence");
                xmlSequence.setTo("Sequence");
                xmlSequence.addValue<string>("Name", pair->getParent()->getName() + "/" + e->getElement()->getName());
                xmlSequence.addValue<vector<float> >("Values", e->getSequence()->getAllValues());
                xmlSequence.addValue<bool>("Active", e->getSequence()->getActive());
                xmlCurrent.addXml(xmlSequence);
            }
        }
        
        xml.addXml(xmlSequencerSets);
        xml.addXml(xmlCurrent);
    }
    
    void setFromXml(ofXml &xml)
    {
        sequencerSets.clear();
        
        // need to try this.....
        
        
        //mChoose->clearToggles();
        
        
        
        
        if (xml.exists("SequencerSets"))
        {
            xml.setTo("SequencerSets");
            if (xml.exists("SequencerSet[0]"))
            {
                xml.setTo("SequencerSet[0]");
                do {
                    string sequencerName = xml.getValue<string>("Name");
                    SequencerSavedSet newSet(sequencerName,
                                             xml.getValue<bool>("Smooth"),
                                             xml.getValue<int>("Bpm"),
                                             xml.getValue<int>("Lerp"),
                                             xml.getValue<int>("NumCols"));

                    if (xml.exists("Sequence[0]"))
                    {
                        xml.setTo("Sequence[0]");
                        do {
                            string sequenceName = xml.getValue<string>("Name");
                            bool sequenceActive = xml.getValue<bool>("Active");

                            string valueStringXml = xml.getValue<string>("Values");
                            vector<string> valueString = ofSplitString(valueStringXml.substr(1, valueStringXml.size()-2), ",");
                            vector<float> sequenceValues;
                            for (auto vs : valueString) {
                                sequenceValues.push_back(ofToFloat(vs));
                            }

                            newSet.addSequence(sequenceName, sequenceValues, sequenceActive);

                            // is checking existing sequence pairs necessary?
                            /*
                            for (auto pair : sequencePairs)
                            {
                                for (auto &e : pair->getElementPairs())
                                {
                                    string currentSequenceName = pair->getParent()->getName() + "/" + e->getElement()->getName();
                                    if (sequenceName == currentSequenceName) {
                                        newSet.addSequence(currentSequenceName, sequenceValues, sequenceActive);
                                    }
                                }
                            }
                             */
                        }
                        while(xml.setToSibling());
                        xml.setToParent();
                    }
                    xml.setToParent();
                    
                    
                    sequencerSets[sequencerName] = newSet;
                    mChoose->addToggle(sequencerName);
                }
                while(xml.setToSibling());
                xml.setToParent();
            }
            xml.setToParent();
        }

        if (xml.exists("Current"))
        {
            xml.setTo("Current");
            
            setName(xml.getValue<string>("Name"));
            setBpm(xml.getValue<int>("Bpm"));
            setSmooth(xml.getValue<bool>("Smooth"));
            setNumberColumns(xml.getValue<int>("NumCols"));
            lerpNumFrames = xml.getValue<int>("Lerp");
            
            if (xml.exists("Sequence[0]"))
            {
                xml.setTo("Sequence[0]");
                do {
                    string sequenceName = xml.getValue<string>("Name");
                    bool sequenceActive = xml.getValue<bool>("Active");

                    // a bit clunky
                    string valueStringXml = xml.getValue<string>("Values");
                    vector<string> valueString = ofSplitString(valueStringXml.substr(1, valueStringXml.size()-2), ",");
                    vector<float> sequenceValues;
                    for (auto vs : valueString) {
                        sequenceValues.push_back(ofToFloat(vs));
                    }
                    
                    for (auto pair : sequencePairs)
                    {
                        for (auto &e : pair->getElementPairs())
                        {
                            string currentSequenceName = pair->getParent()->getName() + "/" + e->getElement()->getName();
                            if (currentSequenceName == sequenceName)
                            {
                                e->getSequence()->setFromValues(sequenceValues);
                                e->getSequence()->setActive(sequenceActive);
                            }
                        }
                    }
                }
                while(xml.setToSibling());
                xml.setToParent();
            }
            xml.setToParent();
            xml.setToParent();
        }
    }
    

//////////////
    
    
protected:
    
    class SequenceElementPair
    {
    public:
        SequenceElementPair(GuiElement *element);
        ~SequenceElementPair();
        
        Sequence * getSequence() {return sequence;}
        GuiElement * getElement() {return element;}
        
        void setElementFromSequence();
        void setSequenceFromElement(int column);
        void lerpWidgetToSequencer(int lerpNumFrames);
    
    private:
        
        void eventSetSequenceFromKey(Sequence::SequenceKeyboardEventArgs & s);

        Sequence *sequence;
        GuiElement *element;
    };
    
    class SequenceGroupPair
    {
    public:
        SequenceGroupPair(GuiElementGroup *elementGroup, GuiWidgetBase *parent);
        ~SequenceGroupPair();
        
        vector<SequenceElementPair*> & getElementPairs() {return pairs;}
        GuiWidgetBase * getParent() {return parent;}
        
        void setNumberColumns(int numCols);
        void setCursor(float cursor);
        void randomizeSequence(float density, float range);
        void updateWidgetFromSequencer();
        void lerpWidgetToSequencer(int lerpNumFrames);
    
    private:
        
        void menuSelected(GuiElementEventArgs &e);
        GuiElementGroup *elementGroup;
        GuiWidgetBase *parent;
        vector<SequenceElementPair*> pairs;
        int numCols;
    };
    
    struct SequencerSavedSet
    {
        string name;
        bool smooth;
        int bpm;

        int lerpNumFrames;
        int numCols;
        map<string, vector<float> > sequences;
        map<string, bool> sequencesActive;
        
        SequencerSavedSet() { }
        SequencerSavedSet(string name, bool smooth, int bpm, int lerpNumFrames, int numCols);
        void addSequence(string sequenceName, vector<float> sequence, bool active);
    };
    
    void getElementGroupsFromWidget(GuiWidgetBase * widget);
    void removeElementGroupsFromWidget(GuiWidgetBase * widget);

    void setupSequencer();
    void setupGuiPositions();
    void setupSequencesFromPanel();
    
    void setAllSequencersActive(bool allSequencesActive);
    void randomizeSequencer();
    void selectColumn(int column);
    void setColumnToCurrentValues(int column);
    void next();

    void eventSequencerMenuSelection(GuiElementEventArgs & evt);
    void eventBpm(GuiElementEventArgs &s);
    void eventNumColumns(GuiElementEventArgs &s);
    void eventActive(GuiElementEventArgs &b);
    void eventWidgetCollapsed(string &s);
    void eventRandomizeSequencer(GuiElementEventArgs &b);
    void eventSelectColumn(GuiElementEventArgs &b);
    void eventBeat();
    void eventElementDeleted(GuiElement * &elementToDelete);
    
    Bpm clock;
    int numCols;
    int column;
    float cursor;
    int bpm;
    int bpmTime;
    float bpmInterval;
    int currentColumn;
    int lerpNumFrames;
    bool active;
    bool smooth;
    bool allSequencesActive;
    
    vector<SequenceGroupPair*> sequencePairs;
    map<string, SequencerSavedSet> sequencerSets;
    GuiWidgetBase * panel;
    ofRectangle sequencerRect;
    
    GuiToggle *tActive, *tSmooth;
    GuiSlider<int> *sBeatPm, *sNumCol, *sLerpNf;
    GuiButton *bRandom;
    GuiMenu *mChoose;
    ofRectangle bToggleAllSequencers;
    vector<ofRectangle> bColumnSelectors;
    int rectSelectColumnMouseOver;
    bool rectToggleAllSeqMouseOver;
    
    int sequencerPanelMargin;
    int sequencerWidth;
};

