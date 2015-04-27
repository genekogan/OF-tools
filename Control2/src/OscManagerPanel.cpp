#include "OscManagerPanel.h"



OscManagerPanel::ElementOsc::ElementOsc(GuiElement *element, OscManager *osc)
{
    this->element = element;
    this->osc = osc;
    address = element->getAddress();
    ofStringReplace(address, " ", "_");
    tAddress = new GuiTextBox("address", new string(address), this, &ElementOsc::eventSetAddress);
    tAddress->setAutoUpdate(false);
    tAddress->setAutoDraw(false);
    tSend = new GuiToggle("s", &sending, this, &ElementOsc::eventSetSending);
    tSend->setAutoUpdate(false);
    tSend->setAutoDraw(false);
}

void OscManagerPanel::ElementOsc::updateAddress()
{
    vector<ParameterBase*> parameters;
    element->getParameters(parameters);
    for (auto p : parameters) {
        p->setOscAddress(address);
    }
}

void OscManagerPanel::ElementOsc::eventSetAddress(GuiTextBoxEventArgs &evt)
{
    address = evt.textBox->getValue();
    ofStringReplace(address, " ", "_");
    tAddress->setValue(address, false);
    if (sending) {
        updateAddress();
    }
}

void OscManagerPanel::ElementOsc::eventSetSending(GuiButtonEventArgs &evt)
{
    updateAddress();
    if (sending) {
        osc->addElementToSender(element);
    }
    else {
        osc->removeElementFromSender(element);
    }
    
}

OscManagerPanel::ElementOscGroup::ElementOscGroup(GuiElement *element, OscManager *osc)
{
    this->element = element;
    this->osc = osc;
    
    if (element->isMultiElement())
    {
        for (auto e : ((GuiMultiElement *) element)->getElements()) {
            elements.push_back(new ElementOsc(e, osc));
        }
    }
    else {
        elements.push_back(new ElementOsc(element, osc));
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
        else
        {
            vector<ElementOsc*>::iterator ite = it->second->elements.begin();
            while (ite != it->second->elements.end())
            {
                if ((*ite)->element == element)
                {
                    delete *ite;
                    it->second->elements.erase(ite);
                    setupGuiPositions();
                }
                else
                {
                    ++ite;
                }
            }
            ++it;
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
            for (auto e : it->second->elements)
            {
                e->tAddress->setRectangle(ofRectangle(0, 0, 0, 0));
                e->tSend->setRectangle(ofRectangle(0, 0, 0, 0));
            }
        }
        else
        {
            for (auto e : it->second->elements)
            {
                e->tAddress->setPosition(rectangle.x, e->element->getRectangle().y);
                e->tAddress->setSize(oscManagerWidth - 20, e->element->getRectangle().height);
                e->tSend->setPosition(rectangle.x + oscManagerWidth - 16, e->element->getRectangle().y);
                e->tSend->setSize(12, e->element->getRectangle().height);
            }
        }
    }
}

void OscManagerPanel::eventMakeTouchOscLayout(GuiButtonEventArgs &evt)
{
    panel->makeTouchOscLayout("OSC "+panel->getName());
}

void OscManagerPanel::update()
{
    tTouchOsc->update();
    map<GuiElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto e : it->second->elements)
        {
            e->tAddress->update();
            e->tSend->update();
        }
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
            for (auto e : it->second->elements)
            {
                e->tAddress->draw();
                e->tSend->draw();
            }
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
            for (auto e : it->second->elements)
            {
                e->tAddress->mouseMoved(mouseX, mouseY);
                e->tSend->mouseMoved(mouseX, mouseY);
            }
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
            for (auto e : it->second->elements)
            {
                if      (e->tAddress->mousePressed(mouseX, mouseY)) return true;
                else if (e->tSend->mousePressed(mouseX, mouseY)) return true;
            }
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
            for (auto e : it->second->elements)
            {
                if      (e->tAddress->mouseDragged(mouseX, mouseY)) return true;
                else if (e->tSend->mouseDragged(mouseX, mouseY)) return true;
            }
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
            for (auto e : it->second->elements)
            {
                if      (e->tAddress->mouseReleased(mouseX, mouseY)) return true;
                else if (e->tSend->mouseReleased(mouseX, mouseY)) return true;
            }
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
                for (auto e : it->second->elements)
                {
                    if      (e->tAddress->keyPressed(key)) return true;
                    else if (e->tSend->keyPressed(key)) return true;
                }
            }
        }
    }
    return false;
}

