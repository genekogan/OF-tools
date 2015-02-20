#pragma once

#include "ofMain.h"
#include "Parameter.h"
#include "GuiElement.h"


class Sequence : public GuiElement
{
public:
    Sequence(string name, int numCells);
    Sequence(string name);
    ~Sequence();
    
    void setActive(bool active);
    void setDiscrete(bool discrete);
    void setSize(int numCells);
    void setCursor(float cursor);
    
    bool getActive() {return active;}
    bool getDiscrete() {return discrete;}
    int getSize() {return numCells;}
    float getCursor() {return cursor;}
    
    void setValueAtCell(int idx, float value);
    void randomize(float density, float range);
    
    float getValueAtIndex(int idx);
    float getValueAtCurrentIndex();
    float getValueAtCursor(float cursor_);
    float getValueAtCurrentCursor();
    
    ofRectangle getActiveRectangle() {return activeRectangle;}
    ofRectangle getSequenceRectangle() {return sequenceRectangle;}
    
    void mouseMoved(int mouseX, int mouseY);
    void mousePressed(int mouseX, int mouseY);
    void mouseReleased(int mouseX, int mouseY);
    void mouseDragged(int mouseX, int mouseY);
    
    void update();
    void draw();
        
protected:
    
    void setupSequence();
    void setupGuiComponents();

    bool active;
    bool discrete;
    vector<float> values;
    int numCells;
    int activeCell;
    float cellWidth;
    ofPoint mousePos;
    float cursor, cursorLerp;
    ofRectangle activeRectangle, sequenceRectangle;
    bool mouseOverActive, mouseOverSequencer;
};
