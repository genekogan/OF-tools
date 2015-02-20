#include "GuiWidget.h"


GuiWidget::GuiWidget(string name) : GuiElement(name)
{

}

GuiWidget::GuiWidget(string name, ParameterBase *parameter_) : GuiElement(name)
{
    parameters.push_back(parameter_);
    setupWidget();
}

GuiWidget::GuiWidget(string name, vector<ParameterBase*> & parameters_) : GuiElement(name)
{
    this->parameters = parameters_;
    setupWidget();
}

void GuiWidget::setupWidget()
{
    header = name;
    headerStringHeight = ofBitmapStringGetBoundingBox(header, 0, 0).height;
    isList = parameters.size() > 1;
    
    for (int i=0; i<parameters.size(); i++)
    {
        if      (dynamic_cast<Parameter<bool>*>(parameters[i]) != NULL)
        {
            createElements((Parameter<bool> *) parameters[i]);
        }
        else if (dynamic_cast<Parameter<int>*>(parameters[i]) != NULL)
        {
            createElements((Parameter<int> *) parameters[i]);
        }
        else if (dynamic_cast<Parameter<float>*>(parameters[i]) != NULL)
        {
            createElements((Parameter<float> *) parameters[i]);
        }
        else if (dynamic_cast<Parameter<double>*>(parameters[i]) != NULL)
        {
            createElements((Parameter<double> *) parameters[i]);
        }
        else if (dynamic_cast<Parameter<ofVec2f>*>(parameters[i]) != NULL)
        {
            createElements((Parameter<ofVec2f> *) parameters[i]);
            if (isList && i<parameters.size()-1) elements[elements.size()-1]->setExtraMargin(true);
        }
        else if (dynamic_cast<Parameter<ofVec3f>*>(parameters[i]) != NULL)
        {
            createElements((Parameter<ofVec3f> *) parameters[i]);
            if (isList && i<parameters.size()-1) elements[elements.size()-1]->setExtraMargin(true);
        }
        else if (dynamic_cast<Parameter<ofVec4f>*>(parameters[i]) != NULL)
        {
            createElements((Parameter<ofVec4f> *) parameters[i]);
            if (isList && i<parameters.size()-1) elements[elements.size()-1]->setExtraMargin(true);
        }
        else if (dynamic_cast<Parameter<ofFloatColor>*>(parameters[i]) != NULL)
        {
            createElements((Parameter<ofFloatColor> *) parameters[i]);
            if (isList && i<parameters.size()-1) elements[elements.size()-1]->setExtraMargin(true);
        }
    }
    
    setAutoUpdate(false);
    setAutoDraw(false);
    setCollapsed(false);
    setupGuiComponents();
}

GuiWidget::~GuiWidget()
{
    widgetChanged.clear();
    widgetChanged.disable();
    setAutoUpdate(false);
    setAutoDraw(false);
    for (auto e : elements) {
        delete e;
    }
    elements.clear();
    parameters.clear();
}

vector<GuiElement*> & GuiWidget::getElements()
{
    return elements;
}

void GuiWidget::setAutoUpdate(bool autoUpdate)
{
    GuiElement::setAutoUpdate(autoUpdate);
    for (auto e : elements) {
        e->setAutoUpdate(autoUpdate);
    }
}

void GuiWidget::setAutoDraw(bool autoDraw)
{
    GuiElement::setAutoDraw(autoDraw);
    for (auto e : elements) {
        e->setAutoDraw(autoDraw);
    }
}

void GuiWidget::setupGuiComponents()
{
    ofPoint topLeft = ofPoint(rectangle.x + style.marginInner, rectangle.y + style.marginInner);
    if (isList)
    {
        headerRectangle = ofRectangle(rectangle.x + 2, rectangle.y + 1, rectangle.width - 4, GUI_DEFAULT_LIST_HEADER_HEIGHT);
        topLeft.y += GUI_DEFAULT_LIST_HEADER_HEIGHT + 1 + style.marginInner;
    }
    for (auto e : elements)
    {
        if (collapsed) {
            e->setRectangle(ofRectangle(0, 0, 0, 0));
        }
        else
        {
            e->setRectangle(ofRectangle(topLeft.x, topLeft.y, style.elementWidth, style.elementHeight));
            topLeft += ofPoint(0, style.elementHeight + (e->getExtraMargin() ? 2 * style.marginBetween : style.marginBetween));
        }
    }
    rectangle.width = style.elementWidth + 2 * style.marginInner;
    rectangle.height = collapsed ? topLeft.y - rectangle.y : topLeft.y + 2 * style.marginInner - style.marginBetween - rectangle.y;
}

void GuiWidget::update()
{
    GuiElement::update();
    for (auto e : elements) {
        e->update();
    }
}

void GuiWidget::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    mouseOver = rectangle.inside(mouseX, mouseY);
    headerActive = headerRectangle.inside(mouseX, mouseY);
    //if (mouseOver)
    {
        for (auto e : elements) {
            e->mouseMoved(mouseX, mouseY);
        }
    }
}

void GuiWidget::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        if (isList && headerRectangle.inside(mouseX, mouseY))
        {
            setCollapsed(!collapsed);
            setupGuiComponents();
            ofNotifyEvent(widgetChanged, name, this);
            return;
        }
        for (auto e : elements) {
            e->mousePressed(mouseX, mouseY);
        }
    }
}

void GuiWidget::mouseReleased(int mouseX, int mouseY)
{
    GuiElement::mouseReleased(mouseX, mouseY);
    if (mouseOver)
    {
        for (auto e : elements) {
            e->mouseReleased(mouseX, mouseY);
        }
    }
}

void GuiWidget::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
    if (mouseOver)
    {
        for (auto e : elements) {
            e->mouseDragged(mouseX, mouseY);
        }
    }
}

void GuiWidget::draw()
{
    ofPushStyle();
    
    mouseOver ? ofSetColor(style.colorBackground, 150) : ofSetColor(style.colorBackground, 100);
    ofFill();
    ofSetLineWidth(0);
    ofRect(rectangle);
    
    if (isList)
    {
        ofFill();
        ofSetColor(GUI_DEFAULT_HEADER_COLOR);
        ofRect(headerRectangle);
        if (headerActive)
        {
            ofSetColor(style.colorActive);
            ofNoFill();
            ofRect(headerRectangle);
        }
        ofSetColor(style.colorText);
        ofDrawBitmapString(header,
                           rectangle.x + 4,
                           rectangle.y + 1 + 0.5 * (GUI_DEFAULT_LIST_HEADER_HEIGHT + 0.5 * headerStringHeight));
        ofDrawBitmapString(collapsed ? "+" : "-",
                           rectangle.x + rectangle.width - 16,
                           rectangle.y + 1 + 0.5 * (GUI_DEFAULT_LIST_HEADER_HEIGHT + 0.5 * headerStringHeight));
    }
    
    if (!collapsed)
    {
        for (auto e : elements) {
            e->draw();
        }
    }
    
    ofPopStyle();
}

GuiColor::GuiColor(string name, Parameter<ofFloatColor> *parameter) : GuiWidget(name, parameter)
{
    isList = true;
    collapsed = false;
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
}

GuiColor::GuiColor(string name, ofFloatColor *color): GuiWidget(name, new Parameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1)))
{
    isList = true;
    collapsed = false;
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
}

GuiEvent::GuiEvent(string name, Parameter<bool> *parameter) : GuiWidget(name)
{
    parameters.push_back(parameter);
    GuiElement::setAutoUpdate(true);
    GuiElement::setAutoDraw(true);
    setupWidget();
}

void GuiEvent::setupWidget()
{
    header = name;
    headerStringHeight = ofBitmapStringGetBoundingBox(header, 0, 0).height;
    isList = false;
    collapsed = false;
    GuiButton *button = new GuiButton(parameters[0]->getName(), (Parameter<bool> *) parameters[0]);
    elements.push_back(button);
    setAutoUpdate(false);
    setAutoDraw(false);
    setCollapsed(false);
    setupGuiComponents();

}


















