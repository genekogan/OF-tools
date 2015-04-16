#include "GuiMenu.h"


GuiMenu::MenuElement::MenuElement(GuiToggle *toggle, Parameter<bool> *parameter, int index)
{
    this->toggle = toggle;
    this->parameter = parameter;
    this->index = index;
}

GuiMenu::GuiMenu(string name, vector<string> choices, bool multipleChoice, bool autoClose) : GuiMultiElement(name)
{
    setupMenu(name, choices, multipleChoice, autoClose);
}

GuiMenu::GuiMenu(string name, bool multipleChoice, bool autoClose) : GuiMultiElement(name)
{
    vector<string> choices;
    setupMenu(name, choices, multipleChoice, autoClose);
}

GuiMenu::~GuiMenu()
{
    clearToggles();
    ///
    //
    //
    //
    //
    //
}

void GuiMenu::setupMenu(string name, vector<string> & choices, bool multipleChoice, bool autoClose)
{
    marginY = GUI_DEFAULT_MARGIN_Y_INNER;
    setName(name);
    setMultipleChoice(multipleChoice);
    setAutoClose(autoClose);
    setCollapsible(true);
    setCollapsed(false);
    for (auto choice : choices) {
        addToggle(choice);
    }
}

GuiToggle * GuiMenu::addToggle(Parameter<bool> *parameter)
{
    int idx = 2;
    string newToggleName = parameter->getName();
    while (menuElements.count(newToggleName) != 0) {
        newToggleName = parameter->getName()+"("+ofToString(idx++)+")";
    }
    parameter->setName(newToggleName);
    
    if (menuElements.count(parameter->getName()) == 0)
    {
        parameters.push_back(parameter);
        GuiToggle *toggle = new GuiToggle(parameter);
        menuElements[parameter->getName()] = new MenuElement(toggle, parameter, elements.size());
        initializeElement(toggle);
        ofAddListener(toggle->buttonEvent, this, &GuiMenu::buttonChanged);
        return toggle;
    }
    else
    {
        ofLog(OF_LOG_ERROR, "Note: toggle "+parameter->getName()+" already exists");
        delete parameter;
        return NULL;
    }
}

GuiToggle * GuiMenu::addToggle(string name, bool *value)
{
    return addToggle(new Parameter<bool>(name, value));
}

GuiToggle * GuiMenu::addToggle(string name)
{
    return addToggle(new Parameter<bool>(name, new bool(false)));
}

void GuiMenu::setToggle(string toggleName, bool value)
{
    if (!multipleChoice)
    {
        for (auto e : elements) {
            ((GuiToggle *) e)->setValue(e->getName() == toggleName, false);
        }
    }
    if (autoClose)
    {
        setCollapsed(true);
        setHeader(toggleName);
    }
    if (menuElements.count(toggleName))
    {
        GuiMenuEventArgs evt(menuElements[toggleName]->toggle, menuElements[toggleName]->index, value);
        ofNotifyEvent(menuEvent, evt, this);
    }
}

bool GuiMenu::getToggle(string toggleName)
{
    if (menuElements.count(toggleName) == 0) {
        return menuElements[toggleName]->toggle->getValue();
    }
    else
    {
        ofLog(OF_LOG_ERROR, "Error: no toggle named "+toggleName);
        return false;
    }
}

void GuiMenu::removeToggle(string toggleName)
{
    if (menuElements.count(toggleName) != 0)
    {
        vector<GuiElement*>::iterator it = elements.begin();
        vector<Parameter<bool>*>::iterator itp = parameters.begin();
        while (itp != parameters.end())
        {
            if ((*itp)->getName() == toggleName) {
                parameters.erase(itp);
            }
            else {
                ++itp;
            }
        }
        while (it != elements.end())
        {
            if ((*it)->getName() == toggleName) {
                elements.erase(it);
            }
            else {
                ++it;
            }
        }
        menuElements.erase(toggleName);
        resetIndices();
        resetGuiPositions();
    }
}

void GuiMenu::clearToggles()
{
    vector<Parameter<bool>*>::iterator itp = parameters.begin();
    map<string, MenuElement*>::iterator itm = menuElements.begin();
    while (itm != menuElements.end())
    {
        
        
        //
        //
        // delete leads to runtime error....
        //
        //
        
        
        //delete itm->second;
        
        
        menuElements.erase(itm++);
    }
    while (itp != parameters.end())
    {
        delete *itp;
        parameters.erase(itp);
    }
    
    
    vector<GuiElement*>::iterator ite = elements.begin();
    while (ite != elements.end())
    {
        //delete *ite;
        elements.erase(ite);
    }
    elements.clear();
    
    
    
    parameters.clear();
    menuElements.clear();
    
    resetGuiPositions();
}

void GuiMenu::resetIndices()
{
    int index = 0;
    for (auto e : elements) {
        menuElements[e->getName()]->index = index++;
    }
}

void GuiMenu::buttonChanged(GuiButtonEventArgs &e)
{
    setToggle(e.button->getName(), e.value);
}

void GuiMenu::setAutoClose(bool autoClose)
{
    this->autoClose = autoClose;
    if (!autoClose && !multipleChoice) {
        setHeader(getName());
    }
}

void GuiMenu::getParameters(vector<ParameterBase*> & parameters_)
{
    for (auto p : parameters) {
        parameters_.push_back(p);
    }
}

void GuiMenu::updateParameterOscAddress()
{
    int idx = 0;
    for (auto p : parameters) {
        p->setOscAddress(getAddress()+"/"+ofToString(idx++));
    }
}

