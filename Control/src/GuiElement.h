#pragma once

#include "ofMain.h"
#include "GuiConstants.h"


class Sequence;


struct GuiElementEventArgs
{
    string name;
    float value;
    int cell;
    
    GuiElementEventArgs(string name, int cell, float value)
    {
        this->name = name;
        this->cell = cell;
        this->value = value;
    }
};


class GuiElement
{
public:
    GuiElement(string name);
    GuiElement();
    
    void setParent(GuiElement *parent);
    GuiElement * getParent() {return parent;}
    bool getHasParent() {return hasParent;}
    
    virtual ~GuiElement();
    
    virtual void setAutoUpdate(bool autoUpdate);
    virtual void setAutoDraw(bool autoDraw);
    
    virtual void setName(string name) {this->name = name;}
    virtual string getName() {return name;}
    
    virtual bool getMouseOver() {return mouseOver;}
    virtual void setMouseOver(bool mouseOver);
    
    virtual bool mouseMoved(int mouseX, int mouseY);
    virtual bool mousePressed(int mouseX, int mouseY);
    virtual bool mouseDragged(int mouseX, int mouseY);
    virtual bool mouseReleased(int mouseX, int mouseY);

    virtual bool keyPressed(int key);
    
    virtual void update() { }
    virtual void draw() { }
    
    virtual bool isWidget() {return false;}
    virtual bool isDiscrete() {return false;}
    
    virtual void lerpTo(float nextSliderValue, int lerpNumFrames) { }
    virtual void setValueFromSequence(Sequence &sequence) { }
    virtual void setSequenceFromValue(Sequence &sequence, int column) { }
    
    virtual void setRectangle(ofRectangle rectangle);
    virtual void setRectangle(int x, int y, int width, int height);
    virtual void setPosition(ofPoint p);
    virtual void setPosition(int x, int y);
    virtual void setSize(ofPoint s);
    virtual void setSize(int width, int height);

    virtual void setElementWidth(int elementWidth) {this->elementWidth = elementWidth;}
    virtual void setElementHeight(int elementHeight) {this->elementHeight = elementHeight;}
    virtual void setColorBackground(ofColor colorBackground) {this->colorBackground = colorBackground;}
    virtual void setColorForeground(ofColor colorForeground) {this->colorForeground = colorForeground;}
    virtual void setColorOutline(ofColor colorOutline) {this->colorOutline = colorOutline;}
    virtual void setColorText(ofColor colorText) {this->colorText = colorText;}
    virtual void setColorActive(ofColor colorOutline) {this->colorOutline = colorOutline;}
    
    ofRectangle getRectangle() {return rectangle;}
    int getElementWidth() {return elementWidth;}
    int getElementHeight() {return elementHeight;}
    ofColor getColorBackground() {return colorBackground;}
    ofColor getColorForeground() {return colorForeground;}
    ofColor getColorOutline() {return colorOutline;}
    ofColor getColorText() {return colorText;}
    ofColor getColorActive() {return colorActive;}
    
    ofEvent<GuiElementEventArgs> elementEvent;
    
protected:
    
    virtual void initialize();
    virtual void setupGuiComponents() { }
    
    void mouseMoved(ofMouseEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    
    void keyPressed(ofKeyEventArgs &evt);
    
    void update(ofEventArgs &data);
    void draw(ofEventArgs &data);
    
    string name;
    bool autoUpdate, autoDraw;
    bool mouseOver, mouseDragging;
    
    GuiElement *parent;
    bool hasParent;
    
    ofRectangle rectangle;
    int elementWidth;
    int elementHeight;
    ofColor colorForeground;
    ofColor colorBackground;
    ofColor colorOutline;
    ofColor colorText;
    ofColor colorActive;
};
