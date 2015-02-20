#pragma once

#include "ofMain.h"
#include "GuiPanel.h"
#include "Bpm.h"



class Sequencer : public GuiWidget
{
public:
    Sequencer(string name, GuiPanel * panel, int numCols=8);
    ~Sequencer();
    
    void setActive(bool active);
    void setSmooth(bool smooth);
    void setBpm(int bpm);
    void setNumberColumns(int numColumns);
    
    bool getActive() {return active;}
    bool getSmooth() {return smooth;}
    int getBpm() {return bpm;}
    int getNumberColumns() {return numColumns;}
    
    void randomizeSequencer();
    void selectColumn(int column);
    void next();
    
    void mouseMoved(int mouseX, int mouseY);
    void mousePressed(int mouseX, int mouseY);
    void mouseReleased(int mouseX, int mouseY);
    void mouseDragged(int mouseX, int mouseY);
    
    void update();
    void draw();
    
private:
    
    struct SequenceElementPair
    {
        SequenceElementPair(GuiElement *element, int numColumns);
        SequenceElementPair(int numCols);
        ~SequenceElementPair();
        
        void updateElementFromSequencer();
        void lerpWidgetToSequencer(int lerpNumFrames);
        
        GuiElement *element;
        Sequence *sequence;
        bool linkedToElement;
    };
    
    struct SequenceWidgetPair
    {
        SequenceWidgetPair(GuiWidget *widget, int numColumns);
        SequenceWidgetPair(int numCols);
        ~SequenceWidgetPair();
        
        GuiWidget * getWidget() {return widget;}
        
        void setNumberColumns(int numColumns);
        void setCursor(float cursor);
        
        void randomizeSequence(float density, float range);
        void updateWidgetFromSequencer();
        void lerpWidgetToSequencer(int lerpNumFrames);
        
        void update();
        void draw();
        
        void mouseMoved(int mouseX, int mouseY);
        void mousePressed(int mouseX, int mouseY);
        void mouseReleased(int mouseX, int mouseY);
        void mouseDragged(int mouseX, int mouseY);
        
        void menuSequenceChanged(GuiElementEventArgs & b);
        
        GuiWidget *widget;
        GuiMenu *menu;
        vector<SequenceElementPair*> elementPairs;
    };
    
    void setupSequencer();
    void setupGuiComponents();
    void setupGuiSequencesFromPanel();
    void setupGuiControls();
    void toggleSequencersActive();
    void drawControls();
    void randomizeSequencerForMenu(SequenceWidgetPair * w);

    void eventBpm(GuiElementEventArgs &s);
    void eventNumColumns(GuiElementEventArgs &s);
    void eventActive(GuiElementEventArgs &b);
    void eventWidgetCollapsed(string &s);
    void eventRandomizeSequencer(GuiElementEventArgs &b);
    void eventSelectColumn(GuiElementEventArgs & b);
    void eventBeat();
    
    // panel
    GuiPanel *panel;
    ofRectangle sequenceRectangle;
    ofRectangle controlRectangle;
    bool linkedToPanel;
    
    // bpm
    Bpm clock;
    int bpm;
    int numColumns, currentColumn;
    bool active;
    bool smooth;
    float bpmTime, bpmInterval;
    float cursor;
    
    // sequencer controls
    vector<SequenceWidgetPair*> widgetPairs;
    GuiToggle *tActive, *tSmooth;
    GuiSlider<int> *sBpm, *sNumColumns, *sLerpFrames;
    GuiSlider<float> *sDensity, *sRange;
    GuiButton *bRandomize;
    GuiMenu *mChooseSeq;
    vector<ofRectangle> rectSelectColumn;
    ofRectangle rectTopSeq;
    ofRectangle rectToggleAllSeq;
    int rectSelectColumnMouseOver;
    int lerpNumFrames;
    float randomDensity, randomRange;
    bool allSequencesActive;
    bool rectToggleAllSeqMouseOver;
};

