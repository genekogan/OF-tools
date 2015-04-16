#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "Sequence.h"
#include "TouchOsc.h"


class GuiButtonBase;

struct GuiButtonEventArgs
{
    GuiButtonBase *button;
    bool value;
    
    GuiButtonEventArgs(GuiButtonBase *button, bool value)
    {
        this->button = button;
        this->value = value;
    }
};


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
        
    void setLeftJustified(bool leftJustified) {this->leftJustified = leftJustified;}
    
    bool getValue();
    void setValue(bool value, bool sendChangeNotification=false);
    bool isDiscrete() {return true;}

    void lerpTo(float nextValue, int numFrames);
    
    void getParameters(vector<ParameterBase*> & parameters);

    virtual void update();
    virtual void draw();

    void setupGuiPositions();
    void addElementToTouchOscLayout(TouchOscPage *page, float *y);

    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
    ofEvent<GuiButtonEventArgs> buttonEvent;
    
protected:
    
    void initializeButton();
    void updateParameterOscAddress();

    void setValueFromSequence(Sequence &sequence);
    void setSequenceFromValue(Sequence &sequence, int column);
    void setSequenceFromExplicitValue(Sequence &sequence, int column, float value);

    Parameter<bool> *parameter;
    float stringWidth, stringHeight;
    bool leftJustified;
    
    float lerpNextValue;;
    int lerpFrame, lerpNumFrames;
};


template <typename L, typename M>
GuiButtonBase::GuiButtonBase(Parameter<bool> *parameter, L *listener, M method) : GuiElement(name)
{
    this->parameter = parameter;
    initializeButton();
    ofAddListener(buttonEvent, listener, method);
}

template <typename L, typename M>
GuiButtonBase::GuiButtonBase(string name, bool *value, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<bool>(name, value);
    initializeButton();
    ofAddListener(buttonEvent, listener, method);
}

template <typename L, typename M>
GuiButtonBase::GuiButtonBase(string name, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<bool>(name, new bool());
    initializeButton();
    ofAddListener(buttonEvent, listener, method);
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
    bool keyPressed(int key);
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
