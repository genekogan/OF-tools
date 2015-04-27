#include "GuiMultiElement.h"



GuiMultiElement::GuiMultiElement(string name) : GuiElement(name)
{
    collapsed = false;
    hasParent = false;
    parent = NULL;

    headerHeight = GUI_DEFAULT_HEADER_HEIGHT;
    headerColor = GUI_DEFAULT_HEADER_COLOR;
    controllerHeight = GUI_DEFAULT_CONTROLLER_HEIGHT;
    marginX = GUI_DEFAULT_MARGIN_X;
    marginY = GUI_DEFAULT_MARGIN_Y;

    setHeader(getName());
    setCollapsible(false);
    setCollapsed(false);
}

GuiMultiElement::~GuiMultiElement()
{
    clearElements();
}

GuiElement * GuiMultiElement::getElement(string name)
{
    vector<GuiElement*>::iterator it = elements.begin();
    for (; it != elements.end(); ++it)
    {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    ofLog(OF_LOG_ERROR, "Error: no element named "+name+" found in "+getName());
    return NULL;
}

bool GuiMultiElement::removeElement(string name)
{
    vector<GuiElement*>::iterator it = elements.begin();
    while (it != elements.end())
    {
        if ((*it)->getName() == name)
        {
            ofNotifyEvent(removeElementEvent, *it, this);
            delete *it;
            elements.erase(it);
            setupGuiPositions();
            return true;
        }
        else
        {
            if ((*it)->isMultiElement())
            {
                if (((GuiMultiElement *)(*it))->removeElement(name))
                {
                    setupGuiPositions();
                    return true;
                }
            }
            ++it;
        }
    }
    return false;
}

void GuiMultiElement::clearElements()
{
    vector<GuiElement*>::iterator it = elements.begin();
    while (it != elements.end())
    {
        ofNotifyEvent(removeElementEvent, *it, this);
        delete *it;
        elements.erase(it);
    }
    elements.clear();
}

void GuiMultiElement::getParameters(vector<ParameterBase*> & parameters_)
{
    for (auto e : elements) {
        e->getParameters(parameters_);
    }
}

void GuiMultiElement::setHeader(string header)
{
    this->header = header;
    headerStringHeight = ofBitmapStringGetBoundingBox(header, 0, 0).height;
}

void GuiMultiElement::updateParameterOscAddress()
{
    for (auto e : elements) {
        e->updateParameterOscAddress();
    }
}

void GuiMultiElement::setupGuiPositions()
{
    ofPoint topLeft = ofPoint(rectangle.x, rectangle.y);
    if (collapsible && getActive())
    {
        headerRectangle.set(x, y, width, headerHeight);
        topLeft.y += headerHeight;
    }
    else {
        headerRectangle.set(0, 0, 0, 0);
    }

    if (!getCollapsed() && getActive())
    {
        topLeft.y += controllerHeight;
        topLeft.y += collapsible ? marginY : 0;
    
        for (auto e : elements)
        {
            if (e->getActive())
            {
                e->setPosition(topLeft.x + (collapsible ? marginX : 0), topLeft.y);
                e->setWidth(getWidth() - (collapsible ? 2 * marginX : 0));
                topLeft.y += e->getRectangle().height + marginY;
            }
            else
            {
                e->setRectangle(0, 0, 0, 0);
            }
        }
    }
    rectangle.height = topLeft.y - rectangle.y;
}

void GuiMultiElement::addElementToTouchOscLayout(TouchOscPage *page, float *y)
{
    for (auto e : elements) {
        addElementToTouchOscLayout(page, y);
    }
}

void GuiMultiElement::initializeElement(GuiElement *element, bool sendNotification)
{
    elements.push_back(element);
    element->setAutoUpdate(false);
    element->setAutoDraw(false);
    element->setParent(this);
    resetGuiPositions();
    if (sendNotification) {
        ofNotifyEvent(newElementEvent, element, this);
    }
}

void GuiMultiElement::setCollapsible(bool collapsible)
{
    this->collapsible = collapsible;
    setupGuiPositions();
}

void GuiMultiElement::setCollapsed(bool collapsed)
{
    this->collapsed = collapsed;
    resetGuiPositions();
}

bool GuiMultiElement::getCollapsed()
{
    return hasParent ? collapsed || parent->getCollapsed() : collapsed;
}

void GuiMultiElement::update()
{
    for (auto e : elements)
    {
        e->update();
    }
}

void GuiMultiElement::draw()
{
    ofPushStyle();
    
    ofSetLineWidth(1);
    ofSetCircleResolution(16);
    
    ofSetColor(colorBackground, mouseOver ? 110 : 80);
    ofFill();
    ofRect(rectangle);
    
    if (collapsible)
    {
        ofFill();
        ofSetColor(headerColor);
        ofRect(headerRectangle);
        if (headerActive)
        {
            ofSetColor(colorActive);
            ofSetLineWidth(2);
            ofNoFill();
            ofRect(headerRectangle);
        }
        ofSetColor(colorText);
        ofDrawBitmapString(name,    // header??
                           //
                           //
                           //
                           //
                           //
                           //
                           //
                           //
                           
                           
                           rectangle.x + 4,
                           rectangle.y + 1 + 0.5 * (headerHeight + 0.5 * headerStringHeight));
        ofDrawBitmapString(collapsed ? "+" : "-",
                           rectangle.x + rectangle.width - 16,
                           rectangle.y + 1 + 0.5 * (headerHeight + 0.5 * headerStringHeight));
    }
    
    if (!getCollapsed())
    {
        for (auto e : elements)
        {
            if (e->getActive()) {
                e->draw();
            }
        }
    }

    ofPopStyle();
}

bool GuiMultiElement::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    headerActive = headerRectangle.inside(mouseX, mouseY);
    if (!getCollapsed())
    {
        for (auto e : elements) {
            e->mouseMoved(mouseX, mouseY);
        }
    }
    return false;
}

bool GuiMultiElement::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver && !getCollapsed())
    {
        for (auto e : elements) {
            if (e->mousePressed(mouseX, mouseY))  return true;
        }
    }
    return false;
}

bool GuiMultiElement::mouseDragged(int mouseX, int mouseY)
{
    if (!hasParent && (headerActive || draggingWidget))
    {
        setPosition(x + ofGetMouseX() - ofGetPreviousMouseX(), y + ofGetMouseY() - ofGetPreviousMouseY());
        draggingWidget = true;
        return true;
    }
    else if (mouseOver && !getCollapsed())
    {
        for (auto e : elements) {
            if (e->mouseDragged(mouseX, mouseY))  return true;
        }
    }
    return false;
}

bool GuiMultiElement::mouseReleased(int mouseX, int mouseY)
{
    if (headerActive && !draggingWidget)
    {
        draggingWidget = false;
        setCollapsed(!collapsed);
        return true;
    }
    else if (mouseOver && !getCollapsed())
    {
        draggingWidget = false;
        for (auto e : elements) {
            if (e->mouseReleased(mouseX, mouseY))  return true;
        }
    }
    draggingWidget = false;
    return false;
}

bool GuiMultiElement::keyPressed(int key)
{
    GuiElement::keyPressed(key);
    if (mouseOver)
    {
        for (auto e : elements) {
            if (e->keyPressed(key)) return true;
        }
        return true;
    }
    return false;
}

void GuiMultiElement::getXml(ofXml &xml)
{
    for (auto e : elements)
    {
        ofXml xmlElement;
        xmlElement.addChild("Element");
        xmlElement.setTo("Element");
        xmlElement.addValue("Address", e->getAddress());
        e->getXml(xmlElement);
        xml.addXml(xmlElement);
    }
}

void GuiMultiElement::setFromXml(ofXml &xml)
{
    if (!xml.exists("Element[0]"))
    {
        ofLog(OF_LOG_ERROR, "No elements found in preset");
        return;
    }
    
    if (xml.exists("Element[0]"))
    {
        xml.setTo("Element[0]");
        do {
            string name = xml.getValue<string>("Address");
            for (auto e : elements)
            {
                if (e->getAddress() == name) {
                    e->setFromXml(xml);
                }
            }
        }
        while(xml.setToSibling());
        xml.setToParent();
    }
}
