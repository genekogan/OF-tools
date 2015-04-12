#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "Sequence.h"


template <typename T> class GuiSlider;

struct GuiSliderEventArgsBase { };

template<typename T>
struct GuiSliderEventArgs : public GuiSliderEventArgsBase
{
    GuiSlider<T> *slider;
    T value;
    
    GuiSliderEventArgs(GuiSlider<T> *slider, T value)
    {
        this->slider = slider;
        this->value = value;
    }
};


class GuiSliderBase : public GuiElement
{
public:
    GuiSliderBase(string name);
    virtual ~GuiSliderBase() { }
    
    virtual bool mouseMoved(int mouseX, int mouseY);
    virtual bool mousePressed(int mouseX, int mouseY);
    virtual bool mouseReleased(int mouseX, int mouseY);
    virtual bool mouseDragged(int mouseX, int mouseY);
    
    virtual bool keyPressed(int key);
    
    virtual void setValue(float sliderValue);
    float getValue() {return sliderValue;}
    
    void lerpTo(float nextValue, int numFrames);
    //void setValueFromSequence(Sequence &sequence);
    //void setSequenceFromValue(Sequence &sequence, int column);
    
    virtual void update();
    virtual void draw();
    
    
    ///////
    void getXml(ofXml &xml)
    {
        //xml.addValue("Name", getName());
        xml.addValue<float>("Value", getValue());
    }
    void setFromXml(ofXml &xml)
    {
        setValue(xml.getValue<float>("Value"));
    }
    
    //////
    
    
    
protected:
    
    void setEditing(bool editing);
    void keyboardEdit(int key);
    
    virtual void decrement() { }
    virtual void increment() { }
    
    //virtual void setSequenceFromExplicitValue(Sequence &sequence, int column, float value) { }
    virtual void setParameterValueFromString(string valueString) { }
    virtual string getParameterValueString() { }
    virtual void updateValueString() { }
    
    float sliderValue;
    string valueString, valueStringNext;
    float lerpPrevValue, lerpNextValue;;
    int lerpFrame, lerpNumFrames;
    bool editing;
    string editingString;
    bool toUpdateValueString;
    float valueStringWidth, stringHeight;
};


template<typename T>
class GuiSlider : public GuiSliderBase
{
public:
    GuiSlider(Parameter<T> *parameter);
    GuiSlider(string name, T *value, T min, T max);
    GuiSlider(string name, T min, T max);
    
    template <typename L, typename M>
    GuiSlider(Parameter<T> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    GuiSlider(string name, T *value, T min, T max, L *listener, M method);
    
    template <typename L, typename M>
    GuiSlider(string name, T min, T max, L *listener, M method);
    
    ~GuiSlider();
    
    void update();
    
    void setValue(float sliderValue);
    void setParameterValue(T value);
    
    T getParameterValue() {return parameter->get();}
    
    ofEvent<GuiSliderEventArgs<T> > sliderEvent;
    
private:
    
    void decrement();
    void increment();
    
    //void setSequenceFromExplicitValue(Sequence &sequence, int column, float value);
    void setParameterValueFromString(string valueString);
    string getParameterValueString() {return ofToString(parameter->get(), 2);}
    void updateValueString();
    void adjustSliderValue();
    
    Parameter<T> *parameter;
    T previous;
    
    
    Sequence *sequence;
};


template<typename T>
GuiSlider<T>::GuiSlider(Parameter<T> *parameter) : GuiSliderBase(parameter->getName())
{
    this->parameter = parameter;
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
}

template<typename T>
GuiSlider<T>::GuiSlider(string name, T *value, T min, T max) : GuiSliderBase(name)
{
    parameter = new Parameter<T>(name, value, min, max);
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
}

template<typename T>
GuiSlider<T>::GuiSlider(string name, T min, T max) : GuiSliderBase(name)
{
    parameter = new Parameter<T>(name, new T(), min, max);
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
}

template<typename T> template<typename L, typename M>
GuiSlider<T>::GuiSlider(Parameter<T> *parameter, L *listener, M method) : GuiSliderBase(parameter->getName())
{
    this->parameter = parameter;
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
    ofAddListener(sliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
GuiSlider<T>::GuiSlider(string name, T *value, T min, T max, L *listener, M method) : GuiSliderBase(name)
{
    parameter = new Parameter<T>(name, value, min, max);
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
    ofAddListener(sliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
GuiSlider<T>::GuiSlider(string name, T min, T max, L *listener, M method) : GuiSliderBase(name)
{
    parameter = new Parameter<T>(name, new T(), min, max);
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
    ofAddListener(sliderEvent, listener, method);
}

template<typename T>
GuiSlider<T>::~GuiSlider<T>()
{
    delete parameter;
    //
    //
    // who should delete parameter?
    //
    //
    //
    //
}

template<typename T>
void GuiSlider<T>::setValue(float sliderValue)
{
    this->sliderValue = sliderValue;
    parameter->set(sliderValue * parameter->getMax() + (1.0 - sliderValue) * parameter->getMin());
    updateValueString();
    adjustSliderValue();
    GuiSliderEventArgs<T> args(this, parameter->get());
    ofNotifyEvent(sliderEvent, args, this);
}

template<typename T>
void GuiSlider<T>::setParameterValue(T value)
{
    parameter->set(value);
    sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    updateValueString();
    adjustSliderValue();    // this can just be an inline int for setValue<int> instead
    GuiSliderEventArgs<T> args(this, parameter->get());
    ofNotifyEvent(sliderEvent, args, this);
}

template<typename T>
void GuiSlider<T>::setParameterValueFromString(string valueString)
{
    setParameterValue(ofToDouble(valueString));
}

template<typename T>
void GuiSlider<T>::decrement()
{
    setValue(ofClamp(sliderValue - 0.01, 0.0, 1.0));
}

template<> inline void GuiSlider<int>::decrement()
{
    setParameterValue(ofClamp(getParameterValue() - 1, parameter->getMin(), parameter->getMax()));
}

template<typename T>
void GuiSlider<T>::increment()
{
    setValue(ofClamp(sliderValue + 0.01, 0.0, 1.0));
}

template<> inline void GuiSlider<int>::increment()
{
    setParameterValue(ofClamp(getParameterValue() + 1, parameter->getMin(), parameter->getMax()));
}
/*
template<typename T>
void GuiSlider<T>::setSequenceFromExplicitValue(Sequence &sequence, int column, float value)
{
    float sequenceValue = ofClamp((float) (value - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    //sequence.setValueAtCell(column, sequenceValue);
}
*/
template<typename T>
void GuiSlider<T>::updateValueString()
{
    valueStringNext = ofToString(parameter->get(), floor(parameter->get()) == parameter->get() ? 0 : 2);
    toUpdateValueString = true;
}

template<typename T>
void GuiSlider<T>::update()
{
    GuiSliderBase::update();
    if (previous != parameter->get())
    {
        this->sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
        updateValueString();
        adjustSliderValue();
        previous = parameter->get();
    }
}

template<typename T> inline void GuiSlider<T>::adjustSliderValue() { }

template<> inline void GuiSlider<int>::adjustSliderValue()
{
    sliderValue = ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
}