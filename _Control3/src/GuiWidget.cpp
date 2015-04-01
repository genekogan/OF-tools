#include "GuiWidget.h"


GuiWidget::GuiWidget(string name) : GuiMultiElement(name)
{
    
}

GuiWidget::GuiWidget() : GuiMultiElement("Widget")
{
    
}

GuiWidget * GuiWidget::addWidget(GuiWidget *newWidget)
{
    initializeElement(newWidget);
    return newWidget;
}

GuiWidget * GuiWidget::addWidget(string widgetName)
{
    return addWidget(new GuiWidget(widgetName));
}

void GuiWidget::initializeElement(GuiElement *element)
{
    GuiMultiElement::initializeElement(element);
    if (elements.size() > 1) {
        setCollapsible(true);
    }
}

GuiButton * GuiWidget::addButton(Parameter<bool> *parameter)
{
    GuiButton *button = new GuiButton(parameter);
    initializeElement(button);
    return button;
}

GuiButton * GuiWidget::addButton(string name, bool *value)
{
    return addButton(new Parameter<bool>(name, value));
}

GuiToggle * GuiWidget::addToggle(Parameter<bool> *parameter)
{
    GuiToggle *toggle = new GuiToggle(parameter);
    initializeElement(toggle);
    return toggle;
}

GuiToggle * GuiWidget::addToggle(string name, bool *value)
{
    return addToggle(new Parameter<bool>(name, value));
}

GuiTextBox * GuiWidget::addTextBox(Parameter<string> *parameter)
{
    GuiTextBox *textBox = new GuiTextBox(parameter);
    initializeElement(textBox);
    return textBox;
}

GuiTextBox * GuiWidget::addTextBox(string name, string *value)
{
    return addTextBox(new Parameter<string>(name, value));
}

Gui2dPad * GuiWidget::add2dPad(Parameter<ofPoint> *parameter)
{
    Gui2dPad *pad = new Gui2dPad(parameter);
    initializeElement(pad);
    return pad;
}

Gui2dPad * GuiWidget::add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max)
{
    return add2dPad(new Parameter<ofPoint>(name, value, min, max));
}

Gui2dPad * GuiWidget::add2dPad(string name, ofPoint min, ofPoint max)
{
    Gui2dPad *pad = new Gui2dPad(name, min, max);
    initializeElement(pad);
    return pad;
}

GuiColor * GuiWidget::addColor(Parameter<ofFloatColor> *parameter)
{
    GuiColor *color = new GuiColor(parameter);
    initializeElement(color);
    return color;
}

GuiColor * GuiWidget::addColor(string name, ofFloatColor *value)
{
    return addColor(new Parameter<ofFloatColor>(name, value, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1)));
}

GuiMenu * GuiWidget::addMenu(string name, vector<string> choices, bool multipleChoice, bool autoClose)
{
    GuiMenu *menu = new GuiMenu(name, choices, multipleChoice, autoClose);
    initializeElement(menu);
    return menu;
}

GuiMenu * GuiWidget::addMenu(string name, bool multipleChoice, bool autoClose)
{
    vector<string> choices;
    return addMenu(name, choices, multipleChoice, autoClose);
}
