#pragma once

#include "ofMain.h"
#include "GuiSlider.h"
#include "GuiWidget.h"
#include "GuiMenu.h"
#include "Bpm.h"
#include "Sequence.h"



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
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);

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
        struct SequencerSavedSetGroup
        {
            map<string, vector<float> > sequences;
            map<string, bool> sequencesActive;
            void addSequence(string sequenceName, vector<float> sequence, bool active);
        };
        
        string name;
        bool smooth;
        int bpm;
        int lerpNumFrames;
        int numCols;
        map<string, SequencerSavedSetGroup> sequenceGroups;

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
    
    map<GuiElement*,ElementSequenceGroup*> groups;
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

