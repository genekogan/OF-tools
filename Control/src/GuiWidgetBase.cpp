#include "GuiWidgetBase.h"


GuiElementGroup::~GuiElementGroup()
{
    for (auto e : elements) {
        // TODO: something's going wrong here
        //delete e;
    }
    elements.clear();
    
    
    // clearToggles()  ?????
}

void GuiElementGroup::addElement(GuiElement * element)
{
    elements.push_back(element);
}

void GuiElementGroup::removeElement(string name)
{
    vector<GuiElement*>::iterator it = elements.begin();
    while (it != elements.end())
    {
        if ((*it)->getName() == name) {
            delete *it;
            elements.erase(it);
        }
        else {
            ++it;
        }
    }
}

void GuiElementGroup::clearToggles()
{
    for (auto e : elements) {
        // TODO: something's going wrong here???
        delete e;
    }
    elements.clear();
}

GuiWidgetBase::GuiWidgetBase() : GuiElement()
{
    initialize();
}

GuiWidgetBase::GuiWidgetBase(string name) : GuiElement(name)
{
    initialize();
    setName(name);
}

GuiWidgetBase::~GuiWidgetBase()
{
    for (auto g : elementGroups) {
        delete g;
    }
    elementGroups.clear();
}

void GuiWidgetBase::initialize()
{
    headerColor = GUI_DEFAULT_HEADER_COLOR;
    headerHeight = GUI_DEFAULT_HEADER_HEIGHT;
    controllerHeight = GUI_DEFAULT_CONTROLLER_HEIGHT;
    marginOuterX = GUI_DEFAULT_MARGIN_OUTER_X;
    marginOuterY = GUI_DEFAULT_MARGIN_OUTER_Y;
    marginInner = GUI_DEFAULT_MARGIN_INNER;
    
    parent = NULL;
    
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
}

void GuiWidgetBase::setParent(GuiWidgetBase *parent)
{
    this->parent = parent;
    hasParent = true;
}

void GuiWidgetBase::setName(string name)
{
    GuiElement::setName(name);
    header = name;
    address = "/"+name;
    setupGuiPositions();
}

void GuiWidgetBase::setAddress(string address)
{
    this->address = address;
}

vector<GuiElementGroup*> & GuiWidgetBase::getElementGroups()
{
    return elementGroups;
}

vector<ParameterBase*> GuiWidgetBase::getParameters()
{
    vector<ParameterBase*> allParameters;
    for (auto p : parameters) {
        allParameters.push_back(p);
    }
    for (auto group : elementGroups)
    {
        for (auto &e : group->getElements())
        {
            if (e->isWidget())
            {
                vector<ParameterBase*> widgetParameters = ((GuiWidgetBase *) e)->getParameters();
                for (auto p : widgetParameters) {
                    allParameters.push_back(p);
                }
            }
        }
    }
    return allParameters;
}

void GuiWidgetBase::getAllElementGroups(vector<GuiElementGroup*> & allGroups)
{
    for (auto group : elementGroups)
    {
        GuiElementGroup * newGroup = new GuiElementGroup();
        vector<GuiElement*> & elements = group->getElements();
        for (auto &element : elements)
        {
            if (element->isWidget()) {
                ((GuiWidgetBase *) element)->getAllElementGroups(allGroups);
            }
            else {
                newGroup->addElement(element);
            }
        }
        if (newGroup->getElements().size() > 0) {
            allGroups.push_back(newGroup);
        }
    }
}

void GuiWidgetBase::getAllElements(vector<GuiElement*> & allElements)
{
    for (auto group : elementGroups)
    {
        for (auto &element : group->getElements())
        {
            if (element->isWidget()) {
                ((GuiWidgetBase *) element)->getAllElements(allElements);
            }
            else {
                allElements.push_back(element);
            }
        }
    }
}

GuiElement * GuiWidgetBase::getElement(string name)
{
    for (auto group : elementGroups)
    {
        for (auto &e : group->getElements())
        {
            if (e->getName() == name) {
                return e;
            }
        }
    }
    ofLog(OF_LOG_ERROR, "No element found in "+getName()+" called "+name);
}










void GuiWidgetBase::removeElement(string name)
{
    vector<GuiElementGroup*>::iterator it = elementGroups.begin();
    while (it != elementGroups.end())
    {
        if ((*it)->getName() == name)
        {         
            vector<GuiElement*>::iterator ite = (*it)->getElements().begin();
            while (ite != (*it)->getElements().end())
            {
                ofNotifyEvent(elementDeletedEvent, (*ite), this);
                cout << "delete " << (*ite)->getName() << endl;
                delete *ite;
                (*it)->getElements().erase(ite);
            }
            elementGroups.erase(it);
            
        }
        
        else
        {
            vector<GuiElement*>::iterator ite = (*it)->getElements().begin();
            while (ite != (*it)->getElements().end())
            {
                
                
                //////////////
                ///////
                // TODO: messy and doesn't work right for widgets inside widgets
                
                
                
                // ALSO need to delete the associated parameter from parameters
                
                
                // ALSO -- if removing a widget -- need to clear innerWidgets in GuiPanel
                
                
                if ((*ite)->getName() == name)
                {
                    ofNotifyEvent(elementDeletedEvent, (*ite), this);
                    delete *ite;
                    (*it)->getElements().erase(ite);
                }
                else {
                    ++ite;
                }
            }
            ++it;
        }
        
    }
    setupGuiPositions();
    ofNotifyEvent(widgetChanged, name, this);
}

void GuiWidgetBase::clearElements()
{
    vector<GuiElementGroup*>::iterator it = elementGroups.begin();
    while (it != elementGroups.end())
    {
        vector<GuiElement*>::iterator ite = (*it)->getElements().begin();
        while (ite != (*it)->getElements().end())
        {
            
            
            //////////////
            ///////
            // TODO: messy and doesn't work right for widgets inside widgets
            
            
            
            // ALSO need to delete the associated parameter from parameters
            
            
            // ALSO -- if removing a widget -- need to clear innerWidgets in GuiPanel
            
            ofNotifyEvent(elementDeletedEvent, (*ite), this);
            delete *ite;
            (*it)->getElements().erase(ite);
        }
        delete *it;
        elementGroups.erase(it);
    }
    setupGuiPositions();
    ofNotifyEvent(widgetChanged, name, this);
}



















void GuiWidgetBase::savePreset(string path)
{
    ofXml xml;
    xml.addChild("Preset");
    xml.setTo("Preset");
    getXml(xml);
    xml.save(path);
}

void GuiWidgetBase::loadPreset(string path)
{
    ofXml xml;
    xml.load(path);
    xml.setTo("Preset");
    setFromXml(xml);
}

void GuiWidgetBase::getXml(ofXml &xml)
{
    vector<GuiElementGroup*> allGroups;
    getAllElementGroups(allGroups);
    
    ofXml xmlElements;
    xmlElements.addChild("Elements");
    xmlElements.setTo("Elements");
    for (auto group : allGroups)
    {
        for (auto &element : group->getElements())
        {
            ofXml xmlElement;
            xmlElement.addChild("Element");
            xmlElement.setTo("Element");
            element->getXml(xmlElement);
            xmlElements.addXml(xmlElement);
        }
    }
    xml.addXml(xmlElements);
}

void GuiWidgetBase::setFromXml(ofXml &xml)
{
    if (!xml.exists("Elements"))
    {
        ofLog(OF_LOG_ERROR, "No elements found in preset");
        return;
    }
    
    vector<GuiElement*> allElements;
    getAllElements(allElements);
    
    xml.setTo("Elements");
    if (xml.exists("Element[0]"))
    {
        xml.setTo("Element[0]");
        do {
            string name = xml.getValue<string>("Name");
            for (auto element : allElements)
            {
                if (element->getName() == name) {
                    element->setFromXml(xml);
                }
            }
        }
        while(xml.setToSibling());
        xml.setToParent();
    }
    xml.setToParent();
}

void GuiWidgetBase::setList(bool list)
{
    this->list = list;
    header = name;
    headerStringHeight = ofBitmapStringGetBoundingBox(header, 0, 0).height;
    setCollapsed(list ? collapsed : false);
}

void GuiWidgetBase::setCollapsed(bool collapsed)
{
    this->collapsed = list ? collapsed : false;
    setupGuiPositions();
    ofNotifyEvent(widgetChanged, name, this);
}

void GuiWidgetBase::setRectangle(ofRectangle rectangle)
{
    GuiElement::setRectangle(rectangle);
    for (auto w : boundWidgets)
    {
        w->setPosition(rectangle.x + rectangle.width + marginOuterX, rectangle.y);
    }
}

void GuiWidgetBase::setRectangle(int x, int y, int width, int height)
{
    setRectangle(ofRectangle(x, y, width, height));
}

void GuiWidgetBase::addBoundWidget(GuiWidgetBase *other)
{
    boundWidgets.push_back(other);
    other->setPosition(rectangle.x + rectangle.width + marginOuterX, rectangle.y);
}

void GuiWidgetBase::removeBoundWidget(GuiWidgetBase *other)
{
    vector<GuiWidgetBase*>::iterator it = boundWidgets.begin();
    while (it != boundWidgets.end()) {
        if (*it == other) {
            boundWidgets.erase(it);
        }
        else {
            ++it;
        }
    }
}

void GuiWidgetBase::setupElementGroup(GuiElementGroup *elementGroup)
{
    for (auto &e : elementGroup->getElements())
    {
        e->setParent(this);
        e->setAutoDraw(false);
        e->setAutoUpdate(false);
    }
    elementGroups.push_back(elementGroup);
    setList(elementGroups.size() > 1);
}

void GuiWidgetBase::setupGuiPositions()
{
    for (auto group : elementGroups)
    {
        for (auto &e : group->getElements()) {
            e->setSize(rectangle.width - 2 * marginOuterX, e->getElementHeight());
        }
    }
    
    ofPoint topLeft = ofPoint(rectangle.x, rectangle.y);
    if (list)
    {
        headerRectangle = ofRectangle(rectangle.x, rectangle.y, rectangle.width, headerHeight);
        topLeft.y += headerHeight;
    }
    
    if (!collapsed)
    {
        topLeft.y += controllerHeight;
        topLeft.y += marginOuterY;
    }
    
    for (auto group : elementGroups)
    {
        for (auto &e : group->getElements())
        {
            if (collapsed) {
                e->setRectangle(ofRectangle(0, 0, 0, 0));
            }
            else
            {
                e->setPosition(topLeft.x + marginOuterX, topLeft.y);
                topLeft.y += e->getRectangle().height + marginInner;
            }
        }
        if (!collapsed) {
            topLeft.y += marginOuterY - marginInner;
        }
    }
    
    rectangle.height = topLeft.y - rectangle.y;
}

void GuiWidgetBase::update()
{
    GuiElement::update();
    for (auto group : elementGroups)
    {
        for (auto &e : group->getElements()) {
            e->update();
        }
    }
}

void GuiWidgetBase::draw()
{
    ofPushStyle();
    
    ofSetLineWidth(1);
    ofSetCircleResolution(16);
    
    mouseOver ? ofSetColor(colorBackground, 110) : ofSetColor(colorBackground, 80);
    ofFill();
    ofRect(rectangle);
    
    if (list)
    {
        ofFill();
        ofSetColor(headerColor);
        ofRect(headerRectangle);
        if (headerActive)
        {
            ofSetColor(colorActive);
            ofNoFill();
            ofRect(headerRectangle);
        }
        ofSetColor(colorText);
        ofDrawBitmapString(header,
                           rectangle.x + 4,
                           rectangle.y + 1 + 0.5 * (headerHeight + 0.5 * headerStringHeight));
        ofDrawBitmapString(collapsed ? "+" : "-",
                           rectangle.x + rectangle.width - 16,
                           rectangle.y + 1 + 0.5 * (headerHeight + 0.5 * headerStringHeight));
    }
    
    if (!collapsed)
    {
        for (auto group : elementGroups)
        {
            for (auto &e : group->getElements()) {
                e->draw();
            }
        }
    }
    
    ofPopStyle();
}

bool GuiWidgetBase::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    mouseOver = rectangle.inside(mouseX, mouseY);
    headerActive = headerRectangle.inside(mouseX, mouseY);
    for (auto group : elementGroups)
    {
        for (auto &e : group->getElements()) {
            if (e->mouseMoved(mouseX, mouseY))  return true;
        }
    }
    return false;
}

bool GuiWidgetBase::mousePressed(int mouseX, int mouseY)
{
    if (GuiElement::mousePressed(mouseX, mouseY)) return true;
    if (mouseOver)
    {
        if (headerRectangle.inside(mouseX, mouseY))
        {
            pMouse.set(mouseX, mouseY);
            pPosition.set(getRectangle().x, getRectangle().y);
            return;
        }
        for (auto group : elementGroups)
        {
            for (auto &e : group->getElements()) {
                if (e->mousePressed(mouseX, mouseY)) return true;
            }
        }
    }
    return false;
}

bool GuiWidgetBase::mouseReleased(int mouseX, int mouseY)
{
    if (GuiElement::mouseReleased(mouseX, mouseY)) return true;
    if (mouseOver)
    {
        for (auto group : elementGroups)
        {
            if (list && headerRectangle.inside(mouseX, mouseY) && mouseX == pMouse.x && mouseY == pMouse.y)
            {
                setCollapsed(!collapsed);
                return true;
            }
            for (auto &e : group->getElements()) {
                if (e->mouseReleased(mouseX, mouseY)) return true;
            }
        }
    }
    return false;
}

bool GuiWidgetBase::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
    if (mouseOver)
    {
        for (auto group : elementGroups)
        {
            for (auto &e : group->getElements()) {
                if (e->mouseDragged(mouseX, mouseY)) return true;
            }
        }

        if (headerRectangle.inside(mouseX, mouseY) && parent == NULL)
        {
            setPosition(pPosition.x + mouseX - pMouse.x, pPosition.y + mouseY - pMouse.y);
            return true;
        }
    }
    return false;
}

bool GuiWidgetBase::keyPressed(int key)
{
    GuiElement::keyPressed(key);
    if (mouseOver)
    {
        if (key == OF_KEY_UP)
        {
            scrollFocus(-1);
            return true;
        }
        else if (key == OF_KEY_DOWN)
        {
            scrollFocus(1);
            return true;
        }
        else
        {
            for (auto group : elementGroups)
            {
                for (auto &e : group->getElements()) {
                    if (e->keyPressed(key)) return true;
                }
            }
        }
    }
    if (headerActive)
    {
        if (key == OF_KEY_RIGHT)
        {
            setCollapsed(false);
            return true;
        }
        else if (key == OF_KEY_LEFT)
        {
            setCollapsed(true);
            return true;
        }
        else if (key == ' ')
        {
            setCollapsed(!collapsed);
            return true;
        }
    }
    return false;
}

void GuiWidgetBase::scrollFocus(int scroll)
{
    vector<GuiElement*> allElements;
    getAllElements(allElements);
    if (allElements.size() == 0)  return;
    for (int i = 0; i < allElements.size(); i++)
    {
        if (allElements[i]->getMouseOver())
        {
            int next = (i + allElements.size() + scroll) % allElements.size();
            allElements[i]->setMouseOver(false);
            allElements[next]->setMouseOver(true);
            return;
        }
    }
    allElements[0]->setMouseOver(true);
}

void GuiWidgetBase::setHeaderColor(ofColor headerColor)
{
    this->headerColor = headerColor;
    ofNotifyEvent(widgetChanged, name, this);
}

void GuiWidgetBase::setHeaderHeight(int headerHeight)
{
    this->headerHeight = headerHeight;
    ofNotifyEvent(widgetChanged, name, this);
}

void GuiWidgetBase::setControllerHeight(int controllerHeight)
{
    this->controllerHeight = controllerHeight;
    ofNotifyEvent(widgetChanged, name, this);
}

void GuiWidgetBase::setMarginOuterX(int marginOuterX)
{
    this->marginOuterX = marginOuterX;
    ofNotifyEvent(widgetChanged, name, this);
}

void GuiWidgetBase::setMarginOuterY(int marginOuterY)
{
    this->marginOuterY = marginOuterY;
    ofNotifyEvent(widgetChanged, name, this);
}

void GuiWidgetBase::setMarginInner(int marginInner)
{
    this->marginInner = marginInner;
    ofNotifyEvent(widgetChanged, name, this);
}
