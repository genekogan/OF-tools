#pragma once

#include "GuiColor.h"
#include "GuiMenu.h"


class GuiPanel
{
public:
    GuiPanel();
    ~GuiPanel();
    
    void setAutoUpdate(bool autoUpdate);
    void setAutoDraw(bool autoDraw);
    
    void setName(string name);
    string getName() {return name;}
    
    void update();
    void draw();
    
    void setPosition(int x, int y);
    void setWidth(int width);
    
    ofRectangle getRectangle() {return rectangle;}
    

    void mouseMoved(int mouseX, int mouseY);
    void mousePressed(int mouseX, int mouseY);
    void mouseReleased(int mouseX, int mouseY);
    void mouseDragged(int mouseX, int mouseY);

    
    
    // is the widget map needed?
    void removeWidget(string name)
    {
        vector<GuiWidget *>::iterator it = widgets.begin();
        while (it < widgets.end())
        {
            if ((*it)->getName() == name)
            {
                delete *it;
                widgets.erase(it);
                break;
            }
            else {
                ++it;
            }
        }
        if (widgetMap.count(name) > 0) {
            widgetMap.erase(name);
        }
        resetElementPositions();
    }
    
    // inserting widget in index, or before/after another widget

    
    
    void addParameter(string name, ParameterBase *parameter);
    void addParameter(string name, vector<ParameterBase*> parameterGroup);
    
    template <typename T>
    void addParameter(string name, T *value, T min, T max);
    template <typename T>
    void addParameter(string name, T *value);
    template <typename T>
    void addParameter(string name, vector<T> *values, vector<T> min, vector<T> max);
    template <typename T>
    void addParameter(string name, vector<T> *values);
    template <typename L, typename M>
    void addParameter(string name, ParameterBase *parameter, L *listener, M method);
    template <typename L, typename M>
    void addParameter(string name, vector<ParameterBase*> parameterGroup, L *listener, M method);
    
    template <typename T, typename L, typename M>
    void addParameter(string name, T *value, T min, T max, L *listener, M method);
    template <typename T, typename L, typename M>
    void addParameter(string name, T *value, L *listener, M method);
    template <typename T, typename L, typename M>
    void addParameter(string name, vector<T> *values, vector<T> min, vector<T> max, L *listener, M method);
    template <typename T, typename L, typename M>
    void addParameter(string name, vector<T> *values, L *listener, M method);

    template <typename L, typename M>
    void addEvent(string name, L *listener, M method);

    template <typename L, typename M>
    void addMenu(string name, vector<string> items, L *listener, M method, bool multipleChoice=false, bool autoClose=false);

    void addColor(string name, Parameter<ofFloatColor> *parameter);
    void addColor(string name, ofFloatColor *color);

    template <typename L, typename M>
    void addColor(string name, Parameter<ofFloatColor> *parameter, L *listener, M method);
    template <typename L, typename M>
    void addColor(string name, ofFloatColor *color, L *listener, M method);

    map<string, ParameterBase *> * getParameters() {return &parameters;}
    vector<GuiWidget *> & getWidgets() {return widgets;}
    
    void setMarginInner(float marginInner);
    void setMarginBetween(float marginBetween);
    void setElementWidth(float elementWidth);
    void setElementHeight(float elementHeight);
    void setColorBackground(ofColor colorBackground);
    void setColorForeground(ofColor colorForeground);
    void setColorActive(ofColor colorActive);
    void setColorText(ofColor colorText);
    
private:
    
    void mouseMoved(ofMouseEventArgs &evt) {mouseMoved(evt.x, evt.y);}
    void mousePressed(ofMouseEventArgs &evt) {mousePressed(evt.x, evt.y);}
    void mouseReleased(ofMouseEventArgs &evt) {mouseReleased(evt.x, evt.y);}
    void mouseDragged(ofMouseEventArgs &evt) {mouseDragged(evt.x, evt.y);}
    
    void update(ofEventArgs & data) {update();}
    void draw(ofEventArgs & data) {draw();}

    void clearWidgets();
    void receiveWidgetChangedEvent(string &s);
    void resetElementPositions();
    void resetStyle();
    ofPoint getGuiPosition(int idxPosition, bool bottom=true);
    ofPoint getGuiPosition();
    
    void setupNewWidget(string widgetName, GuiWidget *widget);
    void updateWidgets();
    
    void drawPanel();
    void drawWidgets();
    
    bool autoUpdate;
    bool autoDraw;
    string name;
    ofRectangle rectangle;
    GuiStyle style;
    
    map<string, ParameterBase *> parameters;
    map<string, GuiWidget *> widgetMap;
    vector<GuiWidget *> widgets;
};



template <typename T>
void GuiPanel::addParameter(string name, T *value, T min, T max)
{
    ParameterBase *parameter = new Parameter<T>(name, value, min, max);
    addParameter(name, parameter);
}

template <typename T>
void GuiPanel::addParameter(string name, T *value)
{
    ParameterBase *parameter = new Parameter<T>(name, value);
    addParameter(name, parameter);
}

template <typename T>
void GuiPanel::addParameter(string name, vector<T> *values, vector<T> min, vector<T> max)
{
    vector<ParameterBase*> parameterGroup;
    for (int i=0; i<min.size(); i++)
    {
        string parameterName = values->size() > 1 ? name+"["+ofToString(i)+"]" : name;
        parameterGroup.push_back(new Parameter<T>(parameterName, &values->at(i), min[i], max[i]));
    }
    addParameter(name, parameterGroup);
}

template <typename T>
void GuiPanel::addParameter(string name, vector<T> *values)
{
    vector<ParameterBase*> parameterGroup;
    for (int i=0; i<values->size(); i++)
    {
        string parameterName = values->size()() > 1 ? name+"["+ofToString(i)+"]" : name;
        parameterGroup.push_back(new Parameter<T>(parameterName, &values->at(i)));
    }
    addParameter<T>(name, parameterGroup);
}

template <typename L, typename M>
void GuiPanel::addParameter(string name, ParameterBase *parameter, L *listener, M method)
{
    GuiWidget *widget = new GuiWidget(name, parameter, listener, method);
    parameters[name] = parameter;
    setupNewWidget(name, widget);
}

template <typename L, typename M>
void GuiPanel::addParameter(string name, vector<ParameterBase*> parameterGroup, L *listener, M method)
{
    GuiWidget *entry = new GuiWidget(name, parameterGroup, listener, method);
    for (int i=0; i<parameterGroup.size(); i++) {
        parameters[parameterGroup[i]->getName()] = parameterGroup[i];
    }
    setupNewWidget(name, entry);
}

template <typename T, typename L, typename M>
void GuiPanel::addParameter(string name, T *value, T min, T max, L *listener, M method)
{
    ParameterBase *parameter = new Parameter<T>(name, value, min, max);
    addParameter(name, parameter, listener, method);
}

template <typename T, typename L, typename M>
void GuiPanel::addParameter(string name, T *value, L *listener, M method)
{
    ParameterBase *parameter = new Parameter<T>(name, value, NULL, NULL);
    addParameter(name, parameter, listener, method);
}

template <typename T, typename L, typename M>
void GuiPanel::addParameter(string name, vector<T> *value, vector<T> min, vector<T> max, L *listener, M method)
{
    vector<ParameterBase*> parameterGroup;
    for (int i=0; i<min.size(); i++)
    {
        string parameterName = min.size() > 1 ? name+"["+ofToString(i)+"]" : name;
        parameterGroup.push_back(new Parameter<T>(parameterName, &value->at(i), min[i], max[i]));
    }
    addParameter(name, parameterGroup, listener, method);
}

template <typename T, typename L, typename M>
void GuiPanel::addParameter(string name, vector<T> *value, L *listener, M method)
{
    vector<ParameterBase*> parameterGroup;
    for (int i=0; i<value->size(); i++)
    {
        string parameterName = value->size()() > 1 ? name+"["+ofToString(i)+"]" : name;
        parameterGroup.push_back(new Parameter<T>(parameterName, &value->at(i)));
    }
    addParameter(name, parameterGroup, listener, method);
}

template <typename L, typename M>
void GuiPanel::addEvent(string name, L *listener, M method)
{
    Parameter<bool> *parameter = new Parameter<bool>(name, new bool());
    GuiWidget *widget = new GuiEvent(name, parameter, listener, method);
    parameters[name] = parameter;
    setupNewWidget(name, widget);
}

template <typename L, typename M>
void GuiPanel::addMenu(string name, vector<string> items, L *listener, M method, bool multipleChoice, bool autoClose)
{
    GuiWidget *widget = new GuiMenu(name, items, multipleChoice, autoClose);
    ofAddListener(((GuiMenu *) widget)->elementEvent, listener, method);
    setupNewWidget(name, widget);
}

template <typename L, typename M>
void GuiPanel::addColor(string name, Parameter<ofFloatColor> *parameter, L *listener, M method)
{
    addParameter(name, parameter, listener, method);
}

template <typename L, typename M>
void GuiPanel::addColor(string name, ofFloatColor *color, L *listener, M method)
{
    addColor(name, new Parameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1)), listener, method);
}

