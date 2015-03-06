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

GuiToggle * GuiWidget::addToggle(Parameter<bool> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup();
    GuiToggle *newToggle = new GuiToggle(parameter);
    elementGroup->addElement(newToggle);
    setupElementGroup(elementGroup);
    return newToggle;
}

GuiToggle * GuiWidget::addToggle(string name, bool *value)
{
    return addToggle(new Parameter<bool>(name, value));
}

GuiButton * GuiWidget::addButton(Parameter<bool> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup();
    GuiButton *newButton = new GuiButton(parameter);
    elementGroup->addElement(newButton);
    setupElementGroup(elementGroup);
    return newButton;
}

GuiButton * GuiWidget::addButton(string name, bool *value)
{
    return addButton(new Parameter<bool>(name, value));
}

Gui2dPad * GuiWidget::add2dPad(Parameter<ofPoint> *parameter)
{
    GuiElementGroup *elementGroup = new GuiElementGroup();
    Gui2dPad *new2dPad = new Gui2dPad(parameter);
    elementGroup->addElement(new2dPad);
    setupElementGroup(elementGroup);
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
    GuiElementGroup *elementGroup = new GuiElementGroup();
    elementGroup->addElement(color);
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


