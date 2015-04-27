#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "Sequence.h"
#include "TouchOsc.h"


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
    virtual ~GuiSliderBase();
    
    virtual void setValue(float sliderValue);
    float getValue() {return sliderValue;}
    
    void lerpTo(float nextValue, int numFrames);
    
    virtual void setMin(float min) { }
    virtual void setMax(float max) { }
    
    virtual void update();
    virtual void draw();
    
    virtual bool mouseMoved(int mouseX, int mouseY);
    virtual bool mousePressed(int mouseX, int mouseY);
    virtual bool mouseReleased(int mouseX, int mouseY);
    virtual bool mouseDragged(int mouseX, int mouseY);
    virtual bool keyPressed(int key);

    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
protected:
    
    void setValueFromSequence(Sequence &sequence);
    void setSequenceFromValue(Sequence &sequence, int column);

    void setEditing(bool editing);
    void keyboardEdit(int key);
    
    virtual void decrement() { }
    virtual void increment() { }
    
    virtual void setSequenceFromExplicitValue(Sequence &sequence, int column, float value) { }
    virtual void setParameterValueFromString(string valueString) { }
    virtual void updateValueString() { }
    virtual string getParameterValueString() { }
    
    float sliderValue;
    bool changed;
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
    
    void setValue(float sliderValue);
    void setParameterValue(T value);
    
    T getParameterValue() {return parameter->get();}
    void getParameters(vector<ParameterBase*> & parameters);

    void setMin(float min);
    void setMax(float max);

    void update();
    
    ofEvent<GuiSliderEventArgs<T> > sliderEvent;
    
private:

    void decrement();
    void increment();
    
    void setSequenceFromExplicitValue(Sequence &sequence, int column, float value);
    void setParameterValueFromString(string valueString);
    void adjustSliderValue();
    void updateValueString();
    string getParameterValueString() {return ofToString(parameter->get(), 2);}
    
    void updateParameterOscAddress();
    string getOscAddress() {return parameter->getOscAddress(); }
    void sendOsc(ofxOscMessage &msg);
    void receiveOsc(ofxOscMessage &msg) {setValue(msg.getArgAsFloat(0));}
    bool valueChanged();
    void addElementToTouchOscLayout(TouchOscPage *page, float *y);

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
}

template<typename T>
void GuiSlider<T>::setValue(float sliderValue)
{
    GuiSliderBase::setValue(sliderValue);
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
void GuiSlider<T>::getParameters(vector<ParameterBase*> & parameters)
{
    parameters.push_back(parameter);
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

template<typename T>
void GuiSlider<T>::setSequenceFromExplicitValue(Sequence &sequence, int column, float value)
{
    float sequenceValue = ofClamp((float) (value - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    sequence.setValueAtCell(column, sequenceValue);
}

template<typename T>
void GuiSlider<T>::updateValueString()
{
    valueStringNext = ofToString(parameter->get(), floor(parameter->get()) == parameter->get() ? 0 : 2);
    toUpdateValueString = true;
}

template<typename T>
void GuiSlider<T>::updateParameterOscAddress()
{
    parameter->setOscAddress(getAddress());
}

template<typename T>
void GuiSlider<T>::sendOsc(ofxOscMessage &msg)
{
    msg.addFloatArg(parameter->get());
}

template<>
inline void GuiSlider<int>::sendOsc(ofxOscMessage &msg)
{
    msg.addIntArg(parameter->get());
}

template<typename T>
bool GuiSlider<T>::valueChanged()
{
    if (changed)
    {
        changed = false;
        return true;
    }
    else {
        return false;
    }
}

template<typename T>
void GuiSlider<T>::addElementToTouchOscLayout(TouchOscPage *page, float *y)
{
    TouchOscFader *fader = page->addFader(getName(), 0.01, *y, 0.98, 1);
    TouchOscLabel *label = page->addLabel(getName(), 0.01, *y, 0.98, 1);
    fader->setOscAddress(parameter->getOscAddress());
    fader->setType(0);
    label->setType(0);
    label->setColor(RED);
    *y += 1.04;
}

template<typename T>
void GuiSlider<T>::update()
{
    GuiSliderBase::update();
    if (previous != parameter->get())
    {
        GuiSliderBase::setValue(ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
        updateValueString();
        adjustSliderValue();
        previous = parameter->get();
    }
}

template<typename T>
void GuiSlider<T>::setMin(float min)
{
    parameter->setMin(min);
    sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    updateValueString();
    adjustSliderValue();
}

template<typename T>
void GuiSlider<T>::setMax(float max)
{
    parameter->setMax(max);
    sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    updateValueString();
    adjustSliderValue();
}

template<typename T> inline void GuiSlider<T>::adjustSliderValue() { }

template<> inline void GuiSlider<int>::adjustSliderValue()
{
    sliderValue = ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
}
