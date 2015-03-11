#pragma once

#include "ofMain.h"
#include "GuiSlider.h"
#include "GuiWidgetBase.h"
#include "GuiMenu.h"
#include "Bpm.h"
#include "Sequence.h"



struct GuiMidiEventArgs
{
    int type;
    int note;
    int velocity;
    GuiMidiEventArgs(int type, int note, int velocity)
    {
        this->type = type;
        this->note = note;
        this->velocity = velocity;
    }
};


class MidiSequencerEvent : public GuiElement
{
public:
    MidiSequencerEvent(int row, int velocity, int start, int end);
    
    void setRow(float row) {this->row = row;}
    void setVelocity(float velocity) {this->velocity = velocity;}
    void setStart(int start) {this->start = start;}
    void setEnd(int end) {this->end = end;}

    int getRow() {return row;}
    int getVelocity() {return velocity;}
    int getStart() {return start;}
    int getEnd() {return end;}

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);

    void draw();
    
private:
    
    int start;
    int end;
    int row;
    int velocity;
};


class MidiSequencer : public GuiWidgetBase
{
public:
    MidiSequencer(string name);
    ~MidiSequencer();
    
    void setActive(bool active);
    void setNumberRows(int rows);
    void setNumberBeats(int numBeats);
    
    void addMidiEvent(int row, int velocity, int start, int end);
    void removeEvent(MidiSequencerEvent *event);
    
    void update();
    void draw();
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    ofEvent<GuiMidiEventArgs> midiEvent;

private:
    
    void eventBeat();
    void setupGuiComponents();
    
    void setMidiEventRectangle(MidiSequencerEvent* event);
    
    int rows;
    float rowHeight;
    
    MidiSequencerEvent *newEvent;
    MidiSequencerEvent *selectedEvent;
    vector<MidiSequencerEvent*> events;
    vector<vector<MidiSequencerEvent*> > midiOnEvents;
    vector<vector<MidiSequencerEvent*> > midiOffEvents;
    
    Bpm clock;
    float dt = 0.1f;
    float period = 36.0f;
    int numBeats;
    int beat;
    bool active;
};

