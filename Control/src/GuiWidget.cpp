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

void GuiWidget::addToggle(Parameter<bool> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup();
    elementGroup->addElement(new GuiToggle(parameter));
    setupElementGroup(elementGroup);
}

void GuiWidget::addToggle(string name, bool *value)
{
    addToggle(new Parameter<bool>(name, value));
}

void GuiWidget::addButton(Parameter<bool> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup();
    elementGroup->addElement(new GuiButton(parameter));
    setupElementGroup(elementGroup);
}

void GuiWidget::addButton(string name, bool *value)
{
    addButton(new Parameter<bool>(name, value));
}

void GuiWidget::add2dPad(Parameter<ofPoint> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup();
    elementGroup->addElement(new Gui2dPad(parameter));
    setupElementGroup(elementGroup);
}

void GuiWidget::add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max)
{
    add2dPad(new Parameter<ofPoint>(name, value, min, max));
}

void GuiWidget::addColor(Parameter<ofFloatColor> *parameter)
{
    GuiColor *color = new GuiColor(parameter);
    color->setParent(this);
    GuiElementGroup *elementGroup = new GuiElementGroup();
    elementGroup->addElement(color);
    setupElementGroup(elementGroup);
    ofAddListener(color->widgetChanged, this, &GuiWidget::eventWidgetChanged);
}

void GuiWidget::addColor(string name, ofFloatColor * parameter)
{
    addColor(new Parameter<ofFloatColor>(name, parameter, ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)));
}

void GuiWidget::addColor(string name, ofFloatColor * parameter, ofFloatColor min, ofFloatColor max)
{
    addColor(new Parameter<ofFloatColor>(name, parameter, min, max));
}

GuiMenu * GuiWidget::addMenu(string name, vector<string> choices, bool multipleChoice, bool autoClose)
{
    GuiMenu *menu = new GuiMenu(name, choices, multipleChoice, autoClose);
    menu->setParent(this);
    GuiElementGroup *elementGroup = new GuiElementGroup();
    elementGroup->addElement(menu);
    setupElementGroup(elementGroup);
    ofAddListener(menu->widgetChanged, this, &GuiWidget::eventWidgetChanged);
    return menu;
}

GuiMenu * GuiWidget::addMenu(string name, bool multipleChoice, bool autoClose)
{
    vector<string> choices;
    return addMenu(name, choices, multipleChoice, autoClose);
}

void GuiWidget::eventWidgetChanged(string & s)
{
    setupGuiComponents();
    ofNotifyEvent(widgetChanged, name, this);
}


