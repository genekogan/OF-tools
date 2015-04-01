#pragma once

#include "ofMain.h"
#include "GuiConstants.h"



class GuiBase
{
public:
    GuiBase(string name);
    GuiBase();
    virtual ~GuiBase();
    
    virtual void setName(string name) {this->name = name;}
    string getName() {return name;}
    
    void setActive(bool active);
    void setAutoUpdate(bool autoUpdate);
    void setAutoDraw(bool autoDraw);
    bool getActive() {return active;}
    
    void setWidth(int width);
    void setHeight(int height);
    void setSize(int width, int height);
    void setSize(ofPoint s) {setSize(s.x, s.y);}
    void setX(int x);
    void setY(int y);
    void setPosition(int x, int y);
    void setPosition(ofPoint p) {setPosition(p.x, p.y);}
    void setRectangle(int x, int y, int width, int height) {setRectangle(ofRectangle(x, y, width, height));}
    void setRectangle(ofRectangle rectangle);
    
    bool getMouseOver() {return mouseOver;}
    void setMouseOver(bool mouseOver);
    
    virtual bool mouseMoved(int mouseX, int mouseY);
    virtual bool mousePressed(int mouseX, int mouseY);
    virtual bool mouseDragged(int mouseX, int mouseY);
    virtual bool mouseReleased(int mouseX, int mouseY);
    
    virtual bool keyPressed(int key);
    
    virtual void update();
    virtual void draw();
    
    void setColorBackground(ofColor colorBackground) {this->colorBackground = colorBackground;}
    void setColorForeground(ofColor colorForeground) {this->colorForeground = colorForeground;}
    void setColorOutline(ofColor colorOutline) {this->colorOutline = colorOutline;}
    void setColorText(ofColor colorText) {this->colorText = colorText;}
    void setColorActive(ofColor colorOutline) {this->colorOutline = colorOutline;}
    
    ofColor getColorBackground() {return colorBackground;}
    ofColor getColorForeground() {return colorForeground;}
    ofColor getColorOutline() {return colorOutline;}
    ofColor getColorText() {return colorText;}
    ofColor getColorActive() {return colorActive;}
    
    ofRectangle getRectangle() {return rectangle;}
    int getWidth() {return width;}
    int getHeight() {return height;}
    
    virtual void setupGuiPositions();
    
protected:
    
    virtual void initialize();
    
    void mouseMoved(ofMouseEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    
    void keyPressed(ofKeyEventArgs &evt);
    
    void update(ofEventArgs &data);
    void draw(ofEventArgs &data);
    
    string name;
    
    bool active, autoUpdate, autoDraw;
    bool mouseOver, mouseDragging;
    
    ofRectangle rectangle;
    int x, y;
    int width, height;
    
    ofColor colorForeground;
    ofColor colorBackground;
    ofColor colorOutline;
    ofColor colorText;
    ofColor colorActive;
};
