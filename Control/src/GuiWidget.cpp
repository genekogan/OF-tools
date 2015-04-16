#include "GuiWidget.h"


GuiWidget::GuiWidget(string name) : GuiMultiElement(name)
{
    
}

GuiWidget::GuiWidget() : GuiMultiElement("Widget")
{
    
}

void GuiWidget::initializeElement(GuiElement *element, bool sendNotification)
{
    GuiMultiElement::initializeElement(element, sendNotification);
    if (sendNotification) {
        elementGroups.push_back(element);
    }
    if (elements.size() > 1) {
        setCollapsible(true);
    }
}

GuiWidget * GuiWidget::addWidget(GuiWidget *newWidget)
{
    initializeElement(newWidget, false);
    for (auto e : newWidget->getElementGroups()) {
        widgetNewElementAdded(e);
    }
    ofAddListener(newWidget->newElementEvent, this, &GuiWidget::widgetNewElementAdded);
    return newWidget;
}

GuiWidget * GuiWidget::addWidget(string widgetName)
{
    return addWidget(new GuiWidget(widgetName));
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

void GuiWidget::widgetNewElementAdded(GuiElement* & newElement)
{
    elementGroups.push_back(newElement);
    ofNotifyEvent(newElementEvent, newElement, this);
}

void GuiWidget::getParameters(vector<ParameterBase*> & parameters)
{
    for (auto e : elementGroups) {
        e->getParameters(parameters);
    }
}

void GuiWidget::updateParameterOscAddress()
{
    for (auto e : elementGroups) {
        e->updateParameterOscAddress();
    }
}

void GuiWidget::setupGuiPositions()
{
    GuiMultiElement::setupGuiPositions();
    for (auto w : attachedWidgets) {
        w->setPosition(rectangle.x + rectangle.width + marginX, rectangle.y);
    }
}

void GuiWidget::addElementToTouchOscLayout(TouchOscPage *page, float *y)
{
}

void GuiWidget::attachWidget(GuiWidget *other)
{
    attachedWidgets.push_back(other);
    other->setPosition(rectangle.x + rectangle.width + marginX, rectangle.y);
}

void GuiWidget::detachWidget(GuiWidget *other)
{
    vector<GuiWidget*>::iterator it = attachedWidgets.begin();
    while (it != attachedWidgets.end()) {
        if (*it == other) {
            attachedWidgets.erase(it);
        }
        else {
            ++it;
        }
    }
}

void GuiWidget::makeTouchOscLayout(string filename)
{
    
//    for (auto e : elementGroups) {
  //      e->updateParameterOscAddress();
    //}
    
    TouchOsc touchOsc;
    
    touchOsc.setScale(320, 480);
    touchOsc.setDefaultColor(GREEN);
    
    TouchOscPage *page1 = touchOsc.addPage("myPage");
    
    
    float y = 0;
    
    for (auto e : elementGroups) {
        
        e->addElementToTouchOscLayout(page1, &y);
        cout << y << endl;
    }
    
    for (auto w : page1->getWidgets()) {
        w->y = ofMap(w->y, 0, y, 0, 0.9);
        w->h = 0.9 * w->h / y;
    }

    
    
    
    
    
    touchOsc.save("myTouchOscLayout");
    
    
}

void GuiWidget::getXml(ofXml &xml)
{
    xml.addChild("Elements");
    xml.setTo("Elements");
    for (auto e : elements)
    {
        ofXml xmlElement;
        xmlElement.addChild("Element");
        xmlElement.setTo("Element");
        xmlElement.addValue("Address", e->getAddress());
        e->getXml(xmlElement);
        xml.addXml(xmlElement);
    }
    xml.setToParent();
}

void GuiWidget::setFromXml(ofXml &xml)
{
    if (!xml.exists("Elements"))
    {
        ofLog(OF_LOG_ERROR, "No elements found in preset");
        return;
    }
    
    xml.setTo("Elements");
    if (xml.exists("Element[0]"))
    {
        xml.setTo("Element[0]");
        do {
            string address = xml.getValue<string>("Address");
            for (auto e : elements)
            {
                if (e->getAddress() == address) {
                    e->setFromXml(xml);
                }
            }
        }
        while(xml.setToSibling());
        xml.setToParent();
    }
    xml.setToParent();
}
