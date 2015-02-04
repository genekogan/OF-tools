#pragma once

#include "ofMain.h"
#include "GuiConstants.h"


class GuiElement
{
public:
    GuiElement(string name);
    ~GuiElement();
    
    virtual void setAutoUpdate(bool autoUpdate);
    virtual void setAutoDraw(bool autoDraw);
    
    void setName(string name) {this->name = name;}
    virtual void setRectangle(ofRectangle rectangle);
    virtual void setRectangle(int x, int y, int w, int h) {setRectangle(ofRectangle(x, y, w, h));}
    virtual void setPosition(int x, int y);
    
    virtual void setupGuiComponents() { }
    
    string getName() {return name;}
    ofRectangle getRectangle() {return rectangle;}
    
    void setColorFore(ofColor colorFore) {this->colorFore = colorFore;}
    void setColorBack(ofColor colorBack) {this->colorBack = colorBack;}
    void setColorActive(ofColor colorActive) {this->colorActive = colorActive;}
    void setColorText(ofColor colorText) {this->colorText = colorText;}
    void setLineWidthActive(float lineWidthActive) {this->lineWidthActive = lineWidthActive;}
    
    ofColor getColorFore() {return colorFore;}
    ofColor getColorBack() {return colorBack;}
    ofColor getColorActive() {return colorActive;}
    ofColor getColorText() {return colorText;}
    float getLineWidthActive() {return lineWidthActive;}
    
    void setExtraMargin(bool extraMargin) {this->extraMargin = extraMargin;}
    bool getExtraMargin() {return extraMargin;}
    
    void mouseMoved(ofMouseEventArgs &evt) {mouseMoved(evt.x, evt.y);}
    void mousePressed(ofMouseEventArgs &evt) {mousePressed(evt.x, evt.y);}
    void mouseDragged(ofMouseEventArgs &evt) {mouseDragged(evt.x, evt.y);}
    void mouseReleased(ofMouseEventArgs &evt) {mouseReleased(evt.x, evt.y);}
    
    virtual void mouseMoved(int mouseX, int mouseY);
    virtual void mousePressed(int mouseX, int mouseY);
    virtual void mouseDragged(int mouseX, int mouseY);
    virtual void mouseReleased(int mouseX, int mouseY);

    void update(ofEventArgs &data) {update();}
    virtual void update() {}

    void draw(ofEventArgs &data) {draw();}
    virtual void draw() {}

protected:
    
    string name;
    ofRectangle rectangle;
    bool autoUpdate, autoDraw;
    bool active;
    bool dragging;
    bool extraMargin;
    
    ofColor colorFore;
    ofColor colorBack;
    ofColor colorActive;
    ofColor colorText;
    float lineWidthActive;
};
