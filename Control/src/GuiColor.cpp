#include "GuiColor.h"

/*
GuiColor::GuiColor(string name, Parameter<ofFloatColor> *parameter) : GuiWidgetBase(name)
{
    this->parameter = parameter;
    setupColor();
}

GuiColor::GuiColor(string name, ofFloatColor *color) : GuiWidgetBase(name)
{
    parameter = new Parameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1));
    setupColor();
}

void GuiColor::setupColor()
{
    elements.push_back(new GuiSlider<float>(name+".r", &parameter->getReference()->r, parameter->getMin().r, parameter->getMax().r));
    elements.push_back(new GuiSlider<float>(name+".g", &parameter->getReference()->g, parameter->getMin().g, parameter->getMax().g));
    elements.push_back(new GuiSlider<float>(name+".b", &parameter->getReference()->b, parameter->getMin().b, parameter->getMax().b));
    elements.push_back(new GuiSlider<float>(name+".a", &parameter->getReference()->a, parameter->getMin().a, parameter->getMax().a));

    isList = true;
    collapsed = false;
    setupGuiComponents();
    GuiElementBase::setAutoUpdate(true);
    GuiElementBase::setAutoDraw(true);
}

void GuiColor::update()
{
    GuiWidgetBase::update();
    
    ofColor currentColor = parameter->get();
    elements[0]->getStyle().colorForeground = ofColor(currentColor.r, currentColor.g, currentColor.b);
    elements[1]->getStyle().colorForeground = ofColor(currentColor.r, currentColor.g, currentColor.b);
    elements[2]->getStyle().colorForeground = ofColor(currentColor.r, currentColor.g, currentColor.b);
    elements[3]->getStyle().colorForeground = currentColor;
}
*/
