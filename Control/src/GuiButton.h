#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "Sequence.h"


class GuiButtonBase : public GuiElement
{
public:
    template <typename L, typename M>
    GuiButtonBase(Parameter<bool> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    GuiButtonBase(string name, bool *value, L *listener, M method);
    
    template <typename L, typename M>
    GuiButtonBase(string name, L *listener, M method);
    
    GuiButtonBase(Parameter<bool> *parameter);
    GuiButtonBase(string name, bool *value);
    GuiButtonBase(string name);
    
    virtual ~GuiButtonBase();
        
    bool isDiscrete() {return true;}

    void setTriggerAll(bool triggerAll);
    void setLeftJustified(bool leftJustified) {this->leftJustified = leftJustified;}
    
    bool getValue();
    void setValue(bool value, bool sendChangeNotification=false);

    void lerpTo(float nextValue, int numFrames);
    
    int getMenuIndex() {return menuIndex;}
    void setMenuIndex(int menuIndex) {this->menuIndex = menuIndex;}
    
    virtual void update();
    virtual void draw();
    
    
    ///////
    void getXml(ofXml &xml) {
        xml.addValue("Name", getName());
        xml.addValue<bool>("Value", getValue());
    }
    void setFromXml(ofXml &xml) {
        setValue(xml.getValue<bool>("Value"), true);
    }
    
    //////

    
protected:
    
    void setupButton();

    void setValueFromSequence(Sequence &sequence);
    void setSequenceFromValue(Sequence &sequence, int column);
    void setSequenceFromExplicitValue(Sequence &sequence, int column, float value);

    Parameter<bool> *parameter;
    int menuIndex;
    float stringWidth, stringHeight;
    bool leftJustified;
    
    float lerpNextValue;;
    int lerpFrame, lerpNumFrames;
    
    bool triggerAll;
};


template <typename L, typename M>
GuiButtonBase::GuiButtonBase(Parameter<bool> *parameter, L *listener, M method) : GuiElement(name)
{
    this->parameter = parameter;
    setupButton();
    ofAddListener(elementEvent, listener, method);
}

template <typename L, typename M>
GuiButtonBase::GuiButtonBase(string name, bool *value, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<bool>(name, value);
    setupButton();
    ofAddListener(elementEvent, listener, method);
}

template <typename L, typename M>
GuiButtonBase::GuiButtonBase(string name, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<bool>(name, new bool());
    setupButton();
    ofAddListener(elementEvent, listener, method);
}



class GuiButton : public GuiButtonBase
{
public:
    GuiButton(Parameter<bool> *parameter) : GuiButtonBase(parameter) { }
    GuiButton(string name, bool *value) : GuiButtonBase(name, value) { }
    GuiButton(string name) : GuiButtonBase(name) { }

    template <typename L, typename M>
    GuiButton(Parameter<bool> *parameter, L *listener, M method) : GuiButtonBase(parameter, listener, method) { }
    
    template <typename L, typename M>
    GuiButton(string name, bool *value, L *listener, M method) : GuiButtonBase(name, value, listener, method) { }
    
    template <typename L, typename M>
    GuiButton(string name, L *listener, M method) : GuiButtonBase(name, listener, method) { }
    
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
};



class GuiToggle : public GuiButtonBase
{
public:
    GuiToggle(Parameter<bool> *parameter) : GuiButtonBase(parameter) { }
    GuiToggle(string name, bool *value) : GuiButtonBase(name, value) { }
    GuiToggle(string name) : GuiButtonBase(name) { }

    template <typename L, typename M>
    GuiToggle(Parameter<bool> *parameter, L *listener, M method) : GuiButtonBase(parameter, listener, method) { }
    
    template <typename L, typename M>
    GuiToggle(string name, bool *value, L *listener, M method) : GuiButtonBase(name, value, listener, method) { }
    
    template <typename L, typename M>
    GuiToggle(string name, L *listener, M method) : GuiButtonBase(name, listener, method) { }
    
    bool mousePressed(int mouseX, int mouseY);
    bool keyPressed(int key);
};
