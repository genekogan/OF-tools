#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiWidget.h"



class GuiColor : public GuiElement
{
public:
    
    GuiColor(string name, Parameter<ofFloatColor> *parameter);
    GuiColor(string name, ofFloatColor *value);
    //GuiColor() : GuiElement() { }
    
    void setupColor();
    
    void mouseMoved(int mouseX, int mouseY);
    void mousePressed(int mouseX, int mouseY);
    void mouseReleased(int mouseX, int mouseY);
    void mouseDragged(int mouseX, int mouseY);
    
    Parameter<ofFloatColor> * getParameter() {return parameter;}
    
    void setAutoUpdate(bool autoUpdate);
    void setAutoDraw(bool autoDraw);
    void setupGuiComponents();
    
    void update();
    void draw();
    
private:
    
    Parameter<ofFloatColor> *parameter;
    vector<GuiSlider<float>*> channels;
};



class GuiColorEntry : public GuiWidgetBase
{
public:

    GuiColorEntry(string name, Parameter<ofFloatColor> *parameter);
    GuiColorEntry(string name, ofFloatColor *color);

    void setupColor();

    ofPoint getGuiElementSize();
    
private:
    
    GuiColor *color;
    Parameter<ofFloatColor> *parameter;
};
