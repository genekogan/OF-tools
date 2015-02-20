#pragma once

#include "ofMain.h"
#include "GuiSlider.h"
#include "GuiButton.h"
#include "GuiWidget.h"


class GuiWidget : public GuiElement
{
public:
    GuiWidget(string name);
    GuiWidget(string name, ParameterBase *parameter_);
    GuiWidget(string name, vector<ParameterBase*> & parameters_);
    
    template <typename L, typename M>
    GuiWidget(string name, ParameterBase *parameter_, L *listener, M method);
    
    template <typename L, typename M>
    GuiWidget(string name, vector<ParameterBase*> & parameters_, L *listener, M method);

    virtual ~GuiWidget();
    
    virtual bool isMenu() {return false;}
    
    virtual void setAutoUpdate(bool autoUpdate);
    virtual void setAutoDraw(bool autoDraw);
    
    virtual void setupGuiComponents();
    
    virtual void update();
    virtual void draw();
    
    virtual void mouseMoved(int mouseX, int mouseY);
    virtual void mousePressed(int mouseX, int mouseY);
    virtual void mouseReleased(int mouseX, int mouseY);
    virtual void mouseDragged(int mouseX, int mouseY);
    
    virtual void setCollapsed(bool collapsed) {this->collapsed = isList ? collapsed : false;}
    virtual bool getCollapsed() {return collapsed;}
    
    virtual vector<GuiElement*> & getElements();
    
    ofEvent<string> widgetChanged;
    
protected:
    
    virtual void setupWidget();
    
    template <typename T>
    void createElements(Parameter<T> *parameter);
    template <typename L, typename M>
    void addListeners(L *listener, M method=&GuiWidget::createElements);

    //vector<T> previous;

    vector<ParameterBase*> parameters;
    vector<GuiElement*> elements;
    ofRectangle headerRectangle;
    bool headerActive;
    bool isList, collapsed;
    string header;
    float headerStringHeight;
};

class GuiColor : public GuiWidget
{
public:
    GuiColor(string name, Parameter<ofFloatColor> *parameter);
    GuiColor(string name, ofFloatColor *value);
    
    void update()
    {
        GuiWidget::update();
        ofColor currentColor = parameters[0]->get<ofFloatColor>();
        elements[0]->getStyle().colorForeground = ofColor(currentColor.r, currentColor.g, currentColor.b);
        elements[1]->getStyle().colorForeground = ofColor(currentColor.r, currentColor.g, currentColor.b);
        elements[2]->getStyle().colorForeground = ofColor(currentColor.r, currentColor.g, currentColor.b);
        elements[3]->getStyle().colorForeground = currentColor;
    }
};

class GuiEvent : public GuiWidget
{
public:
    GuiEvent(string name, Parameter<bool> *parameter);
    
    template <typename L, typename M>
    GuiEvent(string name, Parameter<bool> *parameter,  L *listener, M method);
    
    void setupWidget();
};


//////////////////////////////////////////////////////
// TEMPLATE:  CONSTRUCT / DESTRUCT

template <typename L, typename M>
GuiWidget::GuiWidget(string name, ParameterBase *parameter_, L *listener, M method) : GuiElement(name)
{
    parameters.push_back(parameter_);
    setupWidget();
    addListeners(listener, method);
}

template <typename L, typename M>
GuiWidget::GuiWidget(string name, vector<ParameterBase*> & parameters_, L *listener, M method) : GuiElement(name)
{
    this->parameters = parameters_;
    setupWidget();
    addListeners(listener, method);
}

template <typename L, typename M>
GuiEvent::GuiEvent(string name, Parameter<bool> *parameter,  L *listener, M method) : GuiWidget(name)
{
    parameters.push_back(parameter);
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
    setupWidget();
    addListeners(listener, method);
}


//////////////////////////////////////////////////////
// TEMPLATE : create elements from parameter

template <typename T>
inline void GuiWidget::createElements(Parameter<T> *parameter)
{

}
template <> inline void GuiWidget::createElements(Parameter<bool> *parameter)
{
    GuiToggle *button = new GuiToggle(parameter->getName(), parameter);
    elements.push_back(button);
}
template <> inline void GuiWidget::createElements(Parameter<int> *parameter)
{
    GuiSlider<int> *slider = new GuiSlider<int>(parameter->getName(), parameter);
    elements.push_back(slider);
}
template <> inline void GuiWidget::createElements(Parameter<float> *parameter)
{
    GuiSlider<float> *slider = new GuiSlider<float>(parameter->getName(), parameter);
    elements.push_back(slider);

}
template <> inline void GuiWidget::createElements(Parameter<double> *parameter)
{
    GuiSlider<double> *slider = new GuiSlider<double>(parameter->getName(), parameter);
    elements.push_back(slider);
}
template <> inline void GuiWidget::createElements(Parameter<ofVec2f> *parameter)
{
    GuiSlider<float> *sliderX = new GuiSlider<float>(parameter->getName()+".x", &parameter->getReference()->x, parameter->getMin().x, parameter->getMax().x);
    GuiSlider<float> *sliderY = new GuiSlider<float>(parameter->getName()+".y", &parameter->getReference()->y, parameter->getMin().y, parameter->getMax().y);
    elements.push_back(sliderX);
    elements.push_back(sliderY);
}
template <>
inline void GuiWidget::createElements(Parameter<ofVec3f> *parameter)
{
    GuiSlider<float> *sliderX = new GuiSlider<float>(parameter->getName()+".x", &parameter->getReference()->x, parameter->getMin().x, parameter->getMax().x);
    GuiSlider<float> *sliderY = new GuiSlider<float>(parameter->getName()+".y", &parameter->getReference()->y, parameter->getMin().y, parameter->getMax().y);
    GuiSlider<float> *sliderZ = new GuiSlider<float>(parameter->getName()+".z", &parameter->getReference()->z, parameter->getMin().z, parameter->getMax().z);
    elements.push_back(sliderX);
    elements.push_back(sliderY);
    elements.push_back(sliderZ);
}
template <>
inline void GuiWidget::createElements(Parameter<ofVec4f> *parameter)
{
    GuiSlider<float> *sliderX = new GuiSlider<float>(parameter->getName()+".x", &parameter->getReference()->x, parameter->getMin().x, parameter->getMax().x);
    GuiSlider<float> *sliderY = new GuiSlider<float>(parameter->getName()+".y", &parameter->getReference()->y, parameter->getMin().y, parameter->getMax().y);
    GuiSlider<float> *sliderZ = new GuiSlider<float>(parameter->getName()+".z", &parameter->getReference()->z, parameter->getMin().z, parameter->getMax().z);
    GuiSlider<float> *sliderW = new GuiSlider<float>(parameter->getName()+".w", &parameter->getReference()->w, parameter->getMin().w, parameter->getMax().w);
    elements.push_back(sliderX);
    elements.push_back(sliderY);
    elements.push_back(sliderZ);
    elements.push_back(sliderW);
}
template <>
inline void GuiWidget::createElements(Parameter<ofFloatColor> *parameter)
{
    GuiSlider<float> *sliderR = new GuiSlider<float>(parameter->getName()+".r", &parameter->getReference()->r, parameter->getMin().r, parameter->getMax().r);
    GuiSlider<float> *sliderG = new GuiSlider<float>(parameter->getName()+".g", &parameter->getReference()->g, parameter->getMin().g, parameter->getMax().g);
    GuiSlider<float> *sliderB = new GuiSlider<float>(parameter->getName()+".b", &parameter->getReference()->b, parameter->getMin().b, parameter->getMax().b);
    GuiSlider<float> *sliderA = new GuiSlider<float>(parameter->getName()+".a", &parameter->getReference()->a, parameter->getMin().a, parameter->getMax().a);
    elements.push_back(sliderR);
    elements.push_back(sliderG);
    elements.push_back(sliderB);
    elements.push_back(sliderA);
}



//////////////////////////////////////////////////////
// TEMPLATE : add listeners for each element

template <typename L, typename M>
void GuiWidget::addListeners(L *listener, M method)
{
    for (auto e : elements)
    {
        ofAddListener(e->elementEvent, listener, method);
    }
}

