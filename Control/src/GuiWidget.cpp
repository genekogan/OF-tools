#include "GuiWidget.h"


GuiWidget::GuiWidget() : GuiWidgetBase()
{

}

GuiWidget::GuiWidget(string name) : GuiWidgetBase(name)
{

}

GuiWidget::~GuiWidget()
{
    
}

GuiButton * GuiWidget::addButton(Parameter<bool> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup(parameter->getName());
    GuiButton *newButton = new GuiButton(parameter);
    elementGroup->addElement(newButton);
    parameter->setOscAddress(getAddress()+parameter->getOscAddress());
    parameters.push_back(parameter);
    setupElementGroup(elementGroup);
    return newButton;
}

GuiButton * GuiWidget::addButton(string name, bool *value)
{
    return addButton(new Parameter<bool>(name, value));
}

GuiToggle * GuiWidget::addToggle(Parameter<bool> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup(parameter->getName());
    GuiToggle *newToggle = new GuiToggle(parameter);
    elementGroup->addElement(newToggle);
    parameter->setOscAddress(getAddress()+parameter->getOscAddress());
    parameters.push_back(parameter);
    setupElementGroup(elementGroup);
    return newToggle;
}

GuiToggle * GuiWidget::addToggle(string name, bool *value)
{
    return addToggle(new Parameter<bool>(name, value));
}

GuiTextBox * GuiWidget::addTextBox(Parameter<string> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup(parameter->getName());
    GuiTextBox *newTextBox = new GuiTextBox(parameter);
    elementGroup->addElement(newTextBox);
    parameter->setOscAddress(getAddress()+parameter->getOscAddress());
    parameters.push_back(parameter);
    setupElementGroup(elementGroup);
    return newTextBox;
}

GuiTextBox * GuiWidget::addTextBox(string name, string *value)
{
    return addTextBox(new Parameter<string>(name, value));
}

Gui2dPad * GuiWidget::add2dPad(Parameter<ofPoint> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup(parameter->getName());
    Gui2dPad *new2dPad = new Gui2dPad(parameter);
    elementGroup->addElement(new2dPad);
    parameter->setOscAddress(getAddress()+parameter->getOscAddress());
    parameters.push_back(parameter);
    setupElementGroup(elementGroup);
    ofAddListener(new2dPad->widgetChanged, this, &GuiWidget::eventWidgetChanged);
    return new2dPad;
}

Gui2dPad * GuiWidget::add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max)
{
    return add2dPad(new Parameter<ofPoint>(name, value, min, max));
}

GuiColor * GuiWidget::addColor(Parameter<ofFloatColor> *parameter)
{
    GuiColor *color = new GuiColor(parameter);
    color->setParent(this);
    GuiElementGroup *elementGroup = new GuiElementGroup(parameter->getName());
    elementGroup->addElement(color);
    parameter->setOscAddress(getAddress()+parameter->getOscAddress());
    parameters.push_back(parameter);
    setupElementGroup(elementGroup);
    ofAddListener(color->widgetChanged, this, &GuiWidget::eventWidgetChanged);
    return color;
}

GuiColor * GuiWidget::addColor(string name, ofFloatColor * parameter)
{
    return addColor(new Parameter<ofFloatColor>(name, parameter, ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)));
}

GuiColor * GuiWidget::addColor(string name, ofFloatColor * parameter, ofFloatColor min, ofFloatColor max)
{
    return addColor(new Parameter<ofFloatColor>(name, parameter, min, max));
}

GuiMenu * GuiWidget::addMenu(string name, vector<string> choices, bool multipleChoice, bool autoClose)
{
    GuiMenu *newMenu = new GuiMenu(name, choices, multipleChoice, autoClose);
    newMenu->setAddress(getAddress()+newMenu->getAddress());
    newMenu->setParent(this);
    GuiElementGroup *elementGroup = new GuiElementGroup(name);
    elementGroup->addElement(newMenu);
    setupElementGroup(elementGroup);
    ofAddListener(newMenu->widgetChanged, this, &GuiWidget::eventWidgetChanged);
    return newMenu;
}

GuiMenu * GuiWidget::addMenu(string name, bool multipleChoice, bool autoClose)
{
    vector<string> choices;
    return addMenu(name, choices, multipleChoice, autoClose);
}

void GuiWidget::eventWidgetChanged(string & s)
{
    setupGuiPositions();
    ofNotifyEvent(widgetChanged, name, this);
}

