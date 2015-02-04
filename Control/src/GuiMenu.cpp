#include "GuiMenu.h"


GuiMenu::GuiMenu(string name, vector<string> choices) : GuiElement(name)
{
    setupMenu(choices);
}

void GuiMenu::setupMenu(vector<string> choices)
{
    for (auto c : choices)
    {
        Parameter<bool> *parameter = new Parameter<bool>(c, new bool());
        GuiToggle *toggle = new GuiToggle(c, parameter);
        parameters.push_back(parameter);
        toggles.push_back(toggle);
        ofAddListener(toggle->buttonEvent, this, &GuiMenu::updateParameter);
    }
    rectangle.set(0, 0, GUI_WIDTH, toggles.size() * (GUI_ELEMENT_HEIGHT + GUI_MARGIN_BETWEEN) + GUI_ELEMENT_HEIGHT);
    setupGuiComponents();
    setAutoUpdate(true);
    setAutoDraw(true);
    multipleChoice = false;
}

void GuiMenu::setAutoUpdate(bool autoUpdate)
{
    this->autoUpdate = autoUpdate;
    GuiElement::setAutoUpdate(autoUpdate);
    for (auto t : toggles) {
        t->setAutoUpdate(false);
    }
}

void GuiMenu::setAutoDraw(bool autoDraw)
{
    this->autoDraw = autoDraw;
    GuiElement::setAutoDraw(autoDraw);
    for (auto t : toggles) {
        t->setAutoDraw(false);
    }
}

void GuiMenu::updateParameter(GuiButtonEventArgs & button)
{
    for (auto t : toggles)
    {
        if (!multipleChoice)
        {
            t->setValue(t->getName() == button.name, false);
        }
    }
    GuiButtonEventArgs args(button.name, multipleChoice ? button.value : true);
    ofNotifyEvent(menuEvent, args, this);
}

void GuiMenu::setupGuiComponents()
{
    for (int i=0; i<toggles.size(); i++)
    {
        toggles[i]->setRectangle(rectangle.x,
                                 rectangle.y + i * (GUI_ELEMENT_HEIGHT + GUI_MARGIN_BETWEEN),
                                 GUI_WIDTH - 2*GUI_MARGIN_INNER - 2*GUI_MARGIN_OUTER,
                                 GUI_ELEMENT_HEIGHT);
    }
}

void GuiMenu::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    for (auto t : toggles) {
        t->mouseMoved(mouseX, mouseY);
    }
}

void GuiMenu::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (active)
    {
        for (auto t : toggles) {
            t->mousePressed(mouseX, mouseY);
        }
    }
}

void GuiMenu::mouseReleased(int mouseX, int mouseY)
{
    GuiElement::mouseReleased(mouseX, mouseY);
    for (auto t : toggles) {
        t->mouseReleased(mouseX, mouseY);
    }
}

void GuiMenu::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
    for (auto t : toggles) {
        t->mouseDragged(mouseX, mouseY);
    }
}

void GuiMenu::draw()
{
    ofPushMatrix();
	ofPushStyle();
    for (auto t : toggles)
    {
        t->draw();
    }
	ofPopStyle();
	ofPopMatrix();
}


GuiMenuEntry::GuiMenuEntry(string name, vector<string> choices) : GuiWidgetBase(name)
{
    isList = true;
    collapsed = false;
    autoClose = false;
    menu = new GuiMenu(name, choices, this, &GuiMenuEntry::updateParameter);
    elements.push_back(menu);
    setupGuiComponents();
    setAutoUpdate(false);
    setAutoDraw(false);
}

void GuiMenuEntry::setAutoClose(bool autoClose)
{
    this->autoClose = autoClose;
}

bool GuiMenuEntry::getAutoClose()
{
    return autoClose;
}

ofPoint GuiMenuEntry::getGuiElementSize()
{
    return ofPoint(GUI_WIDTH - 2*GUI_MARGIN_OUTER - 2*GUI_MARGIN_INNER, menu->getParameters()->size() * (GUI_ELEMENT_HEIGHT + GUI_MARGIN_BETWEEN));
}

void GuiMenuEntry::updateParameter(GuiButtonEventArgs & button)
{
    setHeader(button.name);
    if (autoClose)  collapsed = true;
    ofNotifyEvent(widgetChanged, button.name, this);
}
