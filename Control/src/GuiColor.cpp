#include "GuiColor.h"


GuiColor::GuiColor(Parameter<ofFloatColor> *parameter) : GuiWidgetBase(parameter->getName())
{
    this->parameter = parameter;
    setupColor();
}

GuiColor::GuiColor(string name, ofFloatColor *value) : GuiWidgetBase(name)
{
    parameter = new Parameter<ofFloatColor>(name, value, ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1));
    setupColor();
}

void GuiColor::setupColor()
{
    marginOuterX = 2;
    marginOuterY = 2;
    GuiElementGroup *elementGroup = new GuiElementGroup();
    elementGroup->addElement(new GuiSlider<float>(parameter->getName()+".r", &parameter->getReference()->r, 0.0f, 1.0f));
    elementGroup->addElement(new GuiSlider<float>(parameter->getName()+".g", &parameter->getReference()->g, 0.0f, 1.0f));
    elementGroup->addElement(new GuiSlider<float>(parameter->getName()+".b", &parameter->getReference()->b, 0.0f, 1.0f));
    elementGroup->addElement(new GuiSlider<float>(parameter->getName()+".a", &parameter->getReference()->a, 0.0f, 1.0f));
    setupElementGroup(elementGroup);
    setList(true);
}

void GuiColor::update()
{
    GuiWidgetBase::update();
    ofColor currentColor = parameter->get();
    ofColor textColor = currentColor.r + currentColor.g + currentColor.b > 385 ? GUI_DEFAULT_COLOR_TEXT_DARK : GUI_DEFAULT_COLOR_TEXT_LIGHT;
    elementGroups[0]->getElements()[0]->setColorForeground(ofColor(currentColor, 255));
    elementGroups[0]->getElements()[1]->setColorForeground(ofColor(currentColor, 255));
    elementGroups[0]->getElements()[2]->setColorForeground(ofColor(currentColor, 255));
    elementGroups[0]->getElements()[3]->setColorForeground(currentColor);
    elementGroups[0]->getElements()[0]->setColorText(textColor);
    elementGroups[0]->getElements()[1]->setColorText(textColor);
    elementGroups[0]->getElements()[2]->setColorText(textColor);
    elementGroups[0]->getElements()[3]->setColorText(textColor);
}