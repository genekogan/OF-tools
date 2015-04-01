#include "GuiBase.h"


GuiBase::GuiBase(string name)
{
    setName(name);
    initialize();
}

GuiBase::GuiBase()
{
    setName("element");
    initialize();
}

GuiBase::~GuiBase()
{
    setActive(false);
}

void GuiBase::initialize()
{
    //hasParent = false;
    //parent = NULL;
    //
    x = 0;
    y = 0;
    width = GUI_DEFAULT_ELEMENT_WIDTH;
    height = GUI_DEFAULT_ELEMENT_HEIGHT;
    setRectangle(0, 0, width, height);
    
    colorBackground = GUI_DEFAULT_COLOR_BACKGROUND;
    colorForeground = GUI_DEFAULT_COLOR_FOREGROUND;
    colorOutline = GUI_DEFAULT_COLOR_OUTLINE;
    colorText = GUI_DEFAULT_COLOR_TEXT;
    colorActive = GUI_DEFAULT_COLOR_ACTIVE;
    
    mouseDragging = false;
    mouseOver = false;
    
    autoUpdate = true;
    autoDraw = true;
    setActive(true);
}

void GuiBase::setParent(GuiBase *parent)
{
    this->parent = parent;
    hasParent = true;
}

void GuiBase::setActive(bool active)
{
    this->active = active;
    if (active)
    {
        setAutoUpdate(autoUpdate);
        setAutoDraw(autoDraw);
    }
    else
    {
        ofRemoveListener(ofEvents().mouseMoved, this, &GuiBase::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &GuiBase::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &GuiBase::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &GuiBase::mouseReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &GuiBase::keyPressed);
        ofRemoveListener(ofEvents().update, this, &GuiBase::update);
        ofRemoveListener(ofEvents().draw, this, &GuiBase::draw);
    }
}

void GuiBase::setAutoUpdate(bool autoUpdate)
{
    this->autoUpdate = autoUpdate;
    if (autoUpdate)
    {
        ofAddListener(ofEvents().mouseMoved, this, &GuiBase::mouseMoved);
        ofAddListener(ofEvents().mousePressed, this, &GuiBase::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &GuiBase::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &GuiBase::mouseReleased);
        ofAddListener(ofEvents().keyPressed, this, &GuiBase::keyPressed);
        ofAddListener(ofEvents().update, this, &GuiBase::update);
    }
    else
    {
        ofRemoveListener(ofEvents().mouseMoved, this, &GuiBase::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &GuiBase::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &GuiBase::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &GuiBase::mouseReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &GuiBase::keyPressed);
        ofRemoveListener(ofEvents().update, this, &GuiBase::update);
    }
}

void GuiBase::setAutoDraw(bool autoDraw)
{
    this->autoDraw = autoDraw;
    if (autoDraw) {
        ofAddListener(ofEvents().draw, this, &GuiBase::draw);
    }
    else {
        ofRemoveListener(ofEvents().draw, this, &GuiBase::draw);
    }
}

void GuiBase::setX(int x)
{
    this->x = x;
    if (!getCollapsed()) {
        setRectangle(x, y, width, height);
    }
}

void GuiBase::setY(int y)
{
    this->y = y;
    setRectangle(x, y, width, height);
}

void GuiBase::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
    setRectangle(x, y, width, height);
}

void GuiBase::setWidth(int width)
{
    this->width = width;
    setRectangle(x, y, width, height);
}

void GuiBase::setHeight(int height)
{
    this->height = height;
    setRectangle(x, y, width, height);
}

void GuiBase::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
    setRectangle(x, y, width, height);
}

void GuiBase::setRectangle(ofRectangle rectangle)
{
    this->rectangle = rectangle;
    setupGuiPositions();
}

void GuiBase::setMouseOver(bool mouseOver)
{
    this->mouseOver = mouseOver;
    if (mouseOver && hasParent) {
        parent->setMouseOver(mouseOver);
    }
}

void GuiBase::setupGuiPositions()
{
    if (getCollapsed()) {
        rectangle.set(0, 0, 0, 0);
    }
    else {
        rectangle.set(x, y, width, height);
    }
}

/*
void GuiBase::resetGuiPositions()
{
    if (hasParent) {
        parent->resetGuiPositions();
    }
    else {
        setupGuiPositions();
    }
}*/

bool GuiBase::mouseMoved(int mouseX, int mouseY)
{
    mouseOver = active && rectangle.inside(mouseX, mouseY);
    return mouseOver;
}

bool GuiBase::mousePressed(int mouseX, int mouseY)
{
    if (mouseOver)
    {
        mouseDragging = true;
    }
    return mouseOver;
}

bool GuiBase::mouseDragged(int mouseX, int mouseY)
{
    return mouseOver && mouseDragging;
}

bool GuiBase::mouseReleased(int mouseX, int mouseY)
{
    mouseDragging = false;
    return mouseOver;
}

bool GuiBase::keyPressed(int key)
{
    return false;
}

void GuiBase::update()
{
    
}

void GuiBase::draw()
{
    ofPushStyle();
    ofSetColor(colorBackground);
    ofPopStyle();
}

void GuiBase::mouseMoved(ofMouseEventArgs &evt)
{
    mouseMoved(evt.x, evt.y);
}

void GuiBase::mousePressed(ofMouseEventArgs &evt)
{
    mousePressed(evt.x, evt.y);
}

void GuiBase::mouseDragged(ofMouseEventArgs &evt)
{
    mouseDragged(evt.x, evt.y);
}

void GuiBase::mouseReleased(ofMouseEventArgs &evt)
{
    mouseReleased(evt.x, evt.y);
}

void GuiBase::keyPressed(ofKeyEventArgs &evt)
{
    keyPressed(evt.key);
}

void GuiBase::update(ofEventArgs &data)
{
    update();
}

void GuiBase::draw(ofEventArgs &data)
{
    draw();
}
