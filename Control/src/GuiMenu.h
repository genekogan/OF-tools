#pragma once

#include "ofMain.h"
#include "GuiButton.h"
#include "GuiWidgetBase.h"


class GuiMenu : public GuiWidgetBase
{
public:
    GuiMenu(string name, vector<string> choices, bool multipleChoice=false, bool autoClose=false);
    GuiMenu(string name, bool multipleChoice=false, bool autoClose=false);
    
    template <typename L, typename M>
    GuiMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice=false, bool autoClose=false);
    
    template <typename L, typename M>
    GuiMenu(string name, L *listener, M method, bool multipleChoice=false, bool autoClose=false);
    
    ~GuiMenu();
    
    void setToggle(string toggleName, bool value);
    bool getToggle(string toggleName);
    
    void addToggle(string choice, bool *value);
    void addToggle(string choice);
    
    template <typename L, typename M>
    void addToggle(string choice, L *listener, M method);
    
    void removeToggle(string toggleName);
    void clearToggles();
    
    void setAutoClose(bool autoClose) {this->autoClose = autoClose;}
    void setMultipleChoice(bool multipleChoice) {this->multipleChoice = multipleChoice;}
    
    bool getAutoClose() {return autoClose;}
    bool getMultipleChoice() {return multipleChoice;}
    bool isMenu() {return true;}
    
    vector<ParameterBase*> getParameters() {return parameters;}
    
private:
    
    void setupMenu(bool multipleChoice, bool autoClose);
    void initializeToggles(vector<string> & choices);
    void updateParameter(GuiElementEventArgs & button);
    
    bool multipleChoice;
    bool autoClose;
    
    GuiElementGroup *menuGroup;
    map<string, GuiToggle*> toggles;
    vector<ParameterBase*> parameters;
};

template <typename L, typename M>
GuiMenu::GuiMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice, bool autoClose) : GuiWidgetBase(name)
{
    setupMenu(multipleChoice, autoClose);
    initializeToggles(choices);
    ofAddListener(elementEvent, listener, method);
}

template <typename L, typename M>
GuiMenu::GuiMenu(string name, L *listener, M method, bool multipleChoice, bool autoClose) : GuiWidgetBase(name)
{
    setupMenu(multipleChoice, autoClose);
    ofAddListener(elementEvent, listener, method);
}

template <typename L, typename M>
void GuiMenu::addToggle(string choice, L *listener, M method)
{
    addToggle(choice);
    ofAddListener(toggles[choice]->elementEvent, listener, method);
}
