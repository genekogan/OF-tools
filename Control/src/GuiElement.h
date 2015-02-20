#pragma once

#include "ofMain.h"
#include "GuiConstants.h"


class Sequence;


class GuiElement
{
public:
    GuiElement(string name);
    virtual ~GuiElement();
    
    virtual void setAutoUpdate(bool autoUpdate);
    virtual void setAutoDraw(bool autoDraw);
    
    virtual void setName(string name);
    virtual string getName();
    
    virtual void lerpTo(float nextSliderValue, int lerpNumFrames) { }
    
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
    
    virtual void setExtraMargin(bool extraMargin);
    virtual bool getExtraMargin();
    virtual bool isDiscrete() {return false;}
    
    virtual void setValueFromSequence(Sequence &sequence) { }
    
    GuiStyle & getStyle();
    
    ofEvent<GuiElementEventArgs> elementEvent;
    
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
    GuiStyle style;
    bool autoUpdate, autoDraw;
    bool mouseOver, mouseDragging;
    bool extraMargin;
};