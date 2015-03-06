#include "GuiWidgetBase.h"


GuiElementGroup::~GuiElementGroup()
{
    for (auto e : elements) {
        //delete e;  //?
    }
    elements.clear();
}

void GuiElementGroup::addElement(GuiElement * element)
{
    elements.push_back(element);
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
    setupGuiComponents();
}

vector<GuiElementGroup*> & GuiWidgetBase::getElementGroups()
{
    return elementGroups;
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
        vector<GuiElement*>::iterator ite = (*it)->getElements().begin();
        while (ite != (*it)->getElements().end())
        {
            
            
            //////////////
            ///////
            // TODO: messy and doesn't work right for widgets inside widgets
            
            
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
    setupGuiComponents();
    ofNotifyEvent(widgetChanged, name, this);
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
    setupGuiComponents();
    ofNotifyEvent(widgetChanged, name, this);
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

void GuiWidgetBase::setupGuiComponents()
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
        if (list && headerRectangle.inside(mouseX, mouseY))
        {
            setCollapsed(!collapsed);
            return true;
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
    }
    return false;
}

bool GuiWidgetBase::keyPressed(int key)
{
    GuiElement::keyPressed(key);
    if (mouseOver)
    {
        if (key == OF_KEY_UP) {
            scrollFocus(-1);
            return true;
        }
        else if (key == OF_KEY_DOWN) {
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
