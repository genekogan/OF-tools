#include "OscManager.h"

/*
OscManager::ParameterOscPair::ParameterOscPairEventArgs::ParameterOscPairEventArgs(ParameterBase *parameter, bool sender, bool toAdd)
{
    this->parameter = parameter;
    this->sender = sender;
    this->toAdd = toAdd;
}

OscManager::ParameterOscPair::ParameterOscPair(ParameterBase *parameter)
{
    this->parameter = parameter;
    tOscIn = new GuiToggle("in", this, &ParameterOscPair::toggleOscIn);
    tOscOut = new GuiToggle("out", this, &ParameterOscPair::toggleOscOut);
    tOscAddress = new GuiTextBox(parameter->getOscAddress(), this, &ParameterOscPair::editOscAddress);
    tOscIn->setAutoUpdate(false);
    tOscIn->setAutoDraw(false);
    tOscOut->setAutoUpdate(false);
    tOscOut->setAutoDraw(false);
    tOscAddress->setAutoUpdate(false);
    tOscAddress->setAutoDraw(false);
}

OscManager::ParameterOscPair::~ParameterOscPair()
{
    delete tOscIn;
    delete tOscOut;
    delete tOscAddress;
}

void OscManager::ParameterOscPair::toggleOscIn(GuiButtonEventArgs & e)
{
    ParameterOscPairEventArgs args(parameter, false, e.value > 0.5);
    ofNotifyEvent(parameterOscEvent, args, this);
}

void OscManager::ParameterOscPair::toggleOscOut(GuiButtonEventArgs & e)
{
    ParameterOscPairEventArgs args(parameter, true, e.value > 0.5);
    ofNotifyEvent(parameterOscEvent, args, this);
}

void OscManager::ParameterOscPair::editOscAddress(GuiTextBoxEventArgs & e)
{
    parameter->setOscAddress(e.value);
    tOscAddress->setValue(parameter->getOscAddress());
}

OscManager::OscPanel::OscPanel(string name) : GuiWidget(name)
{
    allIn = new GuiToggle("receive all", this, &OscPanel::toggleReceiveAll);
    allOut = new GuiToggle("send all", this, &OscPanel::toggleSendAll);
}

OscManager::OscPanel::~OscPanel()
{
    delete allIn;
    delete allOut;
}

void OscManager::OscPanel::toggleReceiveAll(GuiButtonEventArgs & e)
{
    for (auto p : parameters) {
        p->getToggleIn()->setValue(e.value > 0.5, true);
    }
}

void OscManager::OscPanel::toggleSendAll(GuiButtonEventArgs & e)
{
    for (auto p : parameters) {
        p->getToggleOut()->setValue(e.value > 0.5, true);
    }
}

void OscManager::OscPanel::setupGuiPositions()
{
    ofPoint topLeft = ofPoint(rectangle.x, rectangle.y);
    if (getCollapsible())
    {
        headerRectangle.set(rectangle.x, rectangle.y, rectangle.width, headerHeight);
        topLeft.y += headerHeight;
    }
    
    if (getCollapsed() || parameters.size() < 2)
    {
        allIn->setRectangle(0, 0, 0, 0);
        allOut->setRectangle(0, 0, 0, 0);
    }
    else
    {
        
        topLeft.y += marginY;
        allIn->setRectangle(topLeft.x + 4, topLeft.y, getWidth(), getHeight());
        allOut->setRectangle(topLeft.x + getWidth() + 8, topLeft.y, getWidth(), getHeight());
        topLeft.y += getHeight();
        topLeft.y += marginY;
    }
    
    for (auto p : parameters)
    {
        if (getCollapsed())
        {
            p->getToggleIn()->setRectangle(0, 0, 0, 0);
            p->getToggleOut()->setRectangle(0, 0, 0, 0);
            p->getToggleAddress()->setRectangle(0, 0, 0, 0);
        }
        else
        {
            p->getToggleIn()->setRectangle(topLeft.x + 4, topLeft.y, 36, getHeight());
            p->getToggleOut()->setRectangle(topLeft.x + 44, topLeft.y, 36, getHeight());
            p->getToggleAddress()->setRectangle(topLeft.x + 84, topLeft.y, rectangle.width - 8, getHeight());
            topLeft.y += p->getToggleIn()->getRectangle().getHeight() + 4;
        }
        if (!collapsed) {
            topLeft.y += marginY;
        }
    }
    rectangle.height = topLeft.y - rectangle.y;
}


void OscManager::OscPanel::addParameter(ParameterBase *parameter)
{
    for (auto p : parameters)
    {
        if (p->getParameter() == parameter)
        {
            ofLog(OF_LOG_NOTICE, "Parameter "+parameter->getName()+" already registered in osc manager");
            return;
        }
    }
    
    ParameterOscPair *pair = new ParameterOscPair(parameter);
    parameters.push_back(pair);
    ofAddListener(pair->parameterOscEvent, (OscManager *) parent, &OscManager::eventParameterOscAction);
    
 
//    GuiElementGroup *elementGroup = new GuiElementGroup(parameter->getName());
//    elementGroup->addElement(pair->getToggleIn());
//    elementGroup->addElement(pair->getToggleOut());
//    elementGroup->addElement(pair->getToggleAddress());
//    setupElementGroup(elementGroup);
 
}

OscManager::OscManager() : GuiWidget()
{
    setName("OSC Manager");
    marginX = GUI_DEFAULT_PANEL_MARGIN_OUTER_X;
    setSize(GUI_DEFAULT_OSCMANAGER_WIDTH, rectangle.height);
    individualParameters = addOscGroup("all parameters");
}

OscManager::~OscManager()
{
    
}

void OscManager::setupSender(string host, int portIn)
{
    this->portIn = portIn;
    send.setup(host, portIn);
}

void OscManager::setupReceiver(int portOut)
{
    this->portOut = portOut;
    receive.setup(portOut);
}

OscManager::OscPanel * OscManager::addOscGroup(string name)
{
    OscPanel *oscGroup = new OscPanel(name);
    addOscGroup(oscGroup);
    return oscGroup;
}

void OscManager::addOscGroup(OscPanel *oscGroup)
{
    //GuiElementGroup *elementGroup = new GuiElementGroup(oscGroup->getName());
//    oscGroup->setAddress(getAddress()+oscGroup->getAddress());
    oscGroup->setParent(this);
//    elementGroup->addElement(oscGroup);
//    setupElementGroup(elementGroup);
//    ofAddListener(oscGroup->widgetChanged, this, &OscManager::eventWidgetChanged);
}

void OscManager::addParameters(string groupName, vector<ParameterBase*> parameters)
{
    OscPanel *oscGroup = addOscGroup(groupName);
    for (auto p : parameters) {
        oscGroup->addParameter(p);
    }
}

void OscManager::addParameter(ParameterBase *parameter)
{
    individualParameters->addParameter(parameter);
}

void OscManager::addParameterToSender(ParameterBase *parameter)
{
    for (auto p : sendParameters)
    {
        if (p == parameter)
        {
            ofLog(OF_LOG_NOTICE, "Error: parameter "+parameter->getName()+" already added to osc sender");
            return;
        }
    }
    sendParameters.push_back(parameter);
    ofLog(OF_LOG_NOTICE, "Parameter "+parameter->getName()+" added to osc sender");
}

void OscManager::addParameterToReceiver(ParameterBase *parameter)
{
    if (receiveParameters.count(parameter->getOscAddress()) == 0)
    {
        receiveParameters[parameter->getOscAddress()] = parameter;
        ofLog(OF_LOG_NOTICE, "Parameter "+parameter->getName()+" added to osc receiver");
    }
    else {
        ofLog(OF_LOG_NOTICE, "Notice: parameter "+parameter->getName()+" already added to osc receiver");
    }
}

void OscManager::removeParameterFromSender(ParameterBase *parameter)
{
    vector<ParameterBase*>::iterator it = sendParameters.begin();
    while (it != sendParameters.end())
    {
        if (*it == parameter)
        {
            sendParameters.erase(it);
            ofLog(OF_LOG_NOTICE, "Parameter "+parameter->getName()+" removed from osc sender");
        }
        else {
            ++it;
        }
    }
}

void OscManager::removeParameterFromReceiver(ParameterBase *parameter)
{
    if (receiveParameters.count(parameter->getOscAddress()) != 0)
    {
        receiveParameters.erase(parameter->getOscAddress());
        ofLog(OF_LOG_NOTICE, "Parameter "+parameter->getName()+" removed from osc receiver");
    }
}

void OscManager::update()
{
    GuiWidget::update();
    
    // receive parameters
    while (receive.hasWaitingMessages())
    {
        ofxOscMessage msg;
        receive.getNextMessage(&msg);
        if (receiveParameters.count(msg.getAddress())) {
            receiveParameters[msg.getAddress()]->receiveOsc(msg);
        }
    }
    
    // send parameters
    for (auto p : sendParameters)
    {
        if (p->valueChanged())
        {
            ofxOscMessage m;
            p->sendOsc(m);
            send.sendMessage(m);
        }
    }
}

void OscManager::eventParameterOscAction(ParameterOscPair::ParameterOscPairEventArgs & p)
{
    if (p.sender) {
        p.toAdd ? addParameterToSender(p.parameter) : removeParameterFromSender(p.parameter);
    }
    else {
        p.toAdd ? addParameterToReceiver(p.parameter) : removeParameterFromReceiver(p.parameter);
    }
}

void OscManager::eventWidgetChanged(string & s)
{
    setupGuiPositions();
}
*/