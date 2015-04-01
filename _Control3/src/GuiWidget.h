#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "GuiTextBox.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiRangeSlider.h"

#include "GuiMultiElement.h"
#include "GuiMultiSlider.h"
#include "GuiMultiRangeSlider.h"
#include "Gui2dPad.h"
#include "GuiColor.h"
#include "GuiMenu.h"


class GuiWidget : public GuiMultiElement
{
public:
    GuiWidget(string name);
    GuiWidget();
    
    GuiWidget * addWidget(GuiWidget *newWidget);
    GuiWidget * addWidget(string widgetName);

    GuiButton * addButton(Parameter<bool> *parameter);
    GuiButton * addButton(string name, bool *value);
    template<typename L, typename M>
    GuiButton * addButton(Parameter<bool> *parameter, L *listener, M method);
    template<typename L, typename M>
    GuiButton * addButton(string name, bool *value, L *listener, M method);
    template<typename L, typename M>
    GuiButton * addButton(string name, L *listener, M method);
    
    GuiToggle * addToggle(Parameter<bool> *parameter);
    GuiToggle * addToggle(string name, bool *value);
    template<typename L, typename M>
    GuiToggle * addToggle(Parameter<bool> *parameter, L *listener, M method);
    template<typename L, typename M>
    GuiToggle * addToggle(string name, bool *value, L *listener, M method);
    template<typename L, typename M>
    GuiToggle * addToggle(string name, L *listener, M method);

    GuiTextBox * addTextBox(Parameter<string> *parameter);
    GuiTextBox * addTextBox(string name, string *value);
    template<typename L, typename M>
    GuiTextBox * addTextBox(Parameter<string> *parameter, L *listener, M method);
    template<typename L, typename M>
    GuiTextBox * addTextBox(string name, string *value, L *listener, M method);
    template<typename L, typename M>
    GuiTextBox * addTextBox(string name, L *listener, M method);
    
    template<typename T>
    GuiSlider<T> * addSlider(Parameter<T> *parameter);
    template<typename T>
    GuiSlider<T> * addSlider(string name, T *value, T min, T max);
    template<typename T, typename L, typename M>
    GuiSlider<T> * addSlider(Parameter<T> *parameter, L *listener, M method);
    template<typename T, typename L, typename M>
    GuiSlider<T> * addSlider(string name, T *value, T min, T max, L *listener, M method);

    template<typename T>
    GuiMultiSlider<T> * addMultiSlider(Parameter<T> *parameter);
    template<typename T>
    GuiMultiSlider<T> * addMultiSlider(string name, T *value, T min, T max);
    template<typename T, typename L, typename M>
    GuiMultiSlider<T> * addMultiSlider(Parameter<T> *parameter, L *listener, M method);
    template<typename T, typename L, typename M>
    GuiMultiSlider<T> * addMultiSlider(string name, T *value, T min, T max, L *listener, M method);

    template<typename T>
    GuiRangeSlider<T> * addRangeSlider(string name, Parameter<T> *parameterLow, Parameter<T> *parameterHigh);
    template<typename T>
    GuiRangeSlider<T> * addRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max);
    template<typename T, typename L, typename M>
    GuiRangeSlider<T> * addRangeSlider(string name, Parameter<T> *parameterLow, Parameter<T> *parameterHigh, L *listener, M method);
    template<typename T, typename L, typename M>
    GuiRangeSlider<T> * addRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max, L *listener, M method);
    template<typename T, typename L, typename M>
    GuiRangeSlider<T> * addRangeSlider(string name, T min, T max, L *listener, M method);
    
    template<typename T>
    GuiMultiRangeSlider<T> * addMultiRangeSlider(string name, Parameter<T> *parameterLow, Parameter<T> *parameterHigh);
    template<typename T>
    GuiMultiRangeSlider<T> * addMultiRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max);
    template<typename T, typename L, typename M>
    GuiMultiRangeSlider<T> * addMultiRangeSlider(string name, Parameter<T> *parameterLow, Parameter<T> *parameterHigh, L *listener, M method);
    template<typename T, typename L, typename M>
    GuiMultiRangeSlider<T> * addMultiRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max, L *listener, M method);
    template<typename T, typename L, typename M>
    GuiMultiRangeSlider<T> * addMultiRangeSlider(string name, T min, T max, L *listener, M method);

    Gui2dPad * add2dPad(Parameter<ofPoint> *parameter);
    Gui2dPad * add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max);
    Gui2dPad * add2dPad(string name, ofPoint min, ofPoint max);
    template<typename L, typename M>
    Gui2dPad * add2dPad(Parameter<ofPoint> *parameter, L *listener, M method);
    template<typename L, typename M>
    Gui2dPad * add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method);
    template<typename L, typename M>
    Gui2dPad * add2dPad(string name, ofPoint min, ofPoint max, L *listener, M method);
    
    GuiColor * addColor(Parameter<ofFloatColor> *parameter);
    GuiColor * addColor(string name, ofFloatColor *value);
    template<typename L, typename M>
    GuiColor * addColor(Parameter<ofFloatColor> *parameter, L *listener, M method);
    template<typename L, typename M>
    GuiColor * addColor(string name, ofFloatColor *value, L *listener, M method);
    
    GuiMenu * addMenu(string name, vector<string> choices, bool multipleChoice=false, bool autoClose=false);
    GuiMenu * addMenu(string name, bool multipleChoice=false, bool autoClose=false);
    template<typename L, typename M>
    GuiMenu * addMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice=false, bool autoClose=false);
    template<typename L, typename M>
    GuiMenu * addMenu(string name, L *listener, M method, bool multipleChoice=false, bool autoClose=false);
    
private:
    
    void initializeElement(GuiElement *element);
};


template<typename L, typename M>
GuiButton * GuiWidget::addButton(Parameter<bool> *parameter, L *listener, M method)
{
    GuiButton *button = addButton(parameter);
    ofAddListener(button->buttonEvent, listener, method);
    return button;
}

template<typename L, typename M>
GuiButton * GuiWidget::addButton(string name, bool *value, L *listener, M method)
{
    return addButton(new Parameter<bool>(name, value), listener, method);
}

template<typename L, typename M>
GuiButton * GuiWidget::addButton(string name, L *listener, M method)
{
    return addButton(new Parameter<bool>(name, new bool(false)), listener, method);
}

template<typename L, typename M>
GuiToggle * GuiWidget::addToggle(Parameter<bool> *parameter, L *listener, M method)
{
    GuiToggle *toggle = addToggle(parameter);
    ofAddListener(toggle->buttonEvent, listener, method);
    return toggle;
}

template<typename L, typename M>
GuiToggle * GuiWidget::addToggle(string name, bool *value, L *listener, M method)
{
    return addToggle(new Parameter<bool>(name, value), listener, method);
}

template<typename L, typename M>
GuiToggle * GuiWidget::addToggle(string name, L *listener, M method)
{
    return addToggle(new Parameter<bool>(name, new bool(false)), listener, method);
}

template<typename L, typename M>
GuiTextBox * GuiWidget::addTextBox(Parameter<string> *parameter, L *listener, M method)
{
    GuiTextBox *textBox = addTextBox(parameter);
    ofAddListener(textBox->textBoxEvent, listener, method);
    return textBox;
}

template<typename L, typename M>
GuiTextBox * GuiWidget::addTextBox(string name, string *value, L *listener, M method)
{
    return addTextBox(new Parameter<string>(name, value), listener, method);
}

template<typename L, typename M>
GuiTextBox * GuiWidget::addTextBox(string name, L *listener, M method)
{
    return addTextBox(new Parameter<string>(name, new string(name)), listener, method);
}

template<typename T>
GuiSlider<T> * GuiWidget::addSlider(Parameter<T> *parameter)
{
    GuiSlider<T> *slider = new GuiSlider<T>(parameter);
    initializeElement(slider);
    return slider;
}

template<typename T>
GuiSlider<T> * GuiWidget::addSlider(string name, T *value, T min, T max)
{
    return addSlider(new Parameter<T>(name, value, min, max));
}

template<typename T, typename L, typename M>
GuiSlider<T> * GuiWidget::addSlider(Parameter<T> *parameter, L *listener, M method)
{
    GuiSlider<T> *slider = addSlider(parameter);
    ofAddListener(slider->sliderEvent, listener, method);
    return slider;
}

template<typename T, typename L, typename M>
GuiSlider<T> * GuiWidget::addSlider(string name, T *value, T min, T max, L *listener, M method)
{
    return addSlider(new Parameter<T>(name, value, min, max), listener, method);
}

template<typename T>
GuiMultiSlider<T> * GuiWidget::addMultiSlider(Parameter<T> *parameter)
{
    GuiMultiSlider<T> *slider = new GuiMultiSlider<T>(parameter);
    initializeElement(slider);
    return slider;
}

template<typename T>
GuiMultiSlider<T> * GuiWidget::addMultiSlider(string name, T *value, T min, T max)
{
    return addMultiSlider(new Parameter<T>(name, value, min, max));
}

template<typename T, typename L, typename M>
GuiMultiSlider<T> * GuiWidget::addMultiSlider(Parameter<T> *parameter, L *listener, M method)
{
    GuiMultiSlider<T> *slider = addMultiSlider(parameter);
    ofAddListener(slider->sliderEvent, listener, method);
    return slider;
}

template<typename T, typename L, typename M>
GuiMultiSlider<T> * GuiWidget::addMultiSlider(string name, T *value, T min, T max, L *listener, M method)
{
    return addMultiSlider(new Parameter<T>(name, value, min, max), listener, method);
}

template<typename T>
GuiRangeSlider<T> * GuiWidget::addRangeSlider(string name, Parameter<T> *parameterLow, Parameter<T> *parameterHigh)
{
    GuiRangeSlider<T> *slider = new GuiRangeSlider<T>(name, parameterLow, parameterHigh);
    initializeElement(slider);
    return slider;
}

template<typename T>
GuiRangeSlider<T> * GuiWidget::addRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max)
{
    return addRangeSlider(name, new Parameter<T>(name+"-low", valueLow, min, max), new Parameter<T>(name+"-high", valueHigh, min, max));
}

template<typename T, typename L, typename M>
GuiRangeSlider<T> * GuiWidget::addRangeSlider(string name, Parameter<T> *parameterLow, Parameter<T> *parameterHigh, L *listener, M method)
{
    GuiRangeSlider<T> *slider = addRangeSlider(name, parameterLow, parameterHigh);
    ofAddListener(slider->rangeSliderEvent, listener, method);
    return slider;
}

template<typename T, typename L, typename M>
GuiRangeSlider<T> * GuiWidget::addRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max, L *listener, M method)
{
    return addRangeSlider(name, new Parameter<T>(name+"-low", valueLow, min, max), new Parameter<T>(name+"-high", valueHigh, min, max), listener, method);
}

template<typename T, typename L, typename M>
GuiRangeSlider<T> * GuiWidget::addRangeSlider(string name, T min, T max, L *listener, M method)
{
    GuiRangeSlider<T> *slider = addRamgeSlider(name, min, max);
    ofAddListener(slider->rangeSliderEvent, listener, method);
    return slider;
}

template<typename T>
GuiMultiRangeSlider<T> * GuiWidget::addMultiRangeSlider(string name, Parameter<T> *parameterLow, Parameter<T> *parameterHigh)
{
    GuiMultiRangeSlider<T> *slider = new GuiMultiRangeSlider<T>(parameterLow, parameterHigh);
    initializeElement(slider);
    return slider;
}

template<typename T>
GuiMultiRangeSlider<T> * GuiWidget::addMultiRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max)
{
    return addMultiRangeSlider(name, new Parameter<T>(name+"-low", valueLow, min, max), new Parameter<T>(name+"-high", valueHigh, min, max));
}

template<typename T, typename L, typename M>
GuiMultiRangeSlider<T> * GuiWidget::addMultiRangeSlider(string name, Parameter<T> *parameterLow, Parameter<T> *parameterHigh, L *listener, M method)
{
    GuiRangeSlider<T> *slider = addMultiRangeSlider(name, parameterLow, parameterHigh);
    ofAddListener(slider->rangeSliderEvent, listener, method);
    return slider;
}

template<typename T, typename L, typename M>
GuiMultiRangeSlider<T> * GuiWidget::addMultiRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max, L *listener, M method)
{
    return addMultiRangeSlider(name, new Parameter<T>(name+"-low", valueLow, min, max), new Parameter<T>(name+"-high", valueHigh, min, max), listener, method);
}

template<typename T, typename L, typename M>
GuiMultiRangeSlider<T> * GuiWidget::addMultiRangeSlider(string name, T min, T max, L *listener, M method)
{
    GuiMultiRangeSlider<T> *slider = addMultiRangeSlider(name, min, max);
    ofAddListener(slider->rangeSliderEvent, listener, method);
    return slider;
}

template<typename L, typename M>
Gui2dPad * GuiWidget::add2dPad(Parameter<ofPoint> *parameter, L *listener, M method)
{
    Gui2dPad *pad = new Gui2dPad(parameter);
    ofAddListener(pad->padEvent, listener, method);
    return pad;
}

template<typename L, typename M>
Gui2dPad * GuiWidget::add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method)
{
    return add2dPad(new Parameter<ofPoint>(name, value, min, max), listener, method);
}

template<typename L, typename M>
Gui2dPad * GuiWidget::add2dPad(string name, ofPoint min, ofPoint max, L *listener, M method)
{
    return add2dPad(new Parameter<ofPoint>(name, new ofPoint(0.5 * (min + max)), min, max), listener, method);
}

template<typename L, typename M>
GuiColor * GuiWidget::addColor(Parameter<ofFloatColor> *parameter, L *listener, M method)
{
    GuiColor *color = addColor(parameter);
    ofAddListener(color->colorEvent, listener, method);
    return color;
}

template<typename L, typename M>
GuiColor * GuiWidget::addColor(string name, ofFloatColor *value, L *listener, M method)
{
    return addColor(new Parameter<ofFloatColor>(name, value, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1)), listener, method);
}

template<typename L, typename M>
GuiMenu * GuiWidget::addMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice, bool autoClose)
{
    GuiMenu *menu = addMenu(name, choices, multipleChoice, autoClose);
    ofAddListener(menu->menuEvent, listener, method);
    return menu;
}

template<typename L, typename M>
GuiMenu * GuiWidget::addMenu(string name, L *listener, M method, bool multipleChoice, bool autoClose)
{
    vector<string> choices;
    return addMenu(name, choices, listener, method);
}
