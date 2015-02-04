#include "GuiWidget.h"


GuiWidgetBase::GuiWidgetBase(string name)
{
    this->name = name;
    header = name;
    active = false;
    headerActive = false;
}

void GuiWidgetBase::setRectanglePosition(ofPoint position)
{
    rectangle.x = position.x;
    rectangle.y = position.y;
    setupGuiComponents();
}

void GuiWidgetBase::setRectangle(ofRectangle rectangle)
{
    this->rectangle = rectangle;
    setupGuiComponents();
}

ofRectangle GuiWidgetBase::getRectangle()
{
    return rectangle;
}

ofPoint GuiWidgetBase::getGuiElementSize()
{
    return ofPoint(GUI_WIDTH - 2*GUI_MARGIN_OUTER - 2*GUI_MARGIN_INNER, GUI_ELEMENT_HEIGHT);
}

string GuiWidgetBase::getName()
{
    return name;
}

string GuiWidgetBase::getHeader()
{
    return header;
}

vector<GuiElement*> & GuiWidgetBase::getElements()
{
    return elements;
}

void GuiWidgetBase::setName(string name)
{
    this->name = name;
}

void GuiWidgetBase::setHeader(string header)
{
    this->header = header;
}

void GuiWidgetBase::setAutoUpdate(bool autoUpdate)
{
    for (auto e : elements) {
        e->setAutoUpdate(autoUpdate);
    }
}

void GuiWidgetBase::setAutoDraw(bool autoDraw)
{
    for (auto e : elements) {
        e->setAutoDraw(autoDraw);
    }
}

void GuiWidgetBase::setup()
{
    
}

void GuiWidgetBase::setupGuiComponents()
{
    ofPoint topLeft = ofPoint(rectangle.x + GUI_MARGIN_INNER, rectangle.y + GUI_MARGIN_INNER);

    if (isList) {
        headerRectangle = ofRectangle(rectangle.x+2, rectangle.y+1, rectangle.width-4, GUI_HEADER_HEIGHT);
        topLeft.y += GUI_HEADER_HEIGHT;
    }
    
    ofPoint guiElementSize = getGuiElementSize();
    for (auto e : elements)
    {
        if (collapsed) {
            e->setRectangle(ofRectangle(0, 0, 0, 0));
        }
        else
        {
            e->setRectangle(ofRectangle(topLeft.x, topLeft.y, guiElementSize.x, guiElementSize.y));
            topLeft += ofPoint(0, guiElementSize.y + (e->getExtraMargin() ? 3 * GUI_MARGIN_BETWEEN : GUI_MARGIN_BETWEEN));
        }
    }
 
    rectangle.width = guiElementSize.x + 2*GUI_MARGIN_INNER;
    rectangle.height = collapsed ? topLeft.y - rectangle.y : topLeft.y + GUI_MARGIN_OUTER - GUI_MARGIN_BETWEEN - rectangle.y;
}

void GuiWidgetBase::update()
{
    for (auto e : elements)
    {
        e->update();
    }
}
/*
void GuiWidgetBase::updateParameter(string &s)
{
    
}
*/
void GuiWidgetBase::mouseMoved(int mouseX, int mouseY)
{
    setActive(rectangle.inside(mouseX, mouseY));
    headerActive = headerRectangle.inside(mouseX, mouseY);
    if (active)
    {
        for (auto e : elements)
        {
            e->mouseMoved(mouseX, mouseY);
        }
    }
}

void GuiWidgetBase::mousePressed(int mouseX, int mouseY)
{
    if (active)
    {
        if (isList && headerRectangle.inside(mouseX, mouseY))
        {
            collapsed = !collapsed;
            ofNotifyEvent(widgetChanged, name, this);
            return;
        }
        for (auto e : elements)
        {
            e->mousePressed(mouseX, mouseY);
        }
    }
}

void GuiWidgetBase::mouseReleased(int mouseX, int mouseY)
{
    if (active)
    {
        for (auto e : elements)
        {
            e->mouseReleased(mouseX, mouseY);
        }
    }
}

void GuiWidgetBase::mouseDragged(int mouseX, int mouseY)
{
    if (active)
    {
        for (auto e : elements)
        {
            e->mouseDragged(mouseX, mouseY);
        }
    }
}

void GuiWidgetBase::draw()
{
    ofPushMatrix();
    ofPushStyle();
    
    active ? ofSetColor(GUI_COLOR_ACTIVE) : ofSetColor(GUI_COLOR_INACTIVE);
    ofFill();
    ofSetLineWidth(0);
    ofRect(rectangle);
    
    ofNoFill();
    ofSetColor(255, 50);
    ofSetLineWidth(1);
    ofRect(rectangle);
    
    if (isList)
    {
        ofFill();
        ofSetColor(0, 0, 255);
        ofRect(headerRectangle);
        if (headerActive)
        {
            ofSetColor(255, 255, 0);
            ofNoFill();
            ofRect(headerRectangle);
        }
        ofSetColor(255);
        ofDrawBitmapString(header, rectangle.x+4, rectangle.y+GUI_HEADER_HEIGHT-3);
        ofDrawBitmapString(collapsed ? "+" : "-", rectangle.x + rectangle.width - 16, rectangle.y + GUI_HEADER_HEIGHT-3);
    }
    
    if (!collapsed)
    {
        for (auto e : elements)
        {
            e->draw();
        }
    }
    
    ofPopStyle();
    ofPopMatrix();
}

void GuiWidgetBase::setActive(bool active)
{
    this->active = active;
}

bool GuiWidgetBase::getActive()
{
    return active;
}


