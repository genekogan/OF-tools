#include "Gui2dPad.h"


Gui2dPadPoint::Gui2dPadPoint(Parameter<ofPoint> *parameter)
{
    this->parameter = parameter;
    padValue.set(ofClamp((parameter->get().x - parameter->getMin().x) / (parameter->getMax().x - parameter->getMin().x), 0.0, 1.0),
                 ofClamp((parameter->get().y - parameter->getMin().y) / (parameter->getMax().y - parameter->getMin().y), 0.0, 1.0));
    previous.set(parameter->get());
    lerpFrame = 0;
    lerpNumFrames = 0;
    lerpPrevValue = padValue;
    lerpNextValue = padValue;
}

Gui2dPadPoint::~Gui2dPadPoint()
{
    delete parameter;
}

void Gui2dPadPoint::setValue(ofPoint padValue)
{
    this->padValue = padValue;
    parameter->set(parameter->getMax() * padValue + parameter->getMin() * (1.0 - padValue));
}

void Gui2dPadPoint::lerpTo(ofPoint nextValue, int numFrames)
{
    if (numFrames > 1)
    {
        lerpNextValue = nextValue;
        lerpPrevValue = padValue;
        lerpNumFrames = numFrames;
        lerpFrame = 0;
    }
    else {
        setValue(nextValue);
    }
}

void Gui2dPadPoint::update()
{
    if (lerpFrame < lerpNumFrames)
    {
        float r = (float) lerpFrame / (lerpNumFrames-1);
        setValue(lerpPrevValue * (1.0 - r) + lerpNextValue * r);
        lerpFrame++;
    }
    if (previous != parameter->get())
    {
        padValue.set(ofClamp((parameter->get().x - parameter->getMin().x) / (parameter->getMax().x - parameter->getMin().x), 0.0, 1.0),
                     ofClamp((parameter->get().y - parameter->getMin().y) / (parameter->getMax().y - parameter->getMin().y), 0.0, 1.0));
        previous.set(parameter->get());
    }
}

void Gui2dPadPoint::increment(float x, float y)
{
    setValue(ofPoint(ofClamp(padValue.x + x, 0, 1),
                     ofClamp(padValue.y + y, 0, 1)));
}

string Gui2dPadPoint::getValueString()
{
    return ofToString(parameter->get().x, floor(parameter->get().x) == parameter->get().x ? 0 : 2) + "," + ofToString(parameter->get().y, floor(parameter->get().y) == parameter->get().y ? 0 : 2) + ")";
}

Gui2dPad::Gui2dPad(Parameter<ofPoint> *parameter) : GuiMultiElement(parameter->getName())
{
    setupPad(parameter->getMin(), parameter->getMax());
    addPoint(parameter);
}

Gui2dPad::Gui2dPad(string name, ofPoint *value, ofPoint min, ofPoint max) : GuiMultiElement(name)
{
    setupPad(min, max);
    addPoint(new Parameter<ofPoint>(name, value, min, max));
}

Gui2dPad::Gui2dPad(string name, ofPoint min, ofPoint max) : GuiMultiElement(name)
{
    setupPad(min, max);
}

void Gui2dPad::setMin(ofPoint min)
{
    this->min = min;
    setupGuiPositions();
}

void Gui2dPad::setMax(ofPoint max)
{
    this->max = max;
    setupGuiPositions();
}

Gui2dPadPoint * Gui2dPad::addPoint(Parameter<ofPoint> *parameter)
{
    Gui2dPadPoint *newPoint = new Gui2dPadPoint(parameter);
    points.push_back(newPoint);
    if (parameter->getMin().x < min.x)  min.x = parameter->getMin().x;
    if (parameter->getMax().x > max.x)  max.x = parameter->getMax().x;
    if (parameter->getMin().y < min.y)  min.y = parameter->getMin().y;
    if (parameter->getMax().y > max.y)  max.y = parameter->getMax().y;
    
    Gui2dPadEventArgs args(newPoint, newPoint->padValue);
    ofNotifyEvent(padEvent, args, this);
    return newPoint;
}

Gui2dPadPoint * Gui2dPad::addPoint(ofPoint *value)
{
    return addPoint(new Parameter<ofPoint>(name, value, min, max));
}

Gui2dPadPoint * Gui2dPad::addPoint()
{
    return addPoint(new Parameter<ofPoint>(name, new ofPoint(0, 0), min, max));
}

void Gui2dPad::removePoint(int idx)
{
    if (idx < 0 || idx >= points.size())  return;
    delete points[idx];
    points.erase(points.begin() + idx);
}

Gui2dPad::~Gui2dPad()
{
    //
    //
    // who should delete parameter?
    //
    //
    //
    //
}

void Gui2dPad::setParent(GuiElement *parent)
{
    this->parent = parent;
    hasParent = true;
    setCollapsible(true);
}

void Gui2dPad::setupPad(ofPoint min, ofPoint max)
{
    setMin(min);
    setMax(max);
    idxActive = -1;
    setCollapsible(false);
    setHeight(width);
    setDrawConnectedPoints(false);
    stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
}

void Gui2dPad::updateParameterOscAddress()
{
    int idx = 0;
    for (auto p : points) {
        p->parameter->setOscAddress(getAddress()+"/"+ofToString(idx++));
    }
}

void Gui2dPad::setupGuiPositions()
{
    if (collapsible)
    {
        headerRectangle.set(x, y, width, headerHeight);
        if (getCollapsed())
        {
            padRectangle.set(0, 0, 0, 0);
            rectangle.set(x, y, width, headerHeight);
        }
        else
        {
            padRectangle.set(x + marginX, y + headerHeight + marginY, getWidth() - 2 * marginX, getHeight() - headerHeight - 2 * marginY);
            rectangle.set(x, y, width, height);
        }
    }
    else
    {
        headerRectangle.set(0, 0, 0, 0);
        padRectangle.set(rectangle);
    }
}

void Gui2dPad::addElementToTouchOscLayout(TouchOscPage *page, float *y)
{

}

void Gui2dPad::setValue(int idx, ofPoint padValue)
{
    points[idx]->setValue(padValue);
    updateValueString();
    Gui2dPadEventArgs args(points[idx], padValue);
    ofNotifyEvent(padEvent, args, this);
}

void Gui2dPad::updateValueString()
{
    if (idxActive == -1)    return;
    valueStringNext = "(" + ofToString(idxActive) + " : " + points[idxActive]->getValueString();
    toUpdateValueString = true;
}

void Gui2dPad::getParameters(vector<ParameterBase*> & parameters)
{
    for (auto p : points) {
        parameters.push_back(p->parameter);
    }
}

void Gui2dPad::lerpTo(int idx, ofPoint nextValue, int numFrames)
{
    points[idx]->lerpTo(nextValue, numFrames);
}

void Gui2dPad::selectPoint(float x, float y)
{
    float minDist = 2.0;
    idxActive = -1;
    for (int idx = 0; idx < points.size(); idx++)
    {
        float padValueDist = ofDist(x, y, points[idx]->padValue.x, points[idx]->padValue.y);
        if (padValueDist < minDist)
        {
            minDist = padValueDist;
            if (minDist < 0.1) {
                idxActive = idx;
            }
        }
    }
    if (idxActive != -1) {
        updateValueString();
    }
}

void Gui2dPad::update()
{
    for (auto p : points) {
        p->update();
    }
}

void Gui2dPad::draw()
{
    if (toUpdateValueString)
    {
        valueString = valueStringNext;
        valueStringWidth = ofBitmapStringGetBoundingBox(valueString, 0, 0).width;
        stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
        toUpdateValueString = false;
    }
    
    GuiMultiElement::draw();
    
    ofPushStyle();

    ofSetColor(colorBackground);
    ofFill();
    ofSetLineWidth(1);
    ofRect(padRectangle);
    
    ofSetColor(colorForeground);
    ofNoFill();
    
    if (mouseOverPad && idxActive != -1)
    {
        ofLine(padRectangle.x, padRectangle.y + points[idxActive]->padValue.y * padRectangle.height, padRectangle.x + padRectangle.width, padRectangle.y + points[idxActive]->padValue.y * padRectangle.height);
        ofLine(padRectangle.x + points[idxActive]->padValue.x * padRectangle.width, padRectangle.y, padRectangle.x + points[idxActive]->padValue.x * padRectangle.width, padRectangle.y + padRectangle.height);
    }
    for (auto p : points) {
        ofCircle(padRectangle.x + p->padValue.x * padRectangle.width, padRectangle.y + p->padValue.y * padRectangle.height, 6);
    }
    
    if (connectPoints)
    {
        ofBeginShape();
        for (auto p : points) {
            ofVertex(padRectangle.x + p->padValue.x * padRectangle.width, padRectangle.y + p->padValue.y * padRectangle.height);
        }
        ofEndShape(true);
    }
    
    ofSetColor(colorOutline, 150);
    ofRect(padRectangle);
    
    if (mouseOverPad)
    {
        ofSetLineWidth(2);
        ofSetColor(colorActive);
        ofRect(padRectangle);
        ofSetLineWidth(1);
        
        ofSetColor(colorText);
        if (!collapsible) {
            ofDrawBitmapString(display, padRectangle.x + 2, padRectangle.y + 2 + stringHeight);
        }
        if (idxActive != -1) {
            ofDrawBitmapString(valueString, padRectangle.x + padRectangle.width - valueStringWidth - 2, padRectangle.y + padRectangle.height - 2);
        }
    }
    
    ofPopStyle();
}

bool Gui2dPad::mouseMoved(int mouseX, int mouseY)
{
    GuiMultiElement::mouseMoved(mouseX, mouseY);
    mouseOverPad = padRectangle.inside(mouseX, mouseY);
    if (mouseOverPad)
    {
        selectPoint(ofClamp((float)(mouseX - padRectangle.x) / padRectangle.width, 0, 1),
                    ofClamp((float)(mouseY - padRectangle.y) / padRectangle.height, 0, 1));
    }
    return mouseOverPad;
}

bool Gui2dPad::mousePressed(int mouseX, int mouseY)
{
    GuiMultiElement::mousePressed(mouseX, mouseY);
    if (mouseOverPad && idxActive != -1)
    {
        setValue(idxActive,
                 ofPoint(ofClamp((float)(mouseX - padRectangle.x) / padRectangle.width, 0, 1),
                         ofClamp((float)(mouseY - padRectangle.y) / padRectangle.height, 0, 1)));
    }
    return mouseOverPad;
}

bool Gui2dPad::mouseReleased(int mouseX, int mouseY)
{
    return GuiMultiElement::mouseReleased(mouseX, mouseY);
}

bool Gui2dPad::mouseDragged(int mouseX, int mouseY)
{
    GuiMultiElement::mouseDragged(mouseX, mouseY);
    if (mouseDragging && idxActive != -1)
    {
        setValue(idxActive,
                 ofPoint(ofClamp((float)(mouseX - padRectangle.x) / padRectangle.width, 0, 1),
                         ofClamp((float)(mouseY - padRectangle.y) / padRectangle.height, 0, 1)));
    }
    return mouseOverPad;
}

bool Gui2dPad::keyPressed(int key)
{
    if (mouseOverPad)
    {
        if (key == 'n')
        {
            float x = ofMap(ofClamp((float) (ofGetMouseX() - padRectangle.x) / padRectangle.width,  0, 1), 0, 1, min.x, max.x);
            float y = ofMap(ofClamp((float) (ofGetMouseY() - padRectangle.y) / padRectangle.height, 0, 1), 0, 1, min.y, max.y);
            addPoint(new ofPoint(x, y));
            idxActive = points.size() - 1;
            return true;
        }
        else if (idxActive != -1)
        {
            if (key == OF_KEY_BACKSPACE)
            {
                removePoint(idxActive);
                idxActive = -1;
                return true;
            }
            else if (key == OF_KEY_LEFT)
            {
                points[idxActive]->increment(-0.01, 0.0);
                return true;
            }
            else if (key == OF_KEY_RIGHT)
            {
                points[idxActive]->increment(+0.01, 0.0);
                return true;
            }
            else if (key == OF_KEY_UP)
            {
                points[idxActive]->increment(0.0, -0.01);
                return true;
            }
            else if (key == OF_KEY_DOWN)
            {
                points[idxActive]->increment(0.0, +0.01);
                return true;
            }
        }
    }
    return false;
}

void Gui2dPad::getXml(ofXml &xml)
{
    xml.addValue<ofPoint>("Min", min);
    xml.addValue<ofPoint>("Max", max);
    xml.addChild("Points");
    xml.setTo("Points");
    for (auto p : points) {
        xml.addValue<ofPoint>("Point", p->padValue);
    }
    xml.setToParent();
}

void Gui2dPad::setFromXml(ofXml &xml)
{
    setMin(xml.getValue<ofPoint>("Min"));
    setMax(xml.getValue<ofPoint>("Max"));

    xml.setTo("Points");
    if (xml.exists("Point[0]"))
    {
        int idx = 0;
        do {
            ofPoint pt = xml.getValue<ofPoint>("Point["+ofToString(idx)+"]");
            if (idx < points.size()) {
                points[idx]->setValue(pt);
            }
            else
            {
                Gui2dPadPoint * newPoint = addPoint();
                newPoint->setValue(pt);
            }
            idx++;
        }
        while(idx < xml.getNumChildren());
    }
    xml.setToParent();
}
