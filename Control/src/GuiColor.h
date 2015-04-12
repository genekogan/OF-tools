#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "GuiSlider.h"
#include "GuiMultiElement.h"


class GuiColor;

struct GuiColorEventArgs
{
    GuiColor *slider;
    ofFloatColor color;
    
    GuiColorEventArgs(GuiColor *slider, ofFloatColor color)
    {
        this->slider = slider;
        this->color = color;
    }
};


class GuiColor : public GuiMultiElement
{
public:
    void getParameters(vector<ParameterBase*> & parameters) {
        parameters.push_back(parameter);
    }

    GuiColor(Parameter<ofFloatColor> *parameter);
    GuiColor(string name, ofFloatColor *color);
    GuiColor(string name);
    
    template <typename L, typename M>
    GuiColor(Parameter<ofFloatColor> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    GuiColor(string name, ofFloatColor *color, L *listener, M method);
    
    template <typename L, typename M>
    GuiColor(string name, L *listener, M method);
    
    ~GuiColor();
    
    void setParameterValue(ofFloatColor color) {parameter->set(color);}
    ofFloatColor getParameterValue() {return parameter->get();}
    
    void update();
    
    ofEvent<GuiColorEventArgs> colorEvent;
    
private:
    
    void setupColor();
    void sliderChanged(GuiSliderEventArgs<float> &e);
    
    Parameter<ofFloatColor> *parameter;
};


template<typename L, typename M>
GuiColor::GuiColor(Parameter<ofFloatColor> *parameter, L *listener, M method) : GuiMultiElement(parameter->getName())
{
    this->parameter = parameter;
    setupColor();
    ofAddListener(colorEvent, listener, method);
}

template<typename L, typename M>
GuiColor::GuiColor(string name, ofFloatColor *color, L *listener, M method) : GuiMultiElement(name)
{
    parameter = new Parameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1));
    setupColor();
    ofAddListener(colorEvent, listener, method);
}

template<typename L, typename M>
GuiColor::GuiColor(string name, L *listener, M method) : GuiMultiElement(name)
{
    parameter = new Parameter<ofFloatColor>(name, new ofFloatColor(0.5, 0.5, 0.5, 1.0), ofFloatColor(0), ofFloatColor(1, 1, 1, 1));
    setupColor();
    ofAddListener(colorEvent, listener, method);
}
