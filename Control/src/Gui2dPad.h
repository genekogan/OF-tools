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
    
    void addPoint(Parameter<ofPoint> *parameter);
    void addPoint(ofPoint *value);
    void addPoint();
    
    bool getDrawConnectedPoints() {return connectPoints;}
    void setDrawConnectedPoints(bool connectPoints) {this->connectPoints = connectPoints;}

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void setValue(int idx, ofPoint padValue);
    ofPoint getValue(int idx) {return padValue[idx];}
    
    ofPoint getParameterValue(int idx) {return parameters[idx]->get();}
    int getNumberOfPoints() {return parameters.size();}
    
    void lerpTo(int idx, ofPoint nextValue, int numFrames);
    
    void update();
    void draw();
    
    
protected:
    
    void setupPad();
    void choosePoint(float x, float y);
    void updateValueString();
    
    vector<Parameter<ofPoint>*> parameters;
    vector<ofPoint> previous;
    vector<ofPoint> padValue;
    ofPoint min, max;
    
    int idxActive;
    string valueString, valueStringNext;
    ofPoint lerpPrevValue, lerpNextValue;;
    int idxLerp, lerpFrame, lerpNumFrames;
    bool toUpdateValueString;
    float valueStringWidth, stringHeight;
    bool connectPoints;
};


template<typename L, typename M>
Gui2dPad::Gui2dPad(Parameter<ofPoint> *parameter, L *listener, M method) : GuiElement(parameter->getName())
{
    parameters.push_back(parameter);
    padValue.push_back(ofPoint(ofClamp((parameter->get().x - parameter->getMin().x) / (parameter->getMax().x - parameter->getMin().x), 0.0, 1.0),
                               ofClamp((parameter->get().y - parameter->getMin().y) / (parameter->getMax().y - parameter->getMin().y), 0.0, 1.0)));
    previous.push_back(parameter->get());
    setupPad();
    ofAddListener(elementEvent, listener, method);
}

template<typename L, typename M>
Gui2dPad::Gui2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method) : GuiElement(name)
{
    Parameter<ofPoint> *parameter = new Parameter<ofPoint>(name, value, min, max);
    parameters.push_back(parameter);
    padValue.push_back(ofPoint(ofClamp((parameter->get().x - parameter->getMin().x) / (parameter->getMax().x - parameter->getMin().x), 0.0, 1.0),
                               ofClamp((parameter->get().y - parameter->getMin().y) / (parameter->getMax().y - parameter->getMin().y), 0.0, 1.0)));
    previous.push_back(parameter->get());
    setupPad();
    ofAddListener(elementEvent, listener, method);
}

template<typename L, typename M>
Gui2dPad::Gui2dPad(string name, ofPoint min, ofPoint max, L *listener, M method) : GuiElement(name)
{
    this->min = min;
    this->max = max;
    setupPad();
    ofAddListener(elementEvent, listener, method);
}
