#include "GuiElement.h"


GuiElement::GuiElement(string name) : GuiBase(name)
{
    hasParent = false;
    parent = NULL;
}

GuiElement::GuiElement() : GuiBase()
{
    hasParent = false;
    parent = NULL;
}

void GuiElement::setParent(GuiElement *parent)
{
    this->parent = parent;
    hasParent = true;
}

bool GuiElement::getCollapsed()
{
    return hasParent ? parent->getCollapsed() : false;
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

void GuiElement::setMouseOver(bool mouseOver)
{
    GuiElement::setMouseOver(mouseOver);
    if (mouseOver && hasParent) {
        parent->setMouseOver(mouseOver);
    }
}

void GuiElement::setupGuiPositions()
{
    if (getCollapsed()) {
        rectangle.set(0, 0, 0, 0);
    }
    else {
        rectangle.set(x, y, width, height);
    }
    setupDisplayString();
}

void GuiElement::getXml(ofXml &xml)
{

}

void GuiElement::setFromXml(ofXml &xml)
{
    
}

