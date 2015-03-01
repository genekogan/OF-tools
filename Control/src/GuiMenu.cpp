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
    menuGroup = new GuiElementGroup();
    setupElementGroup(menuGroup);
}

void GuiMenu::initializeToggles(vector<string> & choices)
{
    for (auto choice : choices)
    {
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
            GuiElementEventArgs args(toggleName, 0, value ? 1.0 : 0.0);
            updateParameter(args);
        }
    }
    else {
        ofLog(OF_LOG_ERROR, "Warning: no toggle named "+toggleName+" found");
    }
}

void GuiMenu::addToggle(string choice, bool *value)
{
    GuiToggle *toggle = new GuiToggle(choice, value);
    toggle->setParent(this);
    toggle->setAutoUpdate(false);
    toggle->setAutoDraw(false);
    toggles[choice] = toggle;
    menuGroup->addElement(toggle);
    ofAddListener(toggle->elementEvent, this, &GuiMenu::updateParameter);
    setList(true);
}

void GuiMenu::addToggle(string choice)
{
    addToggle(choice, new bool());
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

void GuiMenu::updateParameter(GuiElementEventArgs & button)
{
    for (auto e : elementGroups[0]->getElements())
    {
        if (!multipleChoice) {
            ((GuiToggle *) e)->setValue(e->getName() == button.name, false);
        }
    }
    GuiElementEventArgs args(button.name, 0, multipleChoice ? (button.value ? 1.0 : 0.0) : 1.0);
    ofNotifyEvent(elementEvent, args, this);
    if (autoClose)
    {
        header = button.name;
        setCollapsed(!collapsed);
    }
}
