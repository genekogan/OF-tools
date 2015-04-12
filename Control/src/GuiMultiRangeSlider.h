#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "GuiRangeSlider.h"
#include "GuiMultiElement.h"


template<typename T> class GuiMultiRangeSlider;

struct GuiMultiRangeSliderEventArgsBase { };

template<typename T>
struct GuiMultiRangeSliderEventArgs : public GuiMultiRangeSliderEventArgsBase
{
    GuiMultiRangeSlider<T> *slider;
    T low;
    T high;
    
    GuiMultiRangeSliderEventArgs(GuiMultiRangeSlider<T> *slider, T low, T high)
    {
        this->slider = slider;
        this->low = low;
        this->high = high;
    }
};


class GuiMultiRangeSliderBase : public GuiMultiElement
{
public:
    GuiMultiRangeSliderBase(string name);
    virtual ~GuiMultiRangeSliderBase() { }
    
protected:
    
    virtual void createRangeSliders() { }
    virtual void initializeRangeSliders() { }
};


template<typename T>
class GuiMultiRangeSlider : public GuiMultiRangeSliderBase
{
public:
    
    GuiMultiRangeSlider(Parameter<T> *parameterLow, Parameter<T> *parameterHigh);
    GuiMultiRangeSlider(string name, T *low, T *high, T min, T max);
    GuiMultiRangeSlider(string name, T min, T max);
    
    template <typename L, typename M>
    GuiMultiRangeSlider(Parameter<T> *parameterLow, Parameter<T> *parameterHigh, L *listener, M method);
    
    template <typename L, typename M>
    GuiMultiRangeSlider(string name, T *low, T *high, T min, T max, L *listener, M method);
    
    template <typename L, typename M>
    GuiMultiRangeSlider(string name, T min, T max, L *listener, M method);
    
    ~GuiMultiRangeSlider();
    
    void setParameterValueLow(T value) {parameterLow->set(value);}
    void setParameterValueHigh(T value) {parameterHigh->set(value);}
    T getParameterValueLow() {return parameterLow->get();}
    T getParameterValueHigh() {return parameterHigh->get();}
    
    void getParameters(vector<ParameterBase*> & parameters);
    
    ofEvent<GuiMultiRangeSliderEventArgs<T> > rangeSliderEvent;
    
private:
    
    void createRangeSliders();
    void initializeRangeSliders();
    void rangeSliderChanged(GuiRangeSliderEventArgs<float> &e);
    
    Parameter<T> *parameterLow, *parameterHigh;
};


template<typename T>
GuiMultiRangeSlider<T>::GuiMultiRangeSlider(Parameter<T> *parameterLow, Parameter<T> *parameterHigh) : GuiMultiRangeSliderBase(parameterLow->getName())
{
    this->parameterLow = parameterLow;
    this->parameterHigh = parameterHigh;
    createRangeSliders();
    initializeRangeSliders();
}

template<typename T>
GuiMultiRangeSlider<T>::GuiMultiRangeSlider(string name, T *low, T *high, T min, T max) : GuiMultiRangeSliderBase(name)
{
    parameterLow = new Parameter<T>(name, low, min, max);
    parameterHigh = new Parameter<T>(name, high, min, max);
    createRangeSliders();
    initializeRangeSliders();
}

template<typename T>
GuiMultiRangeSlider<T>::GuiMultiRangeSlider(string name, T min, T max) : GuiMultiRangeSliderBase(name)
{
    parameterLow = new Parameter<T>(name, new T(min + 0.33 * (max - min)), min, max);
    parameterHigh = new Parameter<T>(name, new T(min + 0.67 * (max - min)), min, max);
    createRangeSliders();
    initializeRangeSliders();
}

template<typename T> template<typename L, typename M>
GuiMultiRangeSlider<T>::GuiMultiRangeSlider(Parameter<T> *parameterLow, Parameter<T> *parameterHigh, L *listener, M method) : GuiMultiRangeSliderBase(parameterLow->getName())
{
    this->parameterLow = parameterLow;
    this->parameterHigh = parameterHigh;
    createRangeSliders();
    initializeRangeSliders();
    ofAddListener(rangeSliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
GuiMultiRangeSlider<T>::GuiMultiRangeSlider(string name, T *low, T *high, T min, T max, L *listener, M method) : GuiMultiRangeSliderBase(name)
{
    parameterLow = new Parameter<T>(name, low, min, max);
    parameterHigh = new Parameter<T>(name, high, min, max);
    createRangeSliders();
    initializeRangeSliders();
    ofAddListener(rangeSliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
GuiMultiRangeSlider<T>::GuiMultiRangeSlider(string name, T min, T max, L *listener, M method) : GuiMultiRangeSliderBase(name)
{
    parameterLow = new Parameter<T>(name, new T(min + 0.33 * (max - min)), min, max);
    parameterHigh = new Parameter<T>(name, new T(min + 0.67 * (max - min)), min, max);
    createRangeSliders();
    initializeRangeSliders();
    ofAddListener(rangeSliderEvent, listener, method);
}

template<typename T>
GuiMultiRangeSlider<T>::~GuiMultiRangeSlider<T>()
{
    delete parameterLow;
    delete parameterHigh;
}

template<typename T>
void GuiMultiRangeSlider<T>::createRangeSliders() { }

template<> inline void GuiMultiRangeSlider<ofVec2f>::createRangeSliders()
{
    initializeElement(new GuiRangeSlider<float>(name+".x", &parameterLow->getReference()->x, &parameterHigh->getReference()->x, min(parameterLow->getMin().x, parameterHigh->getMin().x), max(parameterLow->getMax().x, parameterHigh->getMax().x)));
    initializeElement(new GuiRangeSlider<float>(name+".y", &parameterLow->getReference()->y, &parameterHigh->getReference()->y, min(parameterLow->getMin().y, parameterHigh->getMin().y), max(parameterLow->getMax().y, parameterHigh->getMax().y)));
}

template<> inline void GuiMultiRangeSlider<ofVec3f>::createRangeSliders()
{
    initializeElement(new GuiRangeSlider<float>(name+".x", &parameterLow->getReference()->x, &parameterHigh->getReference()->x, min(parameterLow->getMin().x, parameterHigh->getMin().x), max(parameterLow->getMax().x, parameterHigh->getMax().x)));
    initializeElement(new GuiRangeSlider<float>(name+".y", &parameterLow->getReference()->y, &parameterHigh->getReference()->y, min(parameterLow->getMin().y, parameterHigh->getMin().y), max(parameterLow->getMax().y, parameterHigh->getMax().y)));
    initializeElement(new GuiRangeSlider<float>(name+".z", &parameterLow->getReference()->z, &parameterHigh->getReference()->z, min(parameterLow->getMin().z, parameterHigh->getMin().z), max(parameterLow->getMax().z, parameterHigh->getMax().z)));
}

template<> inline void GuiMultiRangeSlider<ofVec4f>::createRangeSliders()
{
    initializeElement(new GuiRangeSlider<float>(name+".x", &parameterLow->getReference()->x, &parameterHigh->getReference()->x, min(parameterLow->getMin().x, parameterHigh->getMin().x), max(parameterLow->getMax().x, parameterHigh->getMax().x)));
    initializeElement(new GuiRangeSlider<float>(name+".y", &parameterLow->getReference()->y, &parameterHigh->getReference()->y, min(parameterLow->getMin().y, parameterHigh->getMin().y), max(parameterLow->getMax().y, parameterHigh->getMax().y)));
    initializeElement(new GuiRangeSlider<float>(name+".z", &parameterLow->getReference()->z, &parameterHigh->getReference()->z, min(parameterLow->getMin().z, parameterHigh->getMin().z), max(parameterLow->getMax().z, parameterHigh->getMax().z)));
    initializeElement(new GuiRangeSlider<float>(name+".w", &parameterLow->getReference()->w, &parameterHigh->getReference()->w, min(parameterLow->getMin().w, parameterHigh->getMin().w), max(parameterLow->getMax().w, parameterHigh->getMax().w)));
}

template<typename T>
void GuiMultiRangeSlider<T>::initializeRangeSliders()
{
    for (auto e : elements) {
        ofAddListener(((GuiRangeSlider<float> *) e)->rangeSliderEvent, this, &GuiMultiRangeSlider<T>::rangeSliderChanged);
    }
}

template<typename T>
void GuiMultiRangeSlider<T>::rangeSliderChanged(GuiRangeSliderEventArgs<float> &e)
{
    GuiMultiRangeSliderEventArgs<T> args(this, parameterLow->get(), parameterHigh->get());
    ofNotifyEvent(rangeSliderEvent, args, this);
}

template<typename T>
void GuiMultiRangeSlider<T>::getParameters(vector<ParameterBase*> & parameters)
{
    parameters.push_back(parameterLow);
    parameters.push_back(parameterHigh);
}
