#include "OscManagerPanel.h"

OscManagerPanel::ElementOscGroup::ElementOscGroup(GuiElement *element, OscManager *osc)
{
    this->element = element;
    this->osc = osc;
    address = element->getAddress();
    ofStringReplace(address, " ", "_");
    tAddress = new GuiTextBox("address", new string(address), this, &ElementOscGroup::eventSetAddress);
    tAddress->setAutoUpdate(false);
    tAddress->setAutoDraw(false);
    tSend = new GuiToggle("s", &sending, this, &ElementOscGroup::eventSetSending);
    tSend->setAutoUpdate(false);
    tSend->setAutoDraw(false);
}

void OscManagerPanel::ElementOscGroup::updateAddress()
{
    vector<ParameterBase*> parameters;
    element->getParameters(parameters);
    for (auto p : parameters) {
        p->setOscAddress(address);
    }
}

void OscManagerPanel::ElementOscGroup::eventSetAddress(GuiTextBoxEventArgs &evt)
{
    address = evt.textBox->getValue();
    ofStringReplace(address, " ", "_");
    tAddress->setValue(address, false);
    if (sending) {
        updateAddress();
    }
}

void OscManagerPanel::ElementOscGroup::eventSetSending(GuiButtonEventArgs &evt)
{
    updateAddress();
    vector<ParameterBase*> parameters;
    element->getParameters(parameters);
    for (auto p : parameters)
    {
        if (sending) {
            osc->addParameterToSender(p);
        }
        else {
            osc->removeParameterFromSender(p);
        }
    }
}

OscManagerPanel::OscManagerPanel(string name, GuiWidget * panel, OscManager *osc) : GuiElement(name)
{
    this->panel = panel;
    this->osc = osc;
    this->oscManagerWidth = GUI_DEFAULT_OSCMANAGER_WIDTH;
    
    tTouchOsc = new GuiButton("touchOsc", this, &OscManagerPanel::eventMakeTouchOscLayout);
    tTouchOsc->setAutoUpdate(false);
    tTouchOsc->setAutoDraw(false);

    ofAddListener(panel->newElementEvent, this, &OscManagerPanel::addElement);
    ofAddListener(panel->removeElementEvent, this, &OscManagerPanel::removeElement);
    
    setupOscManagerPanel();
    
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
}

OscManagerPanel::~OscManagerPanel()
{
    ofRemoveListener(panel->newElementEvent, this, &OscManagerPanel::addElement);
    ofRemoveListener(panel->removeElementEvent, this, &OscManagerPanel::removeElement);
}

void OscManagerPanel::setupOscManagerPanel()
{
    for (auto element : panel->getElementGroups()) {
        addElement(element);
    }
}

void OscManagerPanel::addElement(GuiElement* & newElement)
{
    if (groups.count(newElement) == 0)
    {
        groups[newElement] = new ElementOscGroup(newElement, osc);
        setupGuiPositions();
    }
}

void OscManagerPanel::removeElement(GuiElement* & element)
{
    map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
    while (it != groups.end())
    {
        if (it->first == element ||
            (it->second->element->getHasParent() && (it->second->element->getParent() == element)))
        {
            delete it->second;
            groups.erase(it++);
            setupGuiPositions();
        }
    }
}

void OscManagerPanel::setupGuiPositions()
{
    ofRectangle panelRect = panel->getRectangle();
    rectangle.set(panelRect.x + panelRect.getWidth() + 5, panelRect.y, oscManagerWidth, panelRect.getHeight());
    
    tTouchOsc->setPosition(rectangle.x + 5, rectangle.y + 15);
    tTouchOsc->setSize(100, 14);
    
    map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getCollapsed() || getCollapsed())
        {
            it->second->tAddress->setRectangle(0, 0, 0, 0);
            it->second->tSend->setRectangle(0, 0, 0, 0);
        }
        else
        {
            ofRectangle e;
            if (it->second->element->isMultiElement() &&
                ((GuiMultiElement *) it->second->element)->getElements().size() > 0) {
                
                e = ((GuiMultiElement *) it->second->element)->getElements()[0]->getRectangle();
            }
            else {
                e = it->second->element->getRectangle();
            }
            it->second->tAddress->setPosition(rectangle.x, e.y);
            it->second->tAddress->setSize(oscManagerWidth - 20, e.height);
            it->second->tSend->setPosition(rectangle.x + oscManagerWidth - 16, e.y);
            it->second->tSend->setSize(12, e.height);
        }
    }
}

void OscManagerPanel::update()
{
    tTouchOsc->update();
    map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        it->second->tAddress->update();
        it->second->tSend->update();
    }
}

void OscManagerPanel::draw()
{
    ofSetColor(colorBackground, 50);
    ofFill();
    ofRect(rectangle);
    
    tTouchOsc->draw();

    map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            it->second->tAddress->draw();
            it->second->tSend->draw();
        }
    }
}

bool OscManagerPanel::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    
    tTouchOsc->mouseMoved(mouseX, mouseY);
    
    map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            it->second->tAddress->mouseMoved(mouseX, mouseY);
            it->second->tSend->mouseMoved(mouseX, mouseY);
        }
    }
    return mouseOver;
}

bool OscManagerPanel::mousePressed(int mouseX, int mouseY)
{
    if (tTouchOsc->mousePressed(mouseX, mouseY)) return true;
    
    map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            if      (it->second->tAddress->mousePressed(mouseX, mouseY)) return true;
            else if (it->second->tSend->mousePressed(mouseX, mouseY)) return true;
        }
    }
    
    return false;
}

bool OscManagerPanel::mouseDragged(int mouseX, int mouseY)
{
    if (tTouchOsc->mouseDragged(mouseX, mouseY)) return true;

    map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            if      (it->second->tAddress->mouseDragged(mouseX, mouseY)) return true;
            else if (it->second->tSend->mouseDragged(mouseX, mouseY)) return true;
        }
    }
    return false;
}

bool OscManagerPanel::mouseReleased(int mouseX, int mouseY)
{
    if (tTouchOsc->mouseReleased(mouseX, mouseY)) return true;

    map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            if      (it->second->tAddress->mouseReleased(mouseX, mouseY)) return true;
            else if (it->second->tSend->mouseReleased(mouseX, mouseY)) return true;
        }
    }
    return false;
}

bool OscManagerPanel::keyPressed(int key)
{
    if (mouseOver)
    {
        if (tTouchOsc->keyPressed(key)) return true;

        map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
        for (; it != groups.end(); ++it)
        {
            if (it->second->element->getActive())
            {
                if      (it->second->tAddress->keyPressed(key)) return true;
                else if (it->second->tSend->keyPressed(key)) return true;
            }
        }
    }
    return false;
}

