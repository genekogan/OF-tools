#include "GuiElement.h"


GuiElement::GuiElement(string name) : GuiBase(name)
{
}

GuiElement::GuiElement() : GuiBase(name)
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