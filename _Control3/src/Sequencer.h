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

    
    void update();
    void draw();
    
protected:
    
    
    struct SequenceElementPair
    {
        GuiElement *element;
        vector<Sequence*> sequences;

        int numCols = 8;
        
        SequenceElementPair(GuiElement *element)
        {
            this->element = element;
            if (element->isMultiElement())
            {
                for (auto e : ((GuiMultiElement *) element)->getElements())
                {
                    Sequence *sequence = new Sequence(e->getName(), numCols);
                    sequence->setAutoUpdate(false);
                    sequence->setAutoDraw(false);
                    //sequence->setDiscrete(element->isDiscrete());
                    sequences.push_back(sequence);
                    //ofAddListener(sequence->keyboardEvent, this, &SequenceElementPair::eventSetSequenceFromKey);

                }
            }
            else {
                sequences.push_back(new Sequence(element->getName(), numCols));
            }
        }
        
        

        
    };
    
    
    
    void setupSequencer();
    void setupGuiPositions();
    void setupSequencesFromPanel();
    
    void setAllSequencersActive(bool allSequencesActive);
    void randomizeSequencer();
    void selectColumn(int column);
    void setColumnToCurrentValues(int column);
    void next();
    
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
    
    vector<SequenceElementPair*> sequencePairs;
    //map<string, SequencerSavedSet> sequencerSets;
    GuiWidget * panel;
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

