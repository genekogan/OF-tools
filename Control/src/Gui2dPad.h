#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "Sequence.h"


class Gui2dPad : public GuiElement
{
public:
    Gui2dPad(Parameter<ofPoint> *parameter);
    Gui2dPad(string name, ofPoint *value, ofPoint min, ofPoint max);
    Gui2dPad(string name, ofPoint min, ofPoint max);

    template<typename L, typename M>
    Gui2dPad(Parameter<ofPoint> *parameter, L *listener, M method);

    template<typename L, typename M>
    Gui2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method);
    
    template<typename L, typename M>
    Gui2dPad(string name, ofPoint min, ofPoint max, L *listener, M method);
    
    ~Gui2dPad();
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    
    void setValue(ofPoint padValue);
    ofPoint getValue() {return padValue;}
    
    ofPoint getParameterValue() {return parameter->get();}
    
    void lerpTo(ofPoint nextValue, int numFrames);
    void setValueFromSequence(Sequence &sequence);
    
    void update();
    void draw();
    
protected:
    
    void setupPad();
    void updateValueString();
    
    Parameter<ofPoint> *parameter;
    ofPoint previous;

    ofPoint padValue;
    string valueString, valueStringNext;
    ofPoint lerpPrevValue, lerpNextValue;;
    int lerpFrame, lerpNumFrames;
    bool toUpdateValueString;
    float valueStringWidth, stringHeight;
};


template<typename L, typename M>
Gui2dPad::Gui2dPad(Parameter<ofPoint> *parameter, L *listener, M method) : GuiElement(parameter->getName())
{
    this->parameter = parameter;
    setupPad();
    ofAddListener(elementEvent, listener, method);
}

template<typename L, typename M>
Gui2dPad::Gui2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<ofPoint>(name, value, min, max);
    setupPad();
    ofAddListener(elementEvent, listener, method);
}

template<typename L, typename M>
Gui2dPad::Gui2dPad(string name, ofPoint min, ofPoint max, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<ofPoint>(name, new ofPoint(0.5 * (min.x + max.x), 0.5 * (min.y + max.y)), min, max);
    setupPad();
    ofAddListener(elementEvent, listener, method);
}
