#include "GuiElement.h"


GuiElement::GuiElement(string name)
{
    this->name = name;
    initialize();
}

GuiElement::GuiElement()
{
    initialize();
}

void GuiElement::initialize()
{
    hasParent = false;
    
    elementWidth = GUI_DEFAULT_ELEMENT_WIDTH;
    elementHeight = GUI_DEFAULT_ELEMENT_HEIGHT;
    
    colorBackground = GUI_DEFAULT_COLOR_BACKGROUND;
    colorForeground = GUI_DEFAULT_COLOR_FOREGROUND;
    colorOutline = GUI_DEFAULT_COLOR_OUTLINE;
    colorText = GUI_DEFAULT_COLOR_TEXT;
    colorActive = GUI_DEFAULT_COLOR_ACTIVE;
    
    mouseDragging = false;
    mouseOver = false;
    setAutoUpdate(true);
    setAutoDraw(true);
    setRectangle(0, 0, elementWidth, elementHeight);
}

void GuiElement::setParent(GuiElement *parent)
{
    this->parent = parent;
    hasParent = true;
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
        ofAddListener(ofEvents().keyPressed, this, &GuiElement::keyPressed);
        ofAddListener(ofEvents().update, this, &GuiElement::update);
    }
    else
    {
        ofRemoveListener(ofEvents().mouseMoved, this, &GuiElement::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &GuiElement::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &GuiElement::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &GuiElement::mouseReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &GuiElement::keyPressed);
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

void GuiElement::setRectangle(ofRectangle rectangle)
{
    this->rectangle = rectangle;
    setupGuiPositions();
}

void GuiElement::setRectangle(int x, int y, int width, int height)
{
    setRectangle(ofRectangle(x, y, width, height));
}

void GuiElement::setPosition(ofPoint p)
{
    setPosition(p.x, p.y);
}

void GuiElement::setPosition(int x, int y)
{
    setRectangle(x, y, rectangle.width, rectangle.height);
}

void GuiElement::setSize(ofPoint s)
{
    setSize(s.x, s.y);
}

void GuiElement::setSize(int width, int height)
{
    setRectangle(rectangle.x, rectangle.y, width, height);
}

void GuiElement::setMouseOver(bool mouseOver)
{
    this->mouseOver = mouseOver;
    if (mouseOver && hasParent) {
        parent->setMouseOver(mouseOver);
    }
}

bool GuiElement::mouseMoved(int mouseX, int mouseY)
{
    mouseOver = rectangle.inside(mouseX, mouseY);
    return mouseOver;
}

bool GuiElement::mousePressed(int mouseX, int mouseY)
{
    if (mouseOver)
    {
        mouseDragging = true;
    }
    return false;
}

bool GuiElement::mouseDragged(int mouseX, int mouseY)
{
    return mouseOver;
}

bool GuiElement::mouseReleased(int mouseX, int mouseY)
{
    mouseDragging = false;
    return false;
}

bool GuiElement::keyPressed(int key)
{
    return false;
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

void GuiElement::keyPressed(ofKeyEventArgs &evt)
{
    keyPressed(evt.key);
}

void GuiElement::update(ofEventArgs &data)
{
    update();
}

void GuiElement::draw(ofEventArgs &data)
{
    draw();
}

void GuiElement::draw()
{
    ofPushStyle();
    ofSetColor(colorBackground);
    ofPopStyle();
}
