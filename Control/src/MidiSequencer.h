#pragma once

#include "ofMain.h"
#include "GuiSlider.h"
#include "GuiRangeSlider.h"
#include "GuiMenu.h"
#include "GuiWidget.h"
#include "Bpm.h"
#include "Sequence.h"


struct MidiEventArgs
{
    int type;
    int note;
    int velocity;
    MidiEventArgs(int type, int note, int velocity);
};

class MidiSequencer : public GuiWidget
{
public:

    class MidiSequencerEvent : public GuiElement
    {
    public:
        MidiSequencerEvent(int note, int velocity, int start, int end);
        void draw();
        int start, end;
        int note, velocity;
    };

    MidiSequencer(string name);
    ~MidiSequencer();
    
    void setBpm(int bpm);
    void setPeriod(int measures);
    void setActive(bool active);
    void setNoteDisplayRange(int noteMin, int noteMax);
    void setTimeDisplayRange(float start, float end);

    void draw();

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    ofEvent<MidiEventArgs> midiEvent;

private:

    enum MidiEventMouseMode { MIDDLE, LEFT, RIGHT, TOP, BOTTOM };
    
    MidiSequencerEvent * addMidiEvent(int row, int velocity, int start, int end);
    void removeSelectedEvents();
    void removeEvent(MidiSequencerEvent *event, bool toDelete=true);
    void repositionMidiEvent(MidiSequencerEvent* event);
    
    void setNumberBeats(int numBeats);
    
    void setupKeys();
    void setupGuiPositions();
    void setMidiEventRectangle(MidiSequencerEvent *event);
    
    void drawGrid();
    void drawKeyboard();
    void drawVelocity();
    void drawToolbar();
    
    void eventBeat();
    void eventSelectRoot(GuiMenuEventArgs &e);
    void eventSelectKey(GuiMenuEventArgs &e);
    void eventPlay(GuiButtonEventArgs &e);
    void eventPause(GuiButtonEventArgs &e);
    void eventStop(GuiButtonEventArgs &e);
    void eventSetBpm(GuiSliderEventArgs<int> &e);
    void eventSetPeriod(GuiSliderEventArgs<int> &e);
    void eventSetTimeView(GuiRangeSliderEventArgs<float> &e);
    
    GuiMenu *keySelect;
    GuiMenu *rootSelect;
    GuiButton *play, *pause, *stop, *clear;
    GuiSlider<int> *sBpm, *sPeriod;
    GuiRangeSlider<float> *rTime;
    
    MidiSequencerEvent *newEvent;
    MidiSequencerEvent *selectedEvent, *mouseOverEvent;
    vector<MidiSequencerEvent*> events;
    vector<vector<MidiSequencerEvent*> > midiOnEvents;
    vector<vector<MidiSequencerEvent*> > midiOffEvents;
    
    int numRows;
    float rowHeight, colWidth;
    int noteMin, noteMax;
    float start, end;

    int root, key;
    map<int, bool> rootIsMajor;
    map<int, bool> rootIsMinor;
    
    Bpm clock;
    int bpm;
    int measures;
    int numBeats;
    int beat;
    bool active;
    
    bool shift;
    ofPoint pMouse;
    int pNoteMin, pNoteMax;
    float pStart, pEnd;
    int selectedNote;
    bool noteSelected;
    MidiEventMouseMode mouseOverEventMode;
    ofPoint pMouseOverEventNoteVelocity;
    ofPoint pMouseOverEventStartEnd;
    ofRectangle selection;
    
    ofRectangle rectGrid;
    ofRectangle rectKeyboard;
    ofRectangle rectVelocity;
    ofRectangle rectToolbar;
    ofRectangle rectTimebar;
    bool mouseOverGrid;
    bool mouseOverKeyboard;
    bool mouseOverVelocity;
    int keyboardSkip;
    int keyboardWidth;
    int velocityHeight;
    int toolbarHeight;
    int timeScrollHeight;
};

