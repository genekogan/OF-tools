#pragma once

#include "ofBitmapFont.h"
#include "Parameter.h"
#include "GuiElement.h"
#include "Sequence.h"


class GuiTextBox : public GuiElement
{
public:
    template <typename L, typename M>
    GuiTextBox(Parameter<string> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    GuiTextBox(string name, string *value, L *listener, M method);
    
    template <typename L, typename M>
    GuiTextBox(string name, L *listener, M method);
    
    GuiTextBox(Parameter<string> *parameter);
    GuiTextBox(string name, string *value);
    GuiTextBox(string name);
    
    virtual ~GuiTextBox();
    
    void setLeftJustified(bool leftJustified) {this->leftJustified = leftJustified;}
    
    string getValue();
    void setValue(string value, bool sendChangeNotification=false);
    
    void update();
    void draw();
    
    bool mousePressed(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    ///////
    void getXml(ofXml &xml) {
        xml.addValue("Name", getName());
        xml.addValue<string>("Value", parameter->get());
    }
    void setFromXml(ofXml &xml) {
        parameter->set(xml.getValue<string>("Value"));
    }
    
    //////

protected:
    
    void setupTextBox();
    
    Parameter<string> *parameter;
    int cursorPosition;
    bool editing;
    float stringWidth, stringHeight;
    bool leftJustified;
};

template <typename L, typename M>
GuiTextBox::GuiTextBox(Parameter<string> *parameter, L *listener, M method) : GuiElement(parameter->getName())
{
    this->parameter = parameter;
    setupTextBox();
    ofAddListener(elementEvent, listener, method);
}

template <typename L, typename M>
GuiTextBox::GuiTextBox(string name, string *value, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<string>(name, value);
    setupTextBox();
    ofAddListener(elementEvent, listener, method);
}

template <typename L, typename M>
GuiTextBox::GuiTextBox(string name, L *listener, M method) : GuiElement(name)
{
    parameter = new Parameter<string>(name, new string(name));
    setupTextBox();
    ofAddListener(elementEvent, listener, method);
}
