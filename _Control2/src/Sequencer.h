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
    void setNumberColumns(int numCols);

    bool getActive() {return active;}
    bool getSmooth() {return smooth;}
    int getBpm() {return bpm;}
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
    
protected:
    
    // encapsulate double loops into SequenceElementPair
    // check if element active before doing anything
    // guiupdatepositions when collapsed (panel->sequencer->setupguipositions if hasseq)
    // parent of sequences?
    
    struct SequenceElementPair
    {
        GuiElement *element;
        Sequence *sequence;
        SequenceElementPair(GuiElement *element)
        {
            this->element = element;
            sequence = new Sequence(element->getName(), 8); //-----
            sequence->setAutoUpdate(false);
            sequence->setAutoDraw(false);
            
            // parent?
            sequence->setDiscrete(element->isDiscrete());
            // discrete?
            
            //sequence->setDiscrete(element->isDiscrete());
        }
    };

    struct ElementSequenceGroup
    {
        GuiElement *element;
        vector<SequenceElementPair*> pairs;
        int numCols = 8;    // numCols?
        
        ElementSequenceGroup(GuiElement *element)
        {
            this->element = element;
            if (element->isMultiElement())
            {
                for (auto e : ((GuiMultiElement *) element)->getElements()) {
                    pairs.push_back(new SequenceElementPair(e));
                }
            }
            else {
                pairs.push_back(new SequenceElementPair(element));
            }
        }

        void update()
        {
            for (auto p : pairs) {
                p->sequence->update();
            }
        }

        void draw()
        {
            for (auto p : pairs) {
                p->sequence->draw();
            }
        }
        
        bool mouseMoved(int mouseX, int mouseY)
        {
            bool mouse = false;
            for (auto p : pairs) {
                mouse = p->sequence->mouseMoved(mouseX, mouseY) ? true : mouse;
            }
            return mouse;
        }
        
        bool mousePressed(int mouseX, int mouseY)
        {
            bool mouse = false;
            for (auto p : pairs)
            {
                if (p->sequence->getMouseOver()) {
                    mouse = p->sequence->mousePressed(mouseX, mouseY) ? true : mouse;
                }
            }
            return mouse;
        }
        
        bool mouseDragged(int mouseX, int mouseY)
        {
            bool mouse = false;
            for (auto p : pairs)
            {
                if (p->sequence->getMouseOver()) {
                    mouse = p->sequence->mouseDragged(mouseX, mouseY) ? true : mouse;
                }
            }
            return mouse;
        }
        
        bool mouseReleased(int mouseX, int mouseY)
        {
            bool mouse = false;
            for (auto p : pairs)
            {
                if (p->sequence->getMouseOver()) {
                    mouse = p->sequence->mouseReleased(mouseX, mouseY) ? true : mouse;
                }
            }
            return mouse;
        }
        
        bool keyPressed(int key)
        {
            bool mouse = false;
            for (auto p : pairs)
            {
                if (p->sequence->getMouseOver()) {
                    mouse = p->sequence->keyPressed(key) ? true : mouse;
                }
            }
            return mouse;
        }
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
    
    
    
    
    void setupSequencer();
    void newElementEvent(GuiElement* & newElement);     // rename?
    void removeElementEvent(GuiElement* & element);     // rename?
    void setupGuiPositions();
    
    void eventBpm(GuiSliderEventArgs<int> &s);
    void eventNumColumns(GuiSliderEventArgs<int> &s);
    void eventActive(GuiButtonEventArgs &b);
    void eventWidgetCollapsed(string &s);
    void eventRandomizeSequencer(GuiButtonEventArgs &b);
    void eventSelectColumn(GuiSliderEventArgs<int> &b);
    void eventBeat();
    void eventSequencerMenuSelection(GuiMenuEventArgs & evt);
    //void eventElementDeleted(GuiElement * &elementToDelete);

    

    
    
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
    
    map<GuiElement*,ElementSequenceGroup*> groups;   // needs to be ptrr?
    map<string, SequencerSavedSet> sequencerSets;
    GuiWidget * panel;
    ofRectangle sequencerRect;      // unuused
    
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
    int sequencerButtonHeight;
    int marginInner;
};

