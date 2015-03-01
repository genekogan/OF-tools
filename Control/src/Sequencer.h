#pragma once

#include "ofMain.h"
#include "GuiSlider.h"
#include "GuiWidgetBase.h"
#include "GuiMenu.h"
#include "Bpm.h"
#include "Sequence.h"


class Sequencer : public GuiWidgetBase
{
public:
    Sequencer(string name, GuiWidgetBase * panel, int numCols=8);
    ~Sequencer();
    
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
    
private:
    
    class SequenceElementPair
    {
    public:
        SequenceElementPair(GuiElement *element);
        ~SequenceElementPair();
        
        Sequence * getSequence() {return sequence;}
        GuiElement * getElement() {return element;}
        
        void updateElementFromSequencer();
        void lerpWidgetToSequencer(int lerpNumFrames);
    
    private:
        
        Sequence *sequence;
        GuiElement *element;
        bool linked;
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
    void setupGuiComponents();
    void setupSequencesFromPanel();
    
    void setAllSequencersActive(bool allSequencesActive);
    void randomizeSequencer();
    void selectColumn(int column);
    void next();

    void eventSequencerMenuSelection(GuiElementEventArgs & evt);
    void eventBpm(GuiElementEventArgs &s);
    void eventNumColumns(GuiElementEventArgs &s);
    void eventActive(GuiElementEventArgs &b);
    void eventWidgetCollapsed(string &s);
    void eventRandomizeSequencer(GuiElementEventArgs &b);
    void eventSelectColumn(GuiElementEventArgs &b);
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

