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
    GuiMidiEventArgs(int type, int note, int velocity) {
        this->type = type;
        this->note = note;
        this->velocity = velocity;
    }
};


class MidiSequencerEvent : public GuiElement
{
public:
    MidiSequencerEvent(int row, float t1, float t2, int velocity) : GuiElement("")
    {
        this->t1 = t1;
        this->t2 = t2;
        this->row = row;
        this->velocity = velocity;
        
        setAutoDraw(false);
        setAutoUpdate(false);
        
        expired = false;
        on = false;
    }
    
    void setBeginning(float t1) {
        this->t1 = t1;
    }

    void setEnd(float t2) {
        this->t2 = t2;
        cout << "set t1 " << t1 << " " << t2 << endl;
    }

    void setRow(float row) {
        this->row = row;
    }
    
    void setVelocity(float velocity) {
        this->velocity = velocity;
    }
    
    float getBeginning() {return t1;}
    float getEnd() {return t2;}
    int getRow() {return row;}
    int getVelocity() {return velocity;}

    void setOn(bool on) {this->on = on;}
    bool getOn() {return on;}
    
    void checkNote(double t) {

        if (!expired) {
            if (t >= t1 && t <= t2) {
                cout << "FIRE NOTE " << row << " " <<velocity << endl;
                on = true;
            }
            else {
                if (on) {
                    cout << "DONE " << row << " " <<velocity << endl;
                    on = false;
                    expired = true;
                }
            }
        }
    }
    
    void draw()
    {
        ofRect(rectangle);
    }

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);

    float t1, t2;
    int row;
    int velocity;
    
    bool on, expired;
};


class MidiSequencer : public GuiWidgetBase
{
public:
    MidiSequencer(string name);
    ~MidiSequencer();
    
    
    void update();
    void draw();
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    
    ofEvent<GuiMidiEventArgs> midiEvent;
    
private:
    
    void setupGuiComponents();
    
    
    long time;
    long period;
    double tt;
    
    
    int rows;
    float rowHeight;
    
    
    vector<MidiSequencerEvent*> events;
    MidiSequencerEvent *newEvent;
    
    
    
};

