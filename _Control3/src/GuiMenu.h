#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "GuiButton.h"
#include "GuiMultiElement.h"
//#include "Sequence.h"



class GuiMenu;

struct GuiMenuEventArgs
{
    GuiToggle *toggle;
    int index;
    bool value;
    
    GuiMenuEventArgs(GuiToggle *toggle, int index, bool value)
    {
        this->toggle = toggle;
        this->index = index;
        this->value = value;
    }
};


class GuiMenu : public GuiMultiElement
{
public:

    struct MenuElement
    {
        GuiToggle *toggle;
        int index;
        Parameter<bool> *parameter;
        MenuElement(GuiToggle *toggle, Parameter<bool> *parameter, int index);
    };
    
    GuiMenu(string name, vector<string> choices, bool multipleChoice=false, bool autoClose=false);
    GuiMenu(string name, bool multipleChoice=false, bool autoClose=false);
    
    template <typename L, typename M>
    GuiMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice=false, bool autoClose=false);
    
    template <typename L, typename M>
    GuiMenu(string name, L *listener, M method, bool multipleChoice=false, bool autoClose=false);

    ~GuiMenu();

    GuiToggle * addToggle(Parameter<bool> *parameter);
    GuiToggle * addToggle(string choice, bool *value);
    GuiToggle * addToggle(string choice);
    
    template <typename L, typename M>
    GuiToggle * addToggle(Parameter<bool> *parameter, L *listener, M method);

    template <typename L, typename M>
    GuiToggle * addToggle(string choice, bool *value, L *listener, M method);
    
    template <typename L, typename M>
    GuiToggle * addToggle(string choice, L *listener, M method);

    void removeToggle(string toggleName);
    void clearToggles();
    
    void setToggle(string toggleName, bool value);
    bool getToggle(string toggleName);
    
    void setAutoClose(bool autoClose);
    void setMultipleChoice(bool multipleChoice) {this->multipleChoice = multipleChoice;}
    
    bool getAutoClose() {return autoClose;}
    bool getMultipleChoice() {return multipleChoice;}
    
    ofEvent<GuiMenuEventArgs> menuEvent;
    
private:

    void setupMenu(string name, vector<string> & choices, bool multipleChoice, bool autoClose);
    void resetIndices();
    void buttonChanged(GuiButtonEventArgs &e);
    
    bool multipleChoice;
    bool autoClose;

    vector<Parameter<bool>*> parameters;
    map<string, MenuElement*> menuElements;
};


template <typename L, typename M>
GuiMenu::GuiMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice, bool autoClose) : GuiMultiElement(name)
{
    setupMenu(name, choices, multipleChoice, autoClose);
    ofAddListener(menuEvent, listener, method);
}

template <typename L, typename M>
GuiMenu::GuiMenu(string name, L *listener, M method, bool multipleChoice, bool autoClose) : GuiMultiElement(name)
{
    vector<string> choices;
    setupMenu(name, choices, multipleChoice, autoClose);
    ofAddListener(menuEvent, listener, method);
}

template <typename L, typename M>
GuiToggle * GuiMenu::addToggle(Parameter<bool> *parameter, L *listener, M method)
{
    GuiToggle *toggle = addToggle(parameter);
    if (toggle != NULL) {
        ofAddListener(menuElements[toggle->getName()], listener, method);
    }
    return toggle;
}

template <typename L, typename M>
GuiToggle * GuiMenu::addToggle(string choice, bool *value, L *listener, M method)
{
    GuiToggle *toggle = addToggle(choice, value);
    if (toggle != NULL) {
        ofAddListener(menuElements[toggle->getName()], listener, method);
    }
    return toggle;
}

template <typename L, typename M>
GuiToggle * GuiMenu::addToggle(string choice, L *listener, M method)
{
    GuiToggle *toggle = addToggle(choice);
    if (toggle != NULL) {
        ofAddListener(menuElements[toggle->getName()], listener, method);
    }
    return toggle;
}
