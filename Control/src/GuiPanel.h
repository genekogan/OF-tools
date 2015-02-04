#pragma once

#include "ofMain.h"
#include "Parameter.h"
#include "GuiConstants.h"
#include "GuiWidget.h"
#include "GuiColor.h"
#include "GuiMenu.h"


class GuiPanel
{
public:
    GuiPanel();
    ~GuiPanel();
    
    void setActive(bool active);
    
    void setup(string name);
    void update();
    void draw();
    
    void setPosition(int x, int y);
    void setWidth(int width);
    
    void mouseMoved(ofMouseEventArgs &evt);
    void mousePressed(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    
    template <typename T>
    void addParameter(string name, Parameter<T> *parameter, float warp=1.0);
    template <typename T>
    void addParameter(string name, vector<Parameter<T>*> parameterGroup, float warp=1.0);
    template <typename T>
    void addParameter(string name, T *value, T min=NULL, T max=NULL, float warp=1.0);
    template <typename T>
    void addParameter(string name, vector<T> *value, vector<T> min=NULL, vector<T> max=NULL, float warp=1.0);
    
    template <typename L, typename M>
    void addMenu(string name, vector<string> items, L *listener, M method);

    void addColor(string name, Parameter<ofFloatColor> *parameter);
    void addColor(string name, ofFloatColor *color);

    void clearParameters();
    
private:
    
    void receiveWidgetChangedEvent(string &s);
    void resetElementPositions();
    ofPoint getGuiPosition(int idxPosition, bool bottom=true);
    ofPoint getGuiPosition();
    
    void setupNewWidget(string name_, GuiWidgetBase *widget);
    void updateWidgets();
    
    void drawPanel();
    void drawWidgets();
    
    bool active;
    string name;
    ofRectangle rectangle;
    map<string, ParameterBase *> parameters;
    map<string, GuiWidgetBase *> widgetMap;
    vector<GuiWidgetBase *> widgets;
};


template <typename T> void GuiPanel::addParameter(string name, Parameter<T> *parameter, float warp)
{
    GuiWidgetBase *widget = new GuiWidget<T>(name, parameter);
    parameters[name] = parameter;
    setupNewWidget(name, widget);
}

template <typename T> void GuiPanel::addParameter(string name, T *value, T min, T max, float warp)
{
    Parameter<T> *parameter = new Parameter<T>(name, value, min, max, warp);
    addParameter<T>(name, parameter, warp);
}

template <typename T> void GuiPanel::addParameter(string name, vector<Parameter<T>*> parameterGroup, float warp)
{
    GuiWidgetBase *entry = new GuiWidget<T>(name, parameterGroup);
    for (int i=0; i<parameterGroup.size(); i++) {
        parameters[parameterGroup[i]->getName()] = parameterGroup[i];
    }
    setupNewWidget(name, entry);
}

template <typename T> void GuiPanel::addParameter(string name, vector<T> *value, vector<T> min, vector<T> max, float warp)
{
    vector<Parameter<T>*> parameterGroup;
    for (int i=0; i<min.size(); i++) {
        parameterGroup.push_back(new Parameter<T>(name, &value->at(i), min[i], max[i], warp));
    }
    addParameter<T>(name, parameterGroup, warp);
}

template <typename L, typename M> void GuiPanel::addMenu(string name, vector<string> items, L *listener, M method)
{
    GuiWidgetBase *widget = new GuiMenuEntry(name, items);
    ofAddListener(((GuiMenuEntry *) widget)->getMenu()->menuEvent, listener, method);
    setupNewWidget(name, widget);
}
