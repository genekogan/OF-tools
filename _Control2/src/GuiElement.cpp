#include "GuiElement.h"


GuiElement::GuiElement(string name) : GuiBase(name)
{
    
}

GuiElement::GuiElement() : GuiBase()
{
    
}

void GuiElement::initialize()
{
    GuiBase::initialize();
    hasParent = false;
    parent = NULL;
}

void GuiElement::setParent(GuiElement *parent)
{
    this->parent = parent;
    hasParent = true;
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
}
