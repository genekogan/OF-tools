#pragma once

#include "ofMain.h"
#include "GuiSlider.h"
#include "GuiWidget.h"
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




class Sequencer : public GuiElement
{
public:
    Sequencer(string name, GuiWidget * panel, int numCols=8);
    ~Sequencer();

    void setActive(bool active);
    void setSmooth(bool smooth);
    void setBpm(int bpm);
    void setLerpNumFrames(int lerpNumFrames);
    void setNumberColumns(int numCols);

    bool getActive() {return active;}
    bool getSmooth() {return smooth;}
    int getBpm() {return bpm;}
    int getLerpNumFrames() {return lerpNumFrames;}
    int getNumberColumns() {return numCols;}

    void setAllSequencersActive(bool allSequencesActive);
    void randomizeSequencer();
    void selectColumn(int column);
    void setColumnToCurrentValues(int column);
    void next();
    
    void saveSequencerSet(string sequencerName="");
    void loadSequencerSet(string sequencerName);

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void update();
    void draw();
    
    void getXml(ofXml &xml)
    {
        ofXml xmlSequencer;
        xmlSequencer.addChild("Sequencer");
        xmlSequencer.setTo("Sequencer");
        
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
            
            map<string, SequencerSavedSet::SequencerSavedSetGroup>::iterator itg = it->second.sequenceGroups.begin();
            for (; itg != it->second.sequenceGroups.end(); ++itg)
            {
                ofXml xmlSequenceGroup;
                xmlSequenceGroup.addChild("SequenceGroup");
                xmlSequenceGroup.setTo("SequenceGroup");
                xmlSequenceGroup.addValue("Name", itg->first);
                
                map<string, vector<float> >::iterator its = itg->second.sequences.begin();
                for (; its != itg->second.sequences.end(); ++its)
                {
                    ofXml xmlSequence;
                    xmlSequence.addChild("Sequence");
                    xmlSequence.setTo("Sequence");
                    xmlSequence.addValue("Name", its->first);
                    xmlSequence.addValue<vector<float> >("Values", its->second);
                    xmlSequence.addValue<bool>("Active", itg->second.sequencesActive.count(its->first) > 0 ? itg->second.sequencesActive[its->first] : true);
                    xmlSequenceGroup.addXml(xmlSequence);
                }
                
                xmlSequencerSet.addXml(xmlSequenceGroup);
            }
            xmlSequencerSets.addXml(xmlSequencerSet);
        }
        
        ofXml xmlCurrent;
        xmlCurrent.addChild("Current");
        xmlCurrent.setTo("Current");
        xmlCurrent.addValue<string>("Name", getName());
        xmlCurrent.addValue<bool>("Active", getActive());
        xmlCurrent.addValue<int>("Bpm", getBpm());
        xmlCurrent.addValue<bool>("Smooth", getSmooth());
        xmlCurrent.addValue<int>("Lerp", getLerpNumFrames());
        xmlCurrent.addValue<int>("NumCols", getNumberColumns());
        
        map<GuiElement*,ElementSequenceGroup*>::iterator itg = groups.begin();
        for (; itg != groups.end(); ++itg)
        {
            ofXml xmlSequenceGroup;
            xmlSequenceGroup.addChild("SequenceGroup");
            xmlSequenceGroup.setTo("SequenceGroup");
            
            
            //
            //
            //
            //
            //xmlSequenceGroup.addValue<string>("Name", itg->second->element->getName());
            xmlSequenceGroup.addValue<string>("Name", itg->second->element->getAddress());
            
            
            for (auto &p : itg->second->pairs)
            {
                ofXml xmlSequence;
                xmlSequence.addChild("Sequence");
                xmlSequence.setTo("Sequence");
                
                //
                //
                //
                //
                //
                //
                //xmlSequence.addValue<string>("Name", p->element->getName());
                xmlSequence.addValue<string>("Name", p->element->getAddress());
                xmlSequence.addValue<vector<float> >("Values", p->sequence->getAllValues());
                xmlSequence.addValue<bool>("Active", p->sequence->getActive());
                xmlSequenceGroup.addXml(xmlSequence);
            }
            xmlCurrent.addXml(xmlSequenceGroup);
        }
        
        xmlSequencer.addXml(xmlSequencerSets);
        xmlSequencer.addXml(xmlCurrent);
        
        xml.addXml(xmlSequencer);
    }
    
    void setFromXml(ofXml &xml)
    {
        sequencerSets.clear();
        
        
        // ???
        //
        //
        //
        //mChoose->clearToggles();
        
        if (xml.exists("Sequencer"))
        {
            xml.setTo("Sequencer");
        

            
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
                        
                        if (xml.exists("SequenceGroup[0]"))
                        {
                            xml.setTo("SequenceGroup[0]");
                            do {
                                string sequenceGroupName = xml.getValue<string>("Name");
                                if (xml.exists("Sequence[0]"))
                                {
                                    xml.setTo("Sequence[0]");
                                    do {
                                        string sequenceName = xml.getValue<string>("Name");
                                        bool sequenceActive = xml.getValue<bool>("Active");
                                        string valueStringXml = xml.getValue<string>("Values");
                                        vector<string> valueString = ofSplitString(valueStringXml.substr(1, valueStringXml.size()-2), ",");
                                        vector<float> sequenceValues;
                                        for (auto vs : valueString) sequenceValues.push_back(ofToFloat(vs));
                                        newSet.addSequence(sequenceGroupName, sequenceName, sequenceValues, sequenceActive);
                                    }
                                    while(xml.setToSibling());
                                    xml.setToParent();
                                }
                            }
                            while(xml.setToSibling());
                            xml.setToParent();
                        }

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
                setActive(xml.getValue<bool>("Active"));
                setBpm(xml.getValue<int>("Bpm"));
                setSmooth(xml.getValue<bool>("Smooth"));
                setNumberColumns(xml.getValue<int>("NumCols"));
                lerpNumFrames = xml.getValue<int>("Lerp");
                
                if (xml.exists("SequenceGroup[0]"))
                {
                    xml.setTo("SequenceGroup[0]");
                    do {
                        string sequenceGroupName = xml.getValue<string>("Name");
                        if (xml.exists("Sequence[0]"))
                        {
                            xml.setTo("Sequence[0]");
                            do {
                                string sequenceName = xml.getValue<string>("Name");
                                bool sequenceActive = xml.getValue<bool>("Active");
                                
                                string valueStringXml = xml.getValue<string>("Values");
                                vector<string> valueString = ofSplitString(valueStringXml.substr(1, valueStringXml.size()-2), ",");
                                vector<float> sequenceValues;
                                for (auto vs : valueString) sequenceValues.push_back(ofToFloat(vs));
                                
                                map<GuiElement*,ElementSequenceGroup*>::iterator itg = groups.begin();
                                for (; itg != groups.end(); ++itg)
                                {
                                    //
                                    //
                                    //
                                    //
                                    //
                                    //
                                    //
                                    //string currentSequenceGroupName = itg->second->element->getName();
                                    string currentSequenceGroupName = itg->second->element->getAddress();
                                    for (auto &p : itg->second->pairs)
                                    {
                                        //
                                        //
                                        //
                                        //
                                        //
                                        //
                                        //string currentSequenceName = p->element->getName();
                                        string currentSequenceName = p->element->getAddress();
                                        if (currentSequenceGroupName == sequenceGroupName && currentSequenceName == sequenceName)
                                        {
                                            p->sequence->setFromValues(sequenceValues);
                                            p->sequence->setActive(sequenceActive);
                                        }
                                    }
                                }
                            }
                            while(xml.setToSibling());
                            xml.setToParent();
                        }
                    }
                    while(xml.setToSibling());
                    xml.setToParent();
                }
                xml.setToParent();
            }
            
            
            
            xml.setToParent();
        }
    }

    void setupGuiPositions();
    
protected:
    
    struct SequenceElementPair
    {
        GuiElement *element;
        Sequence *sequence;
        SequenceElementPair(GuiElement *element, int numCols);
        void eventSetSequenceFromKey(Sequence::SequenceKeyboardEventArgs & evt);
    };

    struct ElementSequenceGroup
    {
        GuiElement *element;
        vector<SequenceElementPair*> pairs;
        ElementSequenceGroup(GuiElement *element, int numCols);
        bool isSingleChoiceMenu();
        void randomizeSequencer();
        void menuEvent(GuiSequenceEventArgs & evt);
    };
    
    struct SequencerSavedSet
    {
        string name;
        bool smooth;
        int bpm;        
        int lerpNumFrames;
        int numCols;
        
        
        struct SequencerSavedSetGroup
        {
            map<string, vector<float> > sequences;
            map<string, bool> sequencesActive;
            
            void addSequence(string sequenceName, vector<float> sequence, bool active)
            {
                sequences[sequenceName] = sequence;
                sequencesActive[sequenceName] = active;
            }
        };
        
        map<string, SequencerSavedSetGroup> sequenceGroups;
        
        //map<string, vector<float> > sequences;
        //map<string, bool> sequencesActive;
        
        SequencerSavedSet() { }
        SequencerSavedSet(string name, bool smooth, int bpm, int lerpNumFrames, int numCols);
        
        
        void addSequence(string sequenceGroupName, string sequenceName, vector<float> sequence, bool active);
    };
    
    void setupSequencer();
    void addElement(GuiElement* & newElement);
    void removeElement(GuiElement* & element);
    
    void eventBpm(GuiSliderEventArgs<int> &s);
    void eventNumColumns(GuiSliderEventArgs<int> &s);
    void eventActive(GuiButtonEventArgs &b);
    void eventRandomizeSequencer(GuiButtonEventArgs &b);
    void eventSelectColumn(GuiSliderEventArgs<int> &b);
    void eventSequencerMenuSelection(GuiMenuEventArgs & evt);
    void eventBeat();
    
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
    
    map<GuiElement*,ElementSequenceGroup*> groups;   // needs to be ptr?
    map<string, SequencerSavedSet> sequencerSets;
    GuiWidget * panel;
    
    vector<GuiElement*> buttons;
    GuiMenu *mChoose;
    ofRectangle bToggleAllSequencers;
    vector<ofRectangle> bColumnSelectors;
    int rectSelectColumnMouseOver;
    bool rectToggleAllSeqMouseOver;
    
    int sequencerPanelMargin;
    int sequencerWidth;
    int sequencerButtonHeight;
    int marginInner;
};

