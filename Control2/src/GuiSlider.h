#pragma once

//#include "ofMain.h"
#include "ofBitmapFont.h"
//#include "Parameter.h"
//#include "GuiElement.h"
#include "Sequence.h"


class GuiSliderBase : public GuiElement
{
public:
    GuiSliderBase(string name) : GuiElement(name) { }
    
    virtual void mouseMoved(int mouseX, int mouseY) { }
    virtual void mousePressed(int mouseX, int mouseY) { }
    virtual void mouseReleased(int mouseX, int mouseY) { }
    virtual void mouseDragged(int mouseX, int mouseY) { }

    float getSliderValue() {return sliderValue;}
    
    virtual void update() { }
    virtual void draw();
    
    ofEvent<GuiSliderEventArgs> sliderEvent;

protected:
    void setValueString(string _valueString);

    float sliderValue;
    string valueString, valueStringNext;
    bool toUpdateValueString;
    float valueStringWidth, stringHeight;
};


template<typename T>
class GuiSlider : public GuiSliderBase
{
public:
    GuiSlider(string name, Parameter<T> *parameter);
    GuiSlider(string name, T *value, T min, T max);

    void update();
    
    void setValue(float sliderValue);
    void setValueFromSequence(Sequence &sequence);
    T getValue() {return parameter->get();}
    
    void mouseMoved(int mouseX, int mouseY);
    void mousePressed(int mouseX, int mouseY);
    void mouseReleased(int mouseX, int mouseY);
    void mouseDragged(int mouseX, int mouseY);

private:
    void adjustSliderValue();
    
    Parameter<T> *parameter;
    T previous;
};


template<typename T>
GuiSlider<T>::GuiSlider(string name, Parameter<T> *parameter) : GuiSliderBase(name)
{
    this->parameter = parameter;
    sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    setValueString(ofToString(parameter->get(), floor(parameter->get()) == parameter->get() ? 0 : 2));
}

template<typename T>
GuiSlider<T>::GuiSlider(string name, T *value, T min, T max) : GuiSliderBase(name)
{
    parameter = new Parameter<T>(name, value, min, max);
    sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    setValueString(ofToString(parameter->get(), floor(parameter->get()) == parameter->get() ? 0 : 2));
}

template<typename T>
void GuiSlider<T>::setValue(float sliderValue)
{
    this->sliderValue = sliderValue;
    parameter->set(sliderValue * parameter->getMax() + (1.0-sliderValue) * parameter->getMin());
    setValueString(ofToString(parameter->get(), floor(parameter->get()) == parameter->get() ? 0 : 2));
    adjustSliderValue();
    GuiSliderEventArgs args(name, (float) parameter->get());
    ofNotifyEvent(sliderEvent, args, this);
}

template<typename T>
void GuiSlider<T>::setValueFromSequence(Sequence &sequence)
{
    setValue(sequence.getValueAtCurrentCursor());
}

template<typename T>
void GuiSlider<T>::update()
{
    if (previous != parameter->get())
    {
        this->sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
        setValueString(ofToString(parameter->get(), floor(parameter->get()) == parameter->get() ? 0 : 2));
        adjustSliderValue();
        previous = parameter->get();
    }
}


template<typename T> inline void GuiSlider<T>::adjustSliderValue() { }

template<> inline void GuiSlider<int>::adjustSliderValue()
{
    sliderValue = ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
}

template<typename T> inline void GuiSlider<T>::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
}

template<typename T> inline void GuiSlider<T>::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        setValue(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1));
    }
}

template<typename T> inline void GuiSlider<T>::mouseReleased(int mouseX, int mouseY)
{
    GuiElement::mouseReleased(mouseX, mouseY);
}

template<typename T> inline void GuiSlider<T>::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
    if (mouseDragging)
    {
        setValue(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1));
    }
}
