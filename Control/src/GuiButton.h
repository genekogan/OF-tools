#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"


class GuiButtonBase : public GuiElement
{
public:
    template <typename L, typename M>
    GuiButtonBase(string name, Parameter<bool> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    GuiButtonBase(string name, bool *value, L *listener, M method);
    
    template <typename L, typename M>
    GuiButtonBase(string name, L *listener, M method);
    
    GuiButtonBase(string name, Parameter<bool> *parameter);
    GuiButtonBase(string name, bool *value);
    GuiButtonBase(string name);
    
    virtual void mouseMoved(int mouseX, int mouseY);
    virtual void mousePressed(int mouseX, int mouseY);
    virtual void mouseReleased(int mouseX, int mouseY);
    virtual void mouseDragged(int mouseX, int mouseY);
    
    void setValue(bool value, bool sendChangeNotification=false);
    bool getValue();
    
    virtual void update();
    virtual void draw();
    
    ofEvent<GuiButtonEventArgs> buttonEvent;
    
protected:
    void setupButton();
    
    Parameter<bool> *parameter;
    bool previous;
    float stringWidth, stringHeight;
};


template <typename L, typename M>
GuiButtonBase::GuiButtonBase(string name, Parameter<bool> *parameter, L *listener, M method) : GuiElement(name)
{
    this->parameter = parameter;
    setupButton();
    ofAddListener(buttonEvent, listener, method);
}

template <typename L, typename M>
GuiButtonBase::GuiButtonBase(string name, bool *value, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<bool>(name, value);
    setupButton();
    ofAddListener(buttonEvent, listener, method);
}

template <typename L, typename M>
GuiButtonBase::GuiButtonBase(string name, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<bool>(name, new bool());
    setupButton();
    ofAddListener(buttonEvent, listener, method);
}



class GuiButton : public GuiButtonBase
{
public:
    template <typename L, typename M>
    GuiButton(string name, Parameter<bool> *parameter, L *listener, M method) : GuiButtonBase(name, parameter, listener, method) { }
    
    template <typename L, typename M>
    GuiButton(string name, bool *value, L *listener, M method) : GuiButtonBase(name, value, listener, method) { }
    
    template <typename L, typename M>
    GuiButton(string name, L *listener, M method) : GuiButtonBase(name, new bool(), listener, method) { }
    
    GuiButton(string name, Parameter<bool> *parameter) : GuiButtonBase(name, parameter) { }
    GuiButton(string name, bool *value) : GuiButtonBase(name, value) { }
    GuiButton(string name) : GuiButtonBase(name, new bool()) { }

    void mousePressed(int mouseX, int mouseY);
    void mouseReleased(int mouseX, int mouseY);
};



class GuiToggle : public GuiButtonBase
{
public:
    template <typename L, typename M>
    GuiToggle(string name, Parameter<bool> *parameter, L *listener, M method) : GuiButtonBase(name, parameter, listener, method) { }
    
    template <typename L, typename M>
    GuiToggle(string name, bool *value, L *listener, M method) : GuiButtonBase(name, value, listener, method) { }
    
    template <typename L, typename M>
    GuiToggle(string name, L *listener, M method) : GuiButtonBase(name, new bool(), listener, method) { }
    
    GuiToggle(string name, Parameter<bool> *parameter) : GuiButtonBase(name, parameter) { }
    GuiToggle(string name, bool *value) : GuiButtonBase(name, value) { }
    GuiToggle(string name) : GuiButtonBase(name, new bool()) { }
    
    void mousePressed(int mouseX, int mouseY);
};
