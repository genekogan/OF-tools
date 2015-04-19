#include "GuiElement.h"


GuiElement::GuiElement(string name) : GuiBase(name)
{
    hasParent = false;
    parent = NULL;
    display = "";
}

GuiElement::GuiElement() : GuiBase()
{
    hasParent = false;
    parent = NULL;
    display = "";
}

GuiElement::~GuiElement()
{

}

void GuiElement::setParent(GuiElement *parent)
{
    this->parent = parent;
    hasParent = true;
    display = "";
}

string GuiElement::getAddress()
{
    return hasParent ? parent->getAddress() + "/" + getName() : getName();
}

bool GuiElement::getActive()
{
    return hasParent ? parent->getActive() && active : active;
}

void GuiElement::setActive(bool active)
{
    GuiBase::setActive(active);
    resetGuiPositions();
}

bool GuiElement::getCollapsed()
{
    return hasParent ? parent->getCollapsed() : false;
}

void GuiElement::setMouseOver(bool mouseOver)
{
    GuiBase::setMouseOver(mouseOver);
    if (mouseOver && hasParent) {
        parent->setMouseOver(mouseOver);
    }
}

void GuiElement::setupGuiPositions()
{
    if (getCollapsed())
    {
        rectangle.set(0, 0, 0, 0);
    }
    else
    {
        rectangle.set(x, y, width, height);
        setupDisplayString();
    }
}

void GuiElement::resetGuiPositions()
{
    if (hasParent) {
        parent->resetGuiPositions();
    }
    else {
        setupGuiPositions();
    }
}

void GuiElement::addElementToTouchOscLayout(TouchOscPage *page, float *y)
{

}

void GuiElement::updateParameterOscAddress()
{
    
}

void GuiElement::getXml(ofXml &xml)
{

}

void GuiElement::setFromXml(ofXml &xml)
{
    
}

void GuiElement::setupDisplayString()
{
    display = name;
    int displayWidth = ofBitmapStringGetBoundingBox(display, 0, 0).width;
    while (displayWidth > getWidth())
    {
        display = display.substr(0, display.length()-1);
        displayWidth = ofBitmapStringGetBoundingBox(display, 0, 0).width;
    }
}
