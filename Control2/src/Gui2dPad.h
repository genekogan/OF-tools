#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "GuiMultiElement.h"


struct Gui2dPadPoint
{
    Gui2dPadPoint(Parameter<ofPoint> *parameter);
    ~Gui2dPadPoint();
    
    void setValue(ofPoint padValue);
    void lerpTo(ofPoint nextValue, int numFrames);
    void update();
    void increment(float x, float y);
    string getValueString();
    
    Parameter<ofPoint> *parameter;
    ofPoint previous;
    ofPoint padValue;

    ofPoint lerpPrevValue, lerpNextValue;;
    int lerpFrame, lerpNumFrames;
};


struct Gui2dPadEventArgs
{
    Gui2dPadPoint *point;
    ofPoint padValue;
    
    Gui2dPadEventArgs(Gui2dPadPoint *point, ofPoint padValue)
    {
        this->point = point;
        this->padValue = padValue;
    }
};


class Gui2dPad : public GuiMultiElement
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
    
    
    void setParent(GuiElement *parent);
    
    void setMin(ofPoint min);
    void setMax(ofPoint max);
    ofPoint getMin() {return min;}
    ofPoint getMax() {return max;}
    
    Gui2dPadPoint * addPoint(Parameter<ofPoint> *parameter);
    Gui2dPadPoint * addPoint(ofPoint *value);
    Gui2dPadPoint * addPoint();
    void removePoint(int idx);
    void clearPoints();

    bool getDrawConnectedPoints() {return connectPoints;}
    void setDrawConnectedPoints(bool connectPoints) {this->connectPoints = connectPoints;}
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    ofPoint getValue(int idx) {return points[idx]->padValue;}
    void setValue(int idx, ofPoint padValue);
    
    void lerpTo(int idx, ofPoint nextValue, int numFrames);

    ofPoint getParameterValue(int idx) {return points[idx]->parameter->get();}
    int getNumberOfPoints() {return points.size();}
    void getParameters(vector<ParameterBase*> & parameters);
    
    void update();
    void draw();
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
    ofEvent<Gui2dPadEventArgs> padEvent;
    
protected:
    
    void setupPad(ofPoint min, ofPoint max);
    void updateParameterOscAddress();
    void setupGuiPositions();
    void addElementToTouchOscLayout(TouchOscPage *page, float *y);
    void selectPoint(float x, float y);
    void updateValueString();
    
    vector<Gui2dPadPoint*> points;
    ofPoint min, max;
    
    int idxActive;
    string valueString, valueStringNext;
    bool toUpdateValueString;
    float valueStringWidth, stringHeight;
    bool connectPoints;
    
    ofRectangle padRectangle;
    bool mouseOverPad;
};


template<typename L, typename M>
Gui2dPad::Gui2dPad(Parameter<ofPoint> *parameter, L *listener, M method) : GuiMultiElement(parameter->getName())
{
    setupPad(parameter->getMin(), parameter->getMax());
    addPoint(parameter);
    ofAddListener(padEvent, listener, method);
}

template<typename L, typename M>
Gui2dPad::Gui2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method) : GuiMultiElement(name)
{
    setupPad(min, max);
    addPoint(new Parameter<ofPoint>(name, value, min, max));
    ofAddListener(padEvent, listener, method);
}

template<typename L, typename M>
Gui2dPad::Gui2dPad(string name, ofPoint min, ofPoint max, L *listener, M method) : GuiMultiElement(name)
{
    setupPad(min, max);
    ofAddListener(padEvent, listener, method);
}
