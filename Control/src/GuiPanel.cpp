#include "GuiPanel.h"


GuiPanel::GuiPanel()
{
    setAutoUpdate(true);
    setAutoDraw(true);
}

GuiPanel::~GuiPanel()
{
    clearWidgets();
    setAutoUpdate(false);
    setAutoDraw(false);
}

void GuiPanel::setAutoUpdate(bool autoUpdate)
{
    this->autoUpdate = autoUpdate;
    if (autoUpdate)
    {
        ofAddListener(ofEvents().mouseMoved, this, &GuiPanel::mouseMoved);
        ofAddListener(ofEvents().mousePressed, this, &GuiPanel::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &GuiPanel::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &GuiPanel::mouseReleased);
        ofAddListener(ofEvents().update, this, &GuiPanel::update);
    }
    else
    {
        ofRemoveListener(ofEvents().update, this, &GuiPanel::update);
        ofRemoveListener(ofEvents().mouseMoved, this, &GuiPanel::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &GuiPanel::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &GuiPanel::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &GuiPanel::mouseReleased);    
    }
}

void GuiPanel::setAutoDraw(bool autoDraw)
{
    this->autoDraw = autoDraw;
    if (autoDraw) {
        ofAddListener(ofEvents().draw, this, &GuiPanel::draw);
    }
    else {
        ofRemoveListener(ofEvents().draw, this, &GuiPanel::draw);
    }
}

void GuiPanel::setName(string name)
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
    float maxElementWidth = 0;
    for (auto w : widgets)
    {
        maxElementWidth = max(maxElementWidth, w->getRectangle().width);
        ofPoint position = getGuiPosition(idx++, false);
        w->setPosition(position);
    }
    rectangle.set(rectangle.x, rectangle.y, maxElementWidth + 2 * style.marginInner, getGuiPosition().y - rectangle.y + style.marginInner);
}

void GuiPanel::addParameter(string name, ParameterBase *parameter)
{
    GuiWidget *widget = new GuiWidget(name, parameter);
    parameters[name] = parameter;
    setupNewWidget(name, widget);
}

void GuiPanel::addParameter(string name, vector<ParameterBase*> parameterGroup)
{
    GuiWidget *entry = new GuiWidget(name, parameterGroup);
    for (int i=0; i<parameterGroup.size(); i++) {
        parameters[parameterGroup[i]->getName()] = parameterGroup[i];
    }
    setupNewWidget(name, entry);
}

void GuiPanel::addColor(string name, Parameter<ofFloatColor> *parameter)
{
    GuiWidget *widget = new GuiColor(name, parameter);
    parameters[name] = parameter;
    setupNewWidget(name, widget);
}

void GuiPanel::addColor(string name, ofFloatColor *color)
{
    addColor(name, new Parameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1)));
}

void GuiPanel::setupNewWidget(string widgetName, GuiWidget *widget)
{
    widgetMap[widgetName] = widget;
    widget->setPosition(getGuiPosition());
    widget->setAutoUpdate(false);
    widget->setAutoDraw(false);
    widgets.push_back(widget);
    resetElementPositions();
    ofAddListener(widget->widgetChanged, this, &GuiPanel::receiveWidgetChangedEvent);
}

void GuiPanel::update()
{
    updateWidgets();
}

void GuiPanel::updateWidgets()
{
    map<string, GuiWidget *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it) {
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
    ofSetColor(style.colorBackground, 150);
    ofFill();
    ofRect(rectangle);
    ofSetColor(style.colorBackground, 220);
    ofNoFill();
    ofRect(rectangle);
    ofSetColor(style.colorText);
    ofDrawBitmapString(name, rectangle.x + 6, rectangle.y + 16);
}

void GuiPanel::drawWidgets()
{
    map<string, GuiWidget *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it) {
        it->second->draw();
    }
}

void GuiPanel::mouseMoved(int mouseX, int mouseY)
{
    map<string, GuiWidget *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it) {
        it->second->mouseMoved(mouseX, mouseY);
    }
}

void GuiPanel::mousePressed(int mouseX, int mouseY)
{
    map<string, GuiWidget *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it) {
        it->second->mousePressed(mouseX, mouseY);
    }
}

void GuiPanel::mouseReleased(int mouseX, int mouseY)
{
    map<string, GuiWidget *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it) {
        it->second->mouseReleased(mouseX, mouseY);
    }
}

void GuiPanel::mouseDragged(int mouseX, int mouseY)
{
    map<string, GuiWidget *>::iterator it = widgetMap.begin();
    for (; it != widgetMap.end(); ++it) {
        it->second->mouseDragged(mouseX, mouseY);
    }
}

ofPoint GuiPanel::getGuiPosition(int idxPosition, bool bottom)
{
    ofPoint position(rectangle.x, rectangle.y);
    position.y += GUI_DEFAULT_HEADER_HEIGHT;
    position.y += style.marginInner;
    position.x += style.marginInner;
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

void GuiPanel::clearWidgets()
{
    map<string, ParameterBase *>::iterator itp = parameters.begin();
    map<string, GuiWidget *>::iterator ite = widgetMap.begin();
    for (; itp != parameters.end(); ++itp)  delete itp->second;
    for (; ite != widgetMap.end(); ++ite)     delete ite->second;
    widgetMap.erase(widgetMap.begin(), widgetMap.end());
    parameters.erase(parameters.begin(), parameters.end());
    widgets.clear();
}

void GuiPanel::setMarginInner(float marginInner)
{
    style.marginInner = marginInner;
    resetStyle();
    resetElementPositions();
}

void GuiPanel::setMarginBetween(float marginBetween)
{
    style.marginBetween = marginBetween;
    resetStyle();
    resetElementPositions();
}

void GuiPanel::setElementWidth(float elementWidth)
{
    style.elementWidth = elementWidth;
    resetStyle();
    resetElementPositions();
}

void GuiPanel::setElementHeight(float elementHeight)
{
    style.elementHeight = elementHeight;
    resetStyle();
    resetElementPositions();
}

void GuiPanel::setColorBackground(ofColor colorBackground)
{
    style.colorBackground = colorBackground;
    resetStyle();
}

void GuiPanel::setColorForeground(ofColor colorForeground)
{
    style.colorForeground = colorForeground;
    resetStyle();
}

void GuiPanel::setColorActive(ofColor colorActive)
{
    style.colorActive = colorActive;
    resetStyle();
}

void GuiPanel::setColorText(ofColor colorText)
{
    style.colorText = colorText;
    resetStyle();
}

void GuiPanel::resetStyle()
{
    for (auto w : widgets)
    {
        for (auto e : w->getElements())
        {
            e->getStyle().elementWidth = style.elementWidth;
            e->getStyle().elementHeight = style.elementHeight;
            e->getStyle().marginInner = style.marginInner;
            e->getStyle().marginBetween = style.marginBetween;
            e->getStyle().colorBackground = style.colorBackground;
            e->getStyle().colorForeground = style.colorForeground;
            e->getStyle().colorActive = style.colorActive;
            e->getStyle().colorText = style.colorText;
        }
        w->getStyle().elementWidth = style.elementWidth;
        w->getStyle().elementHeight = style.elementHeight;
        w->getStyle().marginInner = style.marginInner;
        w->getStyle().marginBetween = style.marginBetween;
        w->getStyle().colorBackground = style.colorBackground;
        w->getStyle().colorForeground = style.colorForeground;
        w->getStyle().colorActive = style.colorActive;
        w->getStyle().colorText = style.colorText;
    }
}
