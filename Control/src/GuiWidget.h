#pragma once

#include "ofMain.h"

#include "Parameter.h"

#include "GuiConstants.h"
#include "GuiElement.h"
#include "GuiSlider.h"
#include "GuiButton.h"



class GuiWidgetBase
{
public:
    GuiWidgetBase(string name);
    
    void setRectanglePosition(ofPoint position);
    void setRectangle(ofRectangle rectangle);
    ofRectangle getRectangle();
    virtual ofPoint getGuiElementSize();

    string getName();
    string getHeader();
    
    void setName(string name);
    void setHeader(string header);
    
    vector<GuiElement*> & getElements();
    
    virtual void setActive(bool active);
    virtual bool getActive();
    
    virtual void setAutoUpdate(bool autoUpdate);
    virtual void setAutoDraw(bool autoDraw);

    virtual void setup();
    virtual void setupGuiComponents();
    
    virtual void update();
    //virtual void updateParameter(string &s);
    
    virtual void draw();
    
    virtual void mouseMoved(int mouseX, int mouseY);
    virtual void mousePressed(int mouseX, int mouseY);
    virtual void mouseReleased(int mouseX, int mouseY);
    virtual void mouseDragged(int mouseX, int mouseY);
    
    string name, header;
    ofRectangle rectangle, headerRectangle;
    bool active, headerActive;
    vector<GuiElement*> elements;
    bool isList, collapsed;
    ofEvent<string> widgetChanged;
};



template<typename T>
class GuiWidget : public GuiWidgetBase
{
public:
    GuiWidget(string name, vector<Parameter<T>*> parameters_);
    GuiWidget(string name, Parameter<T> *parameters_);
    
    void setup();
    
private:
    
    void resetEntries();
    //void updateParameter(string &s);
    
    vector<Parameter<T>*> parameters;
    vector<T> previous;
};



//=================================
// TEMPLATES:  CONSTRUCTOR

template<typename T>
GuiWidget<T>::GuiWidget(string name, vector<Parameter<T>*> parameters_) : GuiWidgetBase(name)
{
    this->parameters = parameters_;
    isList = true;
    resetEntries();
}

template<typename T>
GuiWidget<T>::GuiWidget(string name, Parameter<T> *parameters_) : GuiWidgetBase(name)
{
    parameters.push_back(parameters_);
    isList = false;
    resetEntries();
}

template<typename T>
void GuiWidget<T>::resetEntries()
{
    previous.resize(parameters.size());
    collapsed = false;
    setup();
    setupGuiComponents();
    setAutoUpdate(false);
    setAutoDraw(false);
}



//=================================
// TEMPLATES:  SETUP

template<typename T> inline void GuiWidget<T>::setup()
{
    
}
template<> inline void GuiWidget<bool>::setup()
{
    for (int i=0; i<parameters.size(); i++) {
        string buttonName = parameters.size() > 1 ? "["+ofToString(i++)+"]" : name;
        GuiToggle *button = new GuiToggle(buttonName, parameters[i]);
        elements.push_back(button);
//        ofAddListener(button->changeEvent, this, &GuiWidget<bool>::updateParameter);
    }
}
template<> inline void GuiWidget<int>::setup()
{
    for (int i=0; i<parameters.size(); i++) {
        string sliderName = parameters.size() > 1 ? "["+ofToString(i)+"]" : name;
        GuiSlider<int> *slider = new GuiSlider<int>(sliderName, parameters[i]);
        elements.push_back(slider);
//        ofAddListener(slider->changeEvent, this, &GuiWidget<int>::updateParameter);
    }
}
template<> inline void GuiWidget<float>::setup()
{
    for (int i=0; i<parameters.size(); i++) {
        string sliderName = parameters.size() > 1 ? "["+ofToString(i)+"]" : name;
        GuiSlider<float> *slider = new GuiSlider<float>(sliderName, parameters[i]);
        slider->setRectangle(10, 40, 300, 100);
        elements.push_back(slider);
//        ofAddListener(slider->changeEvent, this, &GuiWidget<float>::updateParameter);
    }
}
template<> inline void GuiWidget<double>::setup()
{
    for (int i=0; i<parameters.size(); i++) {
        string sliderName = parameters.size() > 1 ? "["+ofToString(i)+"]" : name;
        GuiSlider<double> *slider = new GuiSlider<double>(sliderName, parameters[i]);
        elements.push_back(slider);
//        ofAddListener(slider->changeEvent, this, &GuiWidget<double>::updateParameter);
    }
}
template<> inline void GuiWidget<ofVec2f>::setup()
{
    for (int i=0; i<parameters.size(); i++) {
        string sliderName = parameters.size() > 1 ? "["+ofToString(i)+"]" : name;
        GuiSlider<float> *sliderX = new GuiSlider<float>(sliderName+".x", &parameters[i]->getReference()->x, parameters[i]->getMin().x, parameters[i]->getMax().x);
        GuiSlider<float> *sliderY = new GuiSlider<float>(sliderName+".y", &parameters[i]->getReference()->y, parameters[i]->getMin().y, parameters[i]->getMax().y);
        elements.push_back(sliderX);
        elements.push_back(sliderY);
//        ofAddListener(sliderX->changeEvent, this, &GuiWidget<ofVec2f>::updateParameter);
//        ofAddListener(sliderY->changeEvent, this, &GuiWidget<ofVec2f>::updateParameter);
        if (i < parameters.size()-1) sliderY->setExtraMargin(true);
    }
}
template<> inline void GuiWidget<ofVec3f>::setup()
{
    for (int i=0; i<parameters.size(); i++) {
        string sliderName = parameters.size() > 1 ? "["+ofToString(i)+"]" : name;
        GuiSlider<float> *sliderX = new GuiSlider<float>(sliderName+".x", &parameters[i]->getReference()->x, parameters[i]->getMin().x, parameters[i]->getMax().x);
        GuiSlider<float> *sliderY = new GuiSlider<float>(sliderName+".y", &parameters[i]->getReference()->y, parameters[i]->getMin().y, parameters[i]->getMax().y);
        GuiSlider<float> *sliderZ = new GuiSlider<float>(sliderName+".z", &parameters[i]->getReference()->z, parameters[i]->getMin().z, parameters[i]->getMax().z);
        elements.push_back(sliderX);
        elements.push_back(sliderY);
        elements.push_back(sliderZ);
//        ofAddListener(sliderX->changeEvent, this, &GuiWidget<ofVec3f>::updateParameter);
//        ofAddListener(sliderY->changeEvent, this, &GuiWidget<ofVec3f>::updateParameter);
//        ofAddListener(sliderZ->changeEvent, this, &GuiWidget<ofVec3f>::updateParameter);
        if (i < parameters.size()-1) sliderZ->setExtraMargin(true);
    }
}
template<> inline void GuiWidget<ofVec4f>::setup()
{
    for (int i=0; i<parameters.size(); i++) {
        string sliderName = parameters.size() > 1 ? "["+ofToString(i)+"]" : name;
        GuiSlider<float> *sliderX = new GuiSlider<float>(sliderName+".x", &parameters[i]->getReference()->x, parameters[i]->getMin().x, parameters[i]->getMax().x);
        GuiSlider<float> *sliderY = new GuiSlider<float>(sliderName+".y", &parameters[i]->getReference()->y, parameters[i]->getMin().y, parameters[i]->getMax().y);
        GuiSlider<float> *sliderZ = new GuiSlider<float>(sliderName+".z", &parameters[i]->getReference()->z, parameters[i]->getMin().z, parameters[i]->getMax().z);
        GuiSlider<float> *sliderW = new GuiSlider<float>(sliderName+".w", &parameters[i]->getReference()->w, parameters[i]->getMin().w, parameters[i]->getMax().w);
        elements.push_back(sliderX);
        elements.push_back(sliderY);
        elements.push_back(sliderZ);
        elements.push_back(sliderW);
//        ofAddListener(sliderX->changeEvent, this, &GuiWidget<ofVec4f>::updateParameter);
//        ofAddListener(sliderY->changeEvent, this, &GuiWidget<ofVec4f>::updateParameter);
//        ofAddListener(sliderZ->changeEvent, this, &GuiWidget<ofVec4f>::updateParameter);
//        ofAddListener(sliderW->changeEvent, this, &GuiWidget<ofVec4f>::updateParameter);
        if (i < parameters.size()-1) sliderW->setExtraMargin(true);
    }
}
template<> inline void GuiWidget<ofFloatColor>::setup()
{
    for (int i=0; i<parameters.size(); i++) {
        string sliderName = parameters.size() > 1 ? "["+ofToString(i)+"]" : name;
        GuiSlider<float> *sliderR = new GuiSlider<float>(sliderName+".r", &parameters[i]->getReference()->r, parameters[i]->getMin().r, parameters[i]->getMax().r);
        GuiSlider<float> *sliderG = new GuiSlider<float>(sliderName+".g", &parameters[i]->getReference()->g, parameters[i]->getMin().g, parameters[i]->getMax().g);
        GuiSlider<float> *sliderB = new GuiSlider<float>(sliderName+".b", &parameters[i]->getReference()->b, parameters[i]->getMin().b, parameters[i]->getMax().b);
        GuiSlider<float> *sliderA = new GuiSlider<float>(sliderName+".a", &parameters[i]->getReference()->a, parameters[i]->getMin().a, parameters[i]->getMax().a);
        elements.push_back(sliderR);
        elements.push_back(sliderG);
        elements.push_back(sliderB);
        elements.push_back(sliderA);
//        ofAddListener(sliderR->changeEvent, this, &GuiWidget<ofFloatColor>::updateParameter);
//        ofAddListener(sliderG->changeEvent, this, &GuiWidget<ofFloatColor>::updateParameter);
//        ofAddListener(sliderB->changeEvent, this, &GuiWidget<ofFloatColor>::updateParameter);
//        ofAddListener(sliderA->changeEvent, this, &GuiWidget<ofFloatColor>::updateParameter);
        if (i < parameters.size()-1) sliderA->setExtraMargin(true);
    }
}
template<> inline void GuiWidget<string>::setup()
{
    
}

/*

//=================================
// TEMPLATES:  UPDATE PARAMTER

template<typename T> inline void GuiWidget<T>::updateParameter(string &s)
{
    ofNotifyEvent(widgetChanged, name, this);
}
template<> inline void GuiWidget<bool>::updateParameter(string &s)
{
    for (int i=0; i<parameters.size(); i++) {
        parameters[i]->set(((GuiButton *) elements[i])->getValue());
    }
    ofNotifyEvent(widgetChanged, name, this);
}
template<> inline void GuiWidget<int>::updateParameter(string &s)
{
    for (int i=0; i<parameters.size(); i++) {
        parameters[i]->set((int) ofLerp(parameters[i]->getMin(), parameters[i]->getMax(), ((GuiSlider<int> *) elements[i])->getSliderValue()));
    }
    ofNotifyEvent(widgetChanged, name, this);
}
template<> inline void GuiWidget<float>::updateParameter(string &s)
{
    for (int i=0; i<parameters.size(); i++) {
        parameters[i]->set(ofLerp(parameters[i]->getMin(), parameters[i]->getMax(), ((GuiSlider<float> *) elements[i])->getSliderValue()));
    }
    ofNotifyEvent(widgetChanged, name, this);
}
template<> inline void GuiWidget<double>::updateParameter(string &s)
{
    for (int i=0; i<parameters.size(); i++) {
        parameters[i]->set(ofLerp(parameters[i]->getMin(), parameters[i]->getMax(), ((GuiSlider<double> *) elements[i])->getSliderValue()));
    }
    ofNotifyEvent(widgetChanged, name, this);
}
template<> inline void GuiWidget<ofVec2f>::updateParameter(string &s)
{
    for (int i=0; i<parameters.size(); i++) {
        parameters[i]->set(ofVec2f(ofLerp(parameters[i]->getMin().x, parameters[i]->getMax().x, ((GuiSlider<float> *) elements[2*i ])->getSliderValue()),
                                   ofLerp(parameters[i]->getMin().y, parameters[i]->getMax().y, ((GuiSlider<float> *) elements[2*i+1])->getSliderValue())));
    }
    ofNotifyEvent(widgetChanged, name, this);
}
template<> inline void GuiWidget<ofVec3f>::updateParameter(string &s)
{
    for (int i=0; i<parameters.size(); i++) {
        parameters[i]->set(ofVec3f(ofLerp(parameters[i]->getMin().x, parameters[i]->getMax().x, ((GuiSlider<float> *) elements[3*i  ])->getSliderValue()),
                                   ofLerp(parameters[i]->getMin().y, parameters[i]->getMax().y, ((GuiSlider<float> *) elements[3*i+1])->getSliderValue()),
                                   ofLerp(parameters[i]->getMin().z, parameters[i]->getMax().z, ((GuiSlider<float> *) elements[3*i+2])->getSliderValue())));
    }
    ofNotifyEvent(widgetChanged, name, this);
}
template<> inline void GuiWidget<ofVec4f>::updateParameter(string &s)
{
    for (int i=0; i<parameters.size(); i++) {
        parameters[i]->set(ofVec4f(ofLerp(parameters[i]->getMin().x, parameters[i]->getMax().x, ((GuiSlider<float> *) elements[4*i  ])->getSliderValue()),
                                   ofLerp(parameters[i]->getMin().y, parameters[i]->getMax().y, ((GuiSlider<float> *) elements[4*i+1])->getSliderValue()),
                                   ofLerp(parameters[i]->getMin().z, parameters[i]->getMax().z, ((GuiSlider<float> *) elements[4*i+2])->getSliderValue()),
                                   ofLerp(parameters[i]->getMin().w, parameters[i]->getMax().w, ((GuiSlider<float> *) elements[4*i+3])->getSliderValue())));
    }
    ofNotifyEvent(widgetChanged, name, this);
}
template<> inline void GuiWidget<ofFloatColor>::updateParameter(string &s)
{
    for (int i=0; i<parameters.size(); i++) {
        parameters[i]->set(ofFloatColor(ofLerp(parameters[i]->getMin().r, parameters[i]->getMax().r, ((GuiSlider<float> *) elements[4*i  ])->getSliderValue()),
                                        ofLerp(parameters[i]->getMin().g, parameters[i]->getMax().g, ((GuiSlider<float> *) elements[4*i+1])->getSliderValue()),
                                        ofLerp(parameters[i]->getMin().b, parameters[i]->getMax().b, ((GuiSlider<float> *) elements[4*i+2])->getSliderValue()),
                                        ofLerp(parameters[i]->getMin().a, parameters[i]->getMax().a, ((GuiSlider<float> *) elements[4*i+3])->getSliderValue())));
    }
    ofNotifyEvent(widgetChanged, name, this);
}
template<> inline void GuiWidget<string>::updateParameter(string &s)
{
    ofNotifyEvent(widgetChanged, name, this);
}
*/