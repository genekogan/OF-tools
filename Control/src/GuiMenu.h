#pragma once

#include "GuiWidget.h"


class GuiMenu : public GuiWidget
{
public:
    template <typename L, typename M>
    GuiMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice=false, bool autoClose=false);
    GuiMenu(string name, vector<string> choices, bool multipleChoice=false, bool autoClose=false);
    ~GuiMenu();
    
    bool isMenu() {return true;}

    void setAutoClose(bool autoClose) {this->autoClose = autoClose;}
    bool getAutoClose() {return autoClose;}
    
    void setMultipleChoice(bool multipleChoice) {this->multipleChoice = multipleChoice;}
    bool getMultipleChoice() {return multipleChoice;}

    void setToggle(string toggleName, bool value);
    bool getToggle(string toggleName);
    
private:

    void setupMenu(vector<string> & choices);
    void updateParameter(GuiElementEventArgs & button);
    
    bool multipleChoice;
    bool autoClose;
    float headerStringHeight;
    
    map<string, GuiToggle*> toggles;
};

template <typename L, typename M>
GuiMenu::GuiMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice, bool autoClose) : GuiWidget(name)
{
    this->multipleChoice = multipleChoice;
    this->autoClose = autoClose;
    setupMenu(choices);
    ofAddListener(elementEvent, listener, method);
}

