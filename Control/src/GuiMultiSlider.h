#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "GuiSlider.h"
#include "GuiMultiElement.h"
#include "TouchOSC.h"



template<typename T> class GuiMultiSlider;

struct GuiMultiSliderEventArgsBase { };

template<typename T>
struct GuiMultiSliderEventArgs : public GuiMultiSliderEventArgsBase
{
    GuiMultiSlider<T> *slider;
    T value;
    
    GuiMultiSliderEventArgs(GuiMultiSlider<T> *slider, T value)
    {
        this->slider = slider;
        this->value = value;
    }
};


class GuiMultiSliderBase : public GuiMultiElement
{
public:
    GuiMultiSliderBase(string name);
    virtual ~GuiMultiSliderBase() { }
    
protected:
    
    virtual void createSliders() { }
    virtual void initializeSliders() { }
};


template<typename T>
class GuiMultiSlider : public GuiMultiSliderBase
{
public:

    GuiMultiSlider(Parameter<T> *parameter);
    GuiMultiSlider(string name, T *value, T min, T max);
    GuiMultiSlider(string name, T min, T max);
    
    template <typename L, typename M>
    GuiMultiSlider(Parameter<T> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    GuiMultiSlider(string name, T *value, T min, T max, L *listener, M method);
    
    template <typename L, typename M>
    GuiMultiSlider(string name, T min, T max, L *listener, M method);
    
    ~GuiMultiSlider();
    
    void setParameterValue(T value) {parameter->set(value);}
    T getParameterValue() {return parameter->get();}
    void getParameters(vector<ParameterBase*> & parameters);
    
    ofEvent<GuiMultiSliderEventArgs<T> > sliderEvent;
    
    //void addElementToTouchOscLayout(TouchOscPage *page, float *y);
    

    
private:
    
    void createSliders();
    void initializeSliders();
    void updateParameterOscAddress();
    void sliderChanged(GuiSliderEventArgs<float> &e);
    
    
//    string getOscAddress() { }
//    void sendOsc(ofxOscMessage &msg) { }
//    void receiveOsc(ofxOscMessage &msg) { }
//    bool valueChanged() {}
//
    
    
    Parameter<T> *parameter;
};


/*
template<typename T>
void GuiMultiSlider<T>::addElementToTouchOscLayout(TouchOscPage *page, float *y)
{
    TouchOscMultiFader *fader = page->addMultiFader(getName(), 0.01, *y, 0.9, elements.size());
    
    fader->setOscAddress(parameter->getOscAddress());
    
    fader->setNumber(elements.size());
    
    fader->setCentered(false);
    fader->setInverted(false);
    
    fader->setType(0);

    //fader->setResponseRelative(false);

    *y += (elements.size() + 0.05);
}
 */



template<typename T>
GuiMultiSlider<T>::GuiMultiSlider(Parameter<T> *parameter) : GuiMultiSliderBase(parameter->getName())
{
    this->parameter = parameter;
    createSliders();
    initializeSliders();
}

template<typename T>
GuiMultiSlider<T>::GuiMultiSlider(string name, T *value, T min, T max) : GuiMultiSliderBase(name)
{
    parameter = new Parameter<T>(name, value, min, max);
    createSliders();
    initializeSliders();
}

template<typename T>
GuiMultiSlider<T>::GuiMultiSlider(string name, T min, T max) : GuiMultiSliderBase(name)
{
    parameter = new Parameter<T>(name, new T(), min, max);
    createSliders();
    initializeSliders();
}

template<typename T> template<typename L, typename M>
GuiMultiSlider<T>::GuiMultiSlider(Parameter<T> *parameter, L *listener, M method) : GuiMultiSliderBase(parameter->getName())
{
    this->parameter = parameter;
    createSliders();
    initializeSliders();
    ofAddListener(sliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
GuiMultiSlider<T>::GuiMultiSlider(string name, T *value, T min, T max, L *listener, M method) : GuiMultiSliderBase(name)
{
    parameter = new Parameter<T>(name, value, min, max);
    createSliders();
    initializeSliders();
    ofAddListener(sliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
GuiMultiSlider<T>::GuiMultiSlider(string name, T min, T max, L *listener, M method) : GuiMultiSliderBase(name)
{
    parameter = new Parameter<T>(name, new T(), min, max);
    createSliders();
    initializeSliders();
    ofAddListener(sliderEvent, listener, method);
}

template<typename T>
GuiMultiSlider<T>::~GuiMultiSlider<T>()
{
    delete parameter;
}

template<typename T>
void GuiMultiSlider<T>::createSliders() { }

template<> inline void GuiMultiSlider<ofVec2f>::createSliders()
{
    initializeElement(new GuiSlider<float>(name+".x", &parameter->getReference()->x, parameter->getMin().x, parameter->getMax().x));
    initializeElement(new GuiSlider<float>(name+".y", &parameter->getReference()->y, parameter->getMin().y, parameter->getMax().y));
}

template<> inline void GuiMultiSlider<ofVec3f>::createSliders()
{
    initializeElement(new GuiSlider<float>(name+".x", &parameter->getReference()->x, parameter->getMin().x, parameter->getMax().x));
    initializeElement(new GuiSlider<float>(name+".y", &parameter->getReference()->y, parameter->getMin().y, parameter->getMax().y));
    initializeElement(new GuiSlider<float>(name+".z", &parameter->getReference()->z, parameter->getMin().z, parameter->getMax().z));
}

template<> inline void GuiMultiSlider<ofVec4f>::createSliders()
{
    initializeElement(new GuiSlider<float>(name+".x", &parameter->getReference()->x, parameter->getMin().x, parameter->getMax().x));
    initializeElement(new GuiSlider<float>(name+".y", &parameter->getReference()->y, parameter->getMin().y, parameter->getMax().y));
    initializeElement(new GuiSlider<float>(name+".z", &parameter->getReference()->z, parameter->getMin().z, parameter->getMax().z));
    initializeElement(new GuiSlider<float>(name+".w", &parameter->getReference()->w, parameter->getMin().w, parameter->getMax().w));
}

template<typename T>
void GuiMultiSlider<T>::initializeSliders()
{
    for (auto e : elements) {
        ofAddListener(((GuiSlider<float> *) e)->sliderEvent, this, &GuiMultiSlider<T>::sliderChanged);
    }
}

template<typename T>
void GuiMultiSlider<T>::updateParameterOscAddress()
{
    parameter->setOscAddress(getAddress());
}

template<typename T>
void GuiMultiSlider<T>::sliderChanged(GuiSliderEventArgs<float> &e)
{
    GuiMultiSliderEventArgs<T> args(this, parameter->get());
    ofNotifyEvent(sliderEvent, args, this);
}

template<typename T>
void GuiMultiSlider<T>::getParameters(vector<ParameterBase*> & parameters)
{
    parameters.push_back(parameter);
}
