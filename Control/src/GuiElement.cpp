#include "GuiElement.h"

GuiElement::GuiElement(string name)
{
    this->name = name;
    dragging = false;
    active = false;
    extraMargin = false;
    
    // default style
    colorFore = GUI_DEFAULT_COLOR_FORE;
    colorBack = GUI_DEFAULT_COLOR_BACK;
    colorActive = GUI_DEFAULT_COLOR_ACTIVE;
    colorText = GUI_DEFAULT_COLOR_TEXT;
    lineWidthActive = GUI_DEFAULT_LINE_WIDTH_ACTIVE;
    
    setRectangle(ofRectangle(0, 0, GUI_WIDTH, GUI_ELEMENT_HEIGHT));
    
    setAutoUpdate(true);
    setAutoDraw(true);
}

GuiElement::~GuiElement()
{
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

void GuiElement::setRectangle(ofRectangle rectangle)
{
    this->rectangle = rectangle;
    setupGuiComponents();
}

void GuiElement::setPosition(int x, int y)
{
    rectangle.set(x, y, rectangle.width, rectangle.height);
    setupGuiComponents();
}

void GuiElement::mouseMoved(int mouseX, int mouseY)
{
    active = rectangle.inside(mouseX, mouseY);
}

void GuiElement::mousePressed(int mouseX, int mouseY)
{
    if (active)
    {
        dragging = true;
    }
}

void GuiElement::mouseDragged(int mouseX, int mouseY)
{

}

void GuiElement::mouseReleased(int mouseX, int mouseY)
{
    dragging = false;
}
