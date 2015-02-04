#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiWidget.h"



class GuiMenu : public GuiElement
{
public:
    template <typename L, typename M>
    GuiMenu(string name, vector<string> choices, L *listener, M method);
    GuiMenu(string name, vector<string> choices);

    void setupMenu(vector<string> choices);
    
    void mouseMoved(int mouseX, int mouseY);
    void mousePressed(int mouseX, int mouseY);
    void mouseReleased(int mouseX, int mouseY);
    void mouseDragged(int mouseX, int mouseY);
    
    void setMultipleChoice(bool multipleChoice) {this->multipleChoice = multipleChoice;}
    bool getMultipleChoice() {return multipleChoice;}
    
    vector<Parameter<bool>*> * getParameters() {return &parameters;}
    
    void setAutoUpdate(bool autoUpdate);
    void setAutoDraw(bool autoDraw);
    void setupGuiComponents();
    
    void draw();
    
    ofEvent<GuiButtonEventArgs> menuEvent;
    
private:
    
    void updateParameter(GuiButtonEventArgs & button);
    
    vector<GuiToggle*> toggles;
    vector<Parameter<bool>*> parameters;
    bool multipleChoice;
    
    
};

template <typename L, typename M>
GuiMenu::GuiMenu(string name, vector<string> choices, L *listener, M method) : GuiElement(name)
{
    setupMenu(choices);
    ofAddListener(menuEvent, listener, method);
}



class GuiMenuEntry : public GuiWidgetBase
{
public:
    GuiMenuEntry(string name, vector<string> choices);
    
    void setAutoClose(bool autoClose);
    bool getAutoClose();
    
    ofPoint getGuiElementSize();
    
    GuiMenu * getMenu() {return menu;}
    
private:
    
    void updateParameter(GuiButtonEventArgs & button);
    
    GuiMenu *menu;
    bool autoClose;
};
