#pragma once

#include "ofMain.h"
#include "GuiSlider.h"
#include "GuiWidgetBase.h"



class GuiColor : public GuiWidgetBase
{
public:
    GuiColor(Parameter<ofFloatColor> *parameter);
    GuiColor(string name, ofFloatColor *value);
    
    template <typename L, typename M>
    GuiColor(Parameter<ofFloatColor> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    GuiColor(string name, ofFloatColor *value, L *listener, M method);
    
    
    ///////
    void getXml(ofXml &xml) {
        xml.addValue("Name", getName());
    }
    void setFromXml(ofXml &xml) {

    }
    
    //////

private:
    
    void setupColor();
    void update();
    
    Parameter<ofFloatColor> *parameter;
};

template <typename L, typename M>
GuiColor::GuiColor(Parameter<ofFloatColor> *parameter, L *listener, M method) : GuiWidgetBase(parameter->getName())
{
    this->parameter = parameter;
    setupColor();
    ofAddListener(elementEvent, listener, method);
}

template <typename L, typename M>
GuiColor::GuiColor(string name, ofFloatColor *value, L *listener, M method) : GuiWidgetBase(name)
{
    parameter = new Parameter<ofFloatColor>(name, value, ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1));
    setupColor();
    ofAddListener(elementEvent, listener, method);
}


