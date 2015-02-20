#include "GuiMenu.h"


GuiMenu::GuiMenu(string name, vector<string> choices, bool multipleChoice, bool autoClose) : GuiWidget(name)
{
    this->multipleChoice = multipleChoice;
    this->autoClose = autoClose;
    setupMenu(choices);
}

void GuiMenu::setupMenu(vector<string> & choices)
{
    for (auto choice : choices)
    {
        GuiToggle *toggle = new GuiToggle(choice, new bool());
        toggle->setAutoUpdate(false);
        toggle->setAutoDraw(false);
        toggles[choice] = toggle;
        elements.push_back(toggle);
        ofAddListener(toggle->elementEvent, this, &GuiMenu::updateParameter);
    }
    isList = true;
    collapsed = false;
    setupGuiComponents();
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
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
    for (auto e : elements)
    {
        if (!multipleChoice) {
            ((GuiToggle *) e)->setValue(e->getName() == button.name, false);
        }
    }
    GuiElementEventArgs args(button.name, 0, multipleChoice ? (button.value ? 1.0 : 0.0) : 1.0);
    ofNotifyEvent(elementEvent, args, this);
    if (autoClose)
    {
        collapsed = !collapsed;
        header = button.name;
        setupGuiComponents();
        ofNotifyEvent(widgetChanged, name, this);
    }
}

GuiMenu::~GuiMenu()
{
    toggles.clear();
}