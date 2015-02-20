#include "GuiElement.h"


GuiElement::GuiElement(string name)
{
    this->name = name;
    mouseDragging = false;
    mouseOver = false;
    extraMargin = false;
    setAutoUpdate(true);
    setAutoDraw(true);
    setRectangle(0, 0, style.elementWidth, style.elementHeight);
}

GuiElement::~GuiElement()
{
    elementEvent.clear();
    elementEvent.disable();
    setAutoUpdate(false);
    setAutoDraw(false);
}

void GuiElement::setAutoUpdate(bool autoUpdate)
{
    this->autoUpdate = autoUpdate;
    if (autoUpdate)
    {
        ofAddListener(ofEvents().mouseMoved, this, &GuiElement::mouseMoved);
        ofAddListener(ofEvents().mousePressed, this, &GuiElement::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &GuiElement::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &GuiElement::mouseReleased);
        ofAddListener(ofEvents().update, this, &GuiElement::update);
    }
    else
    {
        ofRemoveListener(ofEvents().mouseMoved, this, &GuiElement::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &GuiElement::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &GuiElement::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &GuiElement::mouseReleased);
        ofRemoveListener(ofEvents().update, this, &GuiElement::update);
    }
}

void GuiElement::setAutoDraw(bool autoDraw)
{
    this->autoDraw = autoDraw;
    if (autoDraw) {
        ofAddListener(ofEvents().draw, this, &GuiElement::draw);
    }
    else {
        ofRemoveListener(ofEvents().draw, this, &GuiElement::draw);
    }
}

void GuiElement::setName(string name)
{
    this->name = name;
}

string GuiElement::getName()
{
    return name;
}

void GuiElement::setRectangle(ofRectangle rectangle)
{
    this->rectangle = rectangle;
    setupGuiComponents();
}

void GuiElement::setRectangle(int x, int y, int width, int height)
{
    setRectangle(ofRectangle(x, y, width, height));
}

void GuiElement::setPosition(int x, int y)
{
    setRectangle(x, y, rectangle.width, rectangle.height);
}

void GuiElement::setPosition(ofPoint p)
{
    setPosition(p.x, p.y);
}

ofRectangle GuiElement::getRectangle()
{
    return rectangle;
}

void GuiElement::mouseMoved(int mouseX, int mouseY)
{
    mouseOver = rectangle.inside(mouseX, mouseY);
}

void GuiElement::mousePressed(int mouseX, int mouseY)
{
    if (mouseOver)
    {
        mouseDragging = true;
    }
}

void GuiElement::mouseDragged(int mouseX, int mouseY)
{
    
}

void GuiElement::mouseReleased(int mouseX, int mouseY)
{
    mouseDragging = false;
}

void GuiElement::mouseMoved(ofMouseEventArgs &evt)
{
    mouseMoved(evt.x, evt.y);
}

void GuiElement::mousePressed(ofMouseEventArgs &evt)
{
    mousePressed(evt.x, evt.y);
}

void GuiElement::mouseDragged(ofMouseEventArgs &evt)
{
    mouseDragged(evt.x, evt.y);
}

void GuiElement::mouseReleased(ofMouseEventArgs &evt)
{
    mouseReleased(evt.x, evt.y);
}

void GuiElement::update(ofEventArgs &data)
{
    update();
}

void GuiElement::draw(ofEventArgs &data)
{
    draw();
}

GuiStyle & GuiElement::getStyle()
{
    return style;
}

void GuiElement::setExtraMargin(bool extraMargin)
{
    this->extraMargin = extraMargin;
}

bool GuiElement::getExtraMargin()
{
    return extraMargin;
}
