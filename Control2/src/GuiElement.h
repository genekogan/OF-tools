#pragma once

#include "ofMain.h"
#include "GuiConstants.h"


class Sequence;


class GuiElementBase
{
public:
    GuiElementBase(string name);
    ~GuiElementBase();
    
    virtual void setAutoUpdate(bool autoUpdate);
    virtual void setAutoDraw(bool autoDraw);
    
    virtual void setName(string name);
    virtual string getName();
    
    virtual void setRectangle(ofRectangle rectangle);
    virtual void setRectangle(int x, int y, int width, int height);
    virtual void setPosition(ofPoint p);
    virtual void setPosition(int x, int y);
    virtual ofRectangle getRectangle();
    
    virtual void mouseMoved(int mouseX, int mouseY);
    virtual void mousePressed(int mouseX, int mouseY);
    virtual void mouseDragged(int mouseX, int mouseY);
    virtual void mouseReleased(int mouseX, int mouseY);
    
    virtual void update() { }
    virtual void draw() { }
    
protected:
    
    virtual void setupGuiComponents() { }
    
    void mouseMoved(ofMouseEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    
    void update(ofEventArgs &data);
    void draw(ofEventArgs &data);
    
    string name;
    ofRectangle rectangle;
    bool autoUpdate, autoDraw;
    bool mouseOver, mouseDragging;
};



class GuiElement : public GuiElementBase
{
public:
    GuiElement(string name);

    virtual void setExtraMargin(bool extraMargin);
    virtual bool getExtraMargin();
    virtual bool isDiscrete();
    
    virtual void setValueFromSequence(Sequence &sequence) { }
    
    GuiStyle & getStyle();
    
protected:
    
    GuiStyle style;
    bool extraMargin;
};
