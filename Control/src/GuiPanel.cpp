#include "GuiPanel.h"


GuiPanel::GuiPanel()
{
    rectangle.set(0, 0, GUI_WIDTH, 0);
    setActive(true);
}

void GuiPanel::setActive(bool active)
{
    if (active)
    {
        ofAddListener(ofEvents().mouseMoved, this, &GuiPanel::mouseMoved);
        ofAddListener(ofEvents().mousePressed, this, &GuiPanel::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &GuiPanel::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &GuiPanel::mouseReleased);
    }
    else
    {
        ofRemoveListener(ofEvents().mouseMoved, this, &GuiPanel::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &GuiPanel::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &GuiPanel::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &GuiPanel::mouseReleased);    
    }
}

void GuiPanel::setup(string name)
{
    this->name = name;
}

void GuiPanel::setPosition(int x, int y)
{
    rectangle.setPosition(x, y);
    resetElementPositions();
}

void GuiPanel::setWidth(int width)
{
    rectangle.setWidth(width);
    resetElementPositions();
}

void GuiPanel::receiveWidgetChangedEvent(string &s)
{
    resetElementPositions();
}

void GuiPanel::resetElementPositions()
{
    int idx = 0;
    for (auto w : widgets)
    {
        ofPoint position = getGuiPosition(idx++, false);
        w->setRectanglePosition(position);
    }
    rectangle.set(rectangle.x, rectangle.y, rectangle.width, getGuiPosition().y - rectangle.y + GUI_MARGIN_OUTER);
}

void GuiPanel::addColor(string name, Parameter<ofFloatColor> *parameter)
{
    GuiWidgetBase *widget = new GuiColorEntry(name, parameter);
    parameters[name] = parameter;
    setupNewWidget(name, widget);
}

void GuiPanel::addColor(string name, ofFloatColor *color)
{
    addColor(name, new Parameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1)));
}

void GuiPanel::setupNewWidget(string name_, GuiWidgetBase *widget)
{
    widgetMap[name_] = widget;
    widget->setRectanglePosition(getGuiPosition());
    widget->setAutoUpdate(false);
    widget->setAutoDraw(false);
    widgets.push_back(widget);
    rectangle.set(rectangle.x, rectangle.y, rectangle.width, getGuiPosition().y - rectangle.y + GUI_MARGIN_OUTER);
    resetElementPositions();
    ofAddListener(widget->widgetChanged, this, &GuiPanel::receiveWidgetChangedEvent);
}

void GuiPanel::update()
{
    updateWidgets();
}

void GuiPanel::updateWidgets()
{
    map<string, GuiWidgetBase *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it)
    {
        it->second->update();
    }
}

void GuiPanel::draw()
{
    drawPanel();
    drawWidgets();
}

void GuiPanel::drawPanel()
{
    ofSetColor(100, 120, 30);
    ofFill();
    ofRect(rectangle);
    ofSetColor(255);
    ofDrawBitmapString(name, rectangle.x + 3, rectangle.y + 16);
}

void GuiPanel::drawWidgets()
{
    map<string, GuiWidgetBase *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it)
    {
        it->second->draw();
    }
}

void GuiPanel::mouseMoved(ofMouseEventArgs &evt)
{
    map<string, GuiWidgetBase *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it)
    {
        it->second->mouseMoved(evt.x, evt.y);
    }
}

void GuiPanel::mousePressed(ofMouseEventArgs &evt)
{
    map<string, GuiWidgetBase *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it)
    {
        it->second->mousePressed(evt.x, evt.y);
    }
}

void GuiPanel::mouseReleased(ofMouseEventArgs &evt)
{
    map<string, GuiWidgetBase *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it)
    {
        it->second->mouseReleased(evt.x, evt.y);
    }
}

void GuiPanel::mouseDragged(ofMouseEventArgs &evt)
{
    map<string, GuiWidgetBase *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it)
    {
        it->second->mouseDragged(evt.x, evt.y);
    }
}

ofPoint GuiPanel::getGuiPosition(int idxPosition, bool bottom)
{
    ofPoint position(rectangle.x, rectangle.y);
    
    // header
    position.y += GUI_HEADER_HEIGHT;
    position.y += GUI_MARGIN_OUTER;
    position.x += GUI_MARGIN_OUTER;
    
    int idx = 0;
    for (auto w : widgets)
    {
        if (bottom)     position.y += w->getRectangle().height;
        if (idx == idxPosition)  return position;
        if (!bottom)    position.y += w->getRectangle().height;
        ++idx;
    }
}

ofPoint GuiPanel::getGuiPosition()
{
    return getGuiPosition(widgetMap.size());
}

void GuiPanel::clearParameters()
{
    map<string, ParameterBase *>::iterator itp = parameters.begin();
    map<string, GuiWidgetBase  *>::iterator ite = widgetMap.begin();
    for (; itp != parameters.end(); ++itp)  delete itp->second;
    for (; ite != widgetMap.end(); ++ite)     delete ite->second;
    widgetMap.erase(widgetMap.begin(), widgetMap.end());
    parameters.erase(parameters.begin(), parameters.end());
    widgets.clear();
}

GuiPanel::~GuiPanel()
{
    clearParameters();
    setActive(false);
}
