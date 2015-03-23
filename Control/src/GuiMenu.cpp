#include "GuiMenu.h"


GuiMenu::GuiMenu(string name, vector<string> choices, bool multipleChoice, bool autoClose) : GuiWidgetBase(name)
{
    setupMenu(multipleChoice, autoClose);
    initializeToggles(choices);
}

GuiMenu::GuiMenu(string name, bool multipleChoice, bool autoClose) : GuiWidgetBase(name)
{
    setupMenu(multipleChoice, autoClose);
}

GuiMenu::~GuiMenu()
{
    toggles.clear();
}

void GuiMenu::setupMenu(bool multipleChoice, bool autoClose)
{
    this->multipleChoice = multipleChoice;
    this->autoClose = autoClose;
    marginOuterX = 2;
    marginOuterY = 2;
    marginInner = 5;
    menuGroup = new GuiElementGroup(name);
    setupElementGroup(menuGroup);
}

void GuiMenu::initializeToggles(vector<string> & choices)
{
    for (auto choice : choices) {
        addToggle(choice);
    }
}

void GuiMenu::setToggle(string toggleName, bool value)
{
    if (toggles.count(toggleName) > 0)
    {
        if (toggles[toggleName]->getValue() != value)
        {
            toggles[toggleName]->setValue(value);
            GuiElementEventArgs args(toggleName, toggles[toggleName]->getMenuIndex(), value ? 1.0 : 0.0);
            updateParameter(args);
        }
    }
    else {
        ofLog(OF_LOG_ERROR, "Warning: no toggle named "+toggleName+" found");
    }
}

bool GuiMenu::getToggle(string toggleName)
{
    if (toggles.count(toggleName) > 0) {
        return toggles[toggleName]->getValue();
    }
    else {
        ofLog(OF_LOG_ERROR, "Warning: no toggle named "+toggleName+" found");
        return false;
    }
}

void GuiMenu::addToggle(string choice, bool *value)
{
    Parameter<bool> *parameter = new Parameter<bool>(choice, value);
    parameter->setOscAddress(getAddress()+parameter->getOscAddress());
    GuiToggle *toggle = new GuiToggle(parameter);
    toggle->setMenuIndex(toggles.size());
    toggle->setParent(this);
    toggle->setAutoUpdate(false);
    toggle->setAutoDraw(false);
    toggles[choice] = toggle;
    menuGroup->addElement(toggle);
    parameters.push_back(parameter);
    ofAddListener(toggle->elementEvent, this, &GuiMenu::updateParameter);
    setList(true);
}

void GuiMenu::addToggle(string choice)
{
    addToggle(choice, new bool());
}

void GuiMenu::removeToggle(string toggleName)
{
    if (toggles.count(toggleName) > 0)
    {
        ofRemoveListener(toggles[toggleName]->elementEvent, this, &GuiMenu::updateParameter);
        toggles.erase(toggleName);
    }
    menuGroup->removeElement(toggleName);

    int menuIndex = 0;
    for (auto e : menuGroup->getElements()) {
        ((GuiToggle *) e)->setMenuIndex(++menuIndex);
    }
    
    // have to remove the parameter as well?
}

void GuiMenu::clearToggles()
{
    map<string, GuiToggle*>::iterator it = toggles.begin();
    for (; it != toggles.end(); ++it) {
        ofRemoveListener(it->second->elementEvent, this, &GuiMenu::updateParameter);
    }
    toggles.clear();
    menuGroup->clearToggles();
}

void GuiMenu::updateParameter(GuiElementEventArgs & button)
{
    for (auto e : elementGroups[0]->getElements())
    {
        if (!multipleChoice) {
            ((GuiToggle *) e)->setValue(e->getName() == button.name, false);
        }
    }
    GuiElementEventArgs args(button.name, button.cell, multipleChoice ? (button.value ? 1.0 : 0.0) : 1.0);
    ofNotifyEvent(elementEvent, args, this);
    if (autoClose)
    {
        header = button.name;
        setCollapsed(!collapsed);
    }
}
