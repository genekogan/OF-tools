#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "Sequence.h"


class GuiRangeSliderBase : public GuiElement
{
public:
    enum {LOW, MIDDLE, HIGH};
    
    GuiRangeSliderBase(string name);
    ~GuiRangeSliderBase();
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void setValue(float sliderValue);
    virtual void setValueLow(float sliderLow);
    virtual void setValueHigh(float sliderHigh);

    float getValueLow() {return sliderLow;}
    float getValueHigh() {return sliderHigh;}
    
    void lerpTo(float nextLow, float nextHigh, int numFrames);
    void setValueFromSequence(Sequence &sequence);
    
    virtual void update();
    void draw();
    
protected:
    
    void selectSlider(float sliderValue);
    void keyboardEdit(int key);
    virtual void updateValueString() { }
    
    virtual void decrement() { }
    virtual void increment() { }
    
    int selection;
    float sliderLow, sliderHigh, sliderValuePrev;
    string valueString, valueStringNext;
    float lerpPrevLow, lerpNextLow;
    float lerpPrevHigh, lerpNextHigh;
    int lerpFrame, lerpNumFrames;
    bool toUpdateValueString;
    float valueStringWidth, stringHeight;
};


template<typename T>
class GuiRangeSlider : public GuiRangeSliderBase
{
public:
    GuiRangeSlider(string name, Parameter<T> *pLow, Parameter<T> *pHigh);
    GuiRangeSlider(string name, T *low, T *high, T min, T max);
    GuiRangeSlider(string name, T min, T max);
    
    void setValueLow(float sliderLow);
    void setValueHigh(float sliderHigh);
    
    void setParameterLowValue(T low);
    void setParameterHighValue(T high);
    
    T getParameterLowValue() {return pLow->get();}
    T getParameterHighValue() {return pHigh->get();}
    
    void update();
    
private:
    
    void decrement();
    void increment();

    void updateValueString();
    void adjustSliderValueLow();
    void adjustSliderValueHigh();
    
    Parameter<T> *pLow, *pHigh;
    T previousLow, previousHigh;
};


template<typename T>
GuiRangeSlider<T>::GuiRangeSlider(string name, Parameter<T> *pLow, Parameter<T> *pHigh) : GuiRangeSliderBase(name)
{
    this->pLow = pLow;
    this->pHigh = pHigh;
    setValueLow(ofClamp((pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0));
    setValueHigh(ofClamp((pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0));
}

template<typename T>
GuiRangeSlider<T>::GuiRangeSlider(string name, T *low, T *high, T min, T max) : GuiRangeSliderBase(name)
{
    pLow = new Parameter<T>(name, low, min, max);
    pHigh = new Parameter<T>(name, high, min, max);
    setValueLow(ofClamp((pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0));
    setValueHigh(ofClamp((pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0));
}

template<typename T>
GuiRangeSlider<T>::GuiRangeSlider(string name, T min, T max) : GuiRangeSliderBase(name)
{
    pLow = new Parameter<T>(name, new T(), min, max);
    pHigh = new Parameter<T>(name, new T(), min, max);
    setValueLow(ofClamp((pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0));
    setValueHigh(ofClamp((pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0));
}

template<typename T>
void GuiRangeSlider<T>::setValueLow(float sliderLow)
{
    this->sliderLow = sliderLow;
    pLow->set(sliderLow * pLow->getMax() + (1.0 - sliderLow) * pLow->getMin());
    updateValueString();
    adjustSliderValueLow();
    GuiElementEventArgs args(name, 0, (float) pLow->get());
    ofNotifyEvent(elementEvent, args, this);
}

template<typename T>
void GuiRangeSlider<T>::setValueHigh(float sliderHigh)
{
    this->sliderHigh = sliderHigh;
    pHigh->set(sliderHigh * pHigh->getMax() + (1.0 - sliderHigh) * pHigh->getMin());
    updateValueString();
    adjustSliderValueHigh();
    GuiElementEventArgs args(name, 1, (float) pHigh->get());
    ofNotifyEvent(elementEvent, args, this);
}

template<typename T>
void GuiRangeSlider<T>::setParameterLowValue(T low)
{
    pLow->set(low);
    sliderLow = ofClamp((pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0);
    updateValueString();
    adjustSliderValueLow();
    GuiElementEventArgs args(name, 0, (float) pLow->get());
    ofNotifyEvent(elementEvent, args, this);
}

template<typename T>
void GuiRangeSlider<T>::setParameterHighValue(T high)
{
    pHigh->set(high);
    sliderHigh = ofClamp((pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0);
    updateValueString();
    adjustSliderValueHigh();
    GuiElementEventArgs args(name, 1, (float) pHigh->get());
    ofNotifyEvent(elementEvent, args, this);
}

template<typename T>
void GuiRangeSlider<T>::updateValueString()
{
    valueStringNext = "("+ofToString(pLow->get(), floor(pLow->get()) == pLow->get() ? 0 : 2)+","+ofToString(pHigh->get(), floor(pHigh->get()) == pHigh->get() ? 0 : 2)+")";
    toUpdateValueString = true;
}

template<typename T>
void GuiRangeSlider<T>::update()
{
    GuiRangeSliderBase::update();
    if (previousLow != pLow->get())
    {
        this->sliderLow = ofClamp((pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0);
        updateValueString();
        adjustSliderValueLow();
        previousLow = pLow->get();
    }
    if (previousHigh != pHigh->get())
    {
        this->sliderHigh = ofClamp((pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0);
        updateValueString();
        adjustSliderValueHigh();
        previousHigh = pHigh->get();
    }
}

template<> inline void GuiRangeSlider<int>::decrement()
{
    if (selection == LOW) {
        setParameterLowValue(ofClamp(getParameterLowValue() - 1, pLow->getMin(), pLow->getMax()));
    }
    else if (selection == HIGH) {
        setParameterHighValue(ofClamp(getParameterHighValue() - 1, pHigh->getMin(), pHigh->getMax()));
    }
    else if (selection == MIDDLE)
    {
        setParameterLowValue(ofClamp(getParameterLowValue() - 1, pLow->getMin(), pLow->getMax()));
        setParameterHighValue(ofClamp(getParameterHighValue() - 1, pHigh->getMin(), pHigh->getMax()));
    }
}

template<typename T>
void GuiRangeSlider<T>::decrement()
{
    if (selection == LOW) {
        setValueLow(ofClamp(sliderLow - 0.01, 0.0, 1.0));
    }
    else if (selection == HIGH) {
        setValueHigh(ofClamp(sliderHigh - 0.01, 0.0, 1.0));
    }
    else if (selection == MIDDLE)
    {
        setValueLow(ofClamp(sliderLow - 0.01, 0.0, 1.0));
        setValueHigh(ofClamp(sliderHigh - 0.01, 0.0, 1.0));
    }
}

template<> inline void GuiRangeSlider<int>::increment()
{
    if (selection == LOW) {
        setParameterLowValue(ofClamp(getParameterLowValue() + 1, pLow->getMin(), pLow->getMax()));
    }
    else if (selection == HIGH) {
        setParameterHighValue(ofClamp(getParameterHighValue() + 1, pHigh->getMin(), pHigh->getMax()));
    }
    else if (selection == MIDDLE)
    {
        setParameterLowValue(ofClamp(getParameterLowValue() + 1, pLow->getMin(), pLow->getMax()));
        setParameterHighValue(ofClamp(getParameterHighValue() + 1, pHigh->getMin(), pHigh->getMax()));
    }
}
template<typename T>
void GuiRangeSlider<T>::increment()
{
    if (selection == LOW) {
        setValueLow(ofClamp(sliderLow + 0.01, 0.0, 1.0));
    }
    else if (selection == HIGH) {
        setValueHigh(ofClamp(sliderHigh + 0.01, 0.0, 1.0));
    }
    else if (selection == MIDDLE)
    {
        setValueLow(ofClamp(sliderLow + 0.01, 0.0, 1.0));
        setValueHigh(ofClamp(sliderHigh + 0.01, 0.0, 1.0));
    }
}

template<typename T> inline void GuiRangeSlider<T>::adjustSliderValueLow() { }
template<typename T> inline void GuiRangeSlider<T>::adjustSliderValueHigh() { }

template<> inline void GuiRangeSlider<int>::adjustSliderValueLow()
{
    sliderLow = ofClamp((float) (pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0);
}
template<> inline void GuiRangeSlider<int>::adjustSliderValueHigh()
{
    sliderHigh = ofClamp((float) (pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0);
}
