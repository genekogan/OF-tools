#include "GuiColor.h"


GuiColor::GuiColor(Parameter<ofFloatColor> *parameter) : GuiMultiElement(parameter->getName())
{
    this->parameter = parameter;
    setupColor();
}

GuiColor::GuiColor(string name, ofFloatColor *color) : GuiMultiElement(name)
{
    parameter = new Parameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1));
    setupColor();
}

GuiColor::GuiColor(string name) : GuiMultiElement(name)
{
    parameter = new Parameter<ofFloatColor>(name, new ofFloatColor(0.5, 1.0), ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1));
    setupColor();
}

GuiColor::~GuiColor()
{
    delete parameter;
}

void GuiColor::setupColor()
{
    marginY = GUI_DEFAULT_MARGIN_Y_INNER;
    
    initializeElement(new GuiSlider<float>(name+".r", &parameter->getReference()->r, parameter->getMin().r, parameter->getMax().r));
    initializeElement(new GuiSlider<float>(name+".g", &parameter->getReference()->g, parameter->getMin().g, parameter->getMax().g));
    initializeElement(new GuiSlider<float>(name+".b", &parameter->getReference()->b, parameter->getMin().b, parameter->getMax().b));
    initializeElement(new GuiSlider<float>(name+".a", &parameter->getReference()->a, parameter->getMin().a, parameter->getMax().a));
    for (auto e : elements) {
        ofAddListener(((GuiSlider<float> *) e)->sliderEvent, this, &GuiColor::sliderChanged);
    }
    
    setCollapsible(true);
    setHeader(getName());

    parameter->setOscAddress(getAddress());
}

void GuiColor::sliderChanged(GuiSliderEventArgs<float> &e)
{
    GuiColorEventArgs args(this, parameter->get());
    ofNotifyEvent(colorEvent, args, this);
}

void GuiColor::update()
{
    GuiMultiElement::update();
    ofColor currentColor = parameter->get();
    ofColor textColor = currentColor.r + currentColor.g + currentColor.b > 385 ? GUI_DEFAULT_COLOR_TEXT_DARK : GUI_DEFAULT_COLOR_TEXT_LIGHT;
    elements[0]->setColorForeground(ofColor(currentColor, 255));
    elements[1]->setColorForeground(ofColor(currentColor, 255));
    elements[2]->setColorForeground(ofColor(currentColor, 255));
    elements[3]->setColorForeground(currentColor);
    elements[0]->setColorText(textColor);
    elements[1]->setColorText(textColor);
    elements[2]->setColorText(textColor);
    elements[3]->setColorText(textColor);
}

void GuiColor::getParameters(vector<ParameterBase*> & parameters)
{
    parameters.push_back(parameter);
}

void GuiColor::updateParameterOscAddress()
{
    parameter->setOscAddress(getAddress());
}

