#include "GuiElement.h"


GuiElementBase::GuiElementBase(string name)
{
    this->name = name;
    mouseDragging = false;
    mouseOver = false;
    
    setAutoUpdate(true);
    setAutoDraw(true);
}

GuiElementBase::~GuiElementBase()
{
    setAutoUpdate(false);
    setAutoDraw(false);
}

void GuiElementBase::setAutoUpdate(bool autoUpdate)
{
    this->autoUpdate = autoUpdate;
    
    if (autoUpdate)
    {
        ofAddListener(ofEvents().mouseMoved, this, &GuiElementBase::mouseMoved);
        ofAddListener(ofEvents().mousePressed, this, &GuiElementBase::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &GuiElementBase::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &GuiElementBase::mouseReleased);
        ofAddListener(ofEvents().update, this, &GuiElementBase::update);
    }
    else
    {
        ofRemoveListener(ofEvents().mouseMoved, this, &GuiElementBase::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &GuiElementBase::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &GuiElementBase::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &GuiElementBase::mouseReleased);
        ofRemoveListener(ofEvents().update, this, &GuiElementBase::update);
    }
}

void GuiElementBase::setAutoDraw(bool autoDraw)
{
    this->autoDraw = autoDraw;
    
    if (autoDraw)
    {
        ofAddListener(ofEvents().draw, this, &GuiElementBase::draw);
    }
    else
    {
        ofRemoveListener(ofEvents().draw, this, &GuiElementBase::draw);
    }
}

void GuiElementBase::setName(string name)
{
    this->name = name;
}

string GuiElementBase::getName()
{
    return name;
}

void GuiElementBase::setRectangle(ofRectangle rectangle)
{
    this->rectangle = rectangle;
    setupGuiComponents();
}

void GuiElementBase::setRectangle(int x, int y, int width, int height)
{
    setRectangle(ofRectangle(x, y, width, height));
}

void GuiElementBase::setPosition(int x, int y)
{
    setRectangle(x, y, rectangle.width, rectangle.height);
}

void GuiElementBase::setPosition(ofPoint p)
{
    setPosition(p.x, p.y);
}

ofRectangle GuiElementBase::getRectangle()
{
    return rectangle;
}

void GuiElementBase::mouseMoved(int mouseX, int mouseY)
{
    mouseOver = rectangle.inside(mouseX, mouseY);
}

void GuiElementBase::mousePressed(int mouseX, int mouseY)
{
    if (mouseOver)
    {
        mouseDragging = true;
    }
}

void GuiElementBase::mouseDragged(int mouseX, int mouseY)
{
    
}

void GuiElementBase::mouseReleased(int mouseX, int mouseY)
{
    mouseDragging = false;
}

void GuiElementBase::mouseMoved(ofMouseEventArgs &evt)
{
    mouseMoved(evt.x, evt.y);
}

void GuiElementBase::mousePressed(ofMouseEventArgs &evt)
{
    mousePressed(evt.x, evt.y);
}

void GuiElementBase::mouseDragged(ofMouseEventArgs &evt)
{
    mouseDragged(evt.x, evt.y);
}

void GuiElementBase::mouseReleased(ofMouseEventArgs &evt)
{
    mouseReleased(evt.x, evt.y);
}

void GuiElementBase::update(ofEventArgs &data)
{
    update();
}

void GuiElementBase::draw(ofEventArgs &data)
{
    draw();
}


GuiElement::GuiElement(string name) : GuiElementBase(name)
{
    setRectangle(0, 0, GUI_DEFAULT_ELEMENT_WIDTH, GUI_DEFAULT_ELEMENT_HEIGHT);
    extraMargin = false;
}

void GuiElement::setExtraMargin(bool extraMargin)
{
    this->extraMargin = extraMargin;
}

bool GuiElement::getExtraMargin()
{
    return extraMargin;
}

bool GuiElement::isDiscrete()
{
    return false;
}

GuiStyle & GuiElement::getStyle()
{
    return style;
}
