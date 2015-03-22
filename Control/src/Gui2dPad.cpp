#include "Gui2dPad.h"


Gui2dPad::Gui2dPad(Parameter<ofPoint> *parameter) : GuiWidgetBase(parameter->getName())
{
    this->min = parameter->getMin();
    this->max = parameter->getMax();
    addPoint(parameter);
    setupPad();
}

Gui2dPad::Gui2dPad(string name, ofPoint *value, ofPoint min, ofPoint max) : GuiWidgetBase(name)
{
    this->min = min;
    this->max = max;
    Parameter<ofPoint> *parameter = new Parameter<ofPoint>(name, value, min, max);
    addPoint(parameter);
    setupPad();
}

Gui2dPad::Gui2dPad(string name, ofPoint min, ofPoint max) : GuiWidgetBase(name)
{
    this->min = min;
    this->max = max;
    setupPad();
}

void Gui2dPad::addPoint(Parameter<ofPoint> *parameter)
{
    parameters.push_back(parameter);
    padValue.push_back(ofPoint(ofClamp((parameter->get().x - parameter->getMin().x) / (parameter->getMax().x - parameter->getMin().x), 0.0, 1.0),
                               ofClamp((parameter->get().y - parameter->getMin().y) / (parameter->getMax().y - parameter->getMin().y), 0.0, 1.0)));
    previous.push_back(parameter->get());
    
    for (auto p : parameters)
    {
        if (p->getMin().x < min.x)  min.x = p->getMin().x;
        if (p->getMax().x > max.x)  max.x = p->getMax().x;
        if (p->getMin().y < min.y)  min.y = p->getMin().y;
        if (p->getMax().y > max.y)  max.y = p->getMax().y;
    }
    
    GuiElementEventArgs args(name, parameters.size()-1, (float) parameter->get().x);
    ofNotifyEvent(elementEvent, args, this);
}

void Gui2dPad::addPoint(ofPoint *value)
{
    Parameter<ofPoint> *parameter = new Parameter<ofPoint>(name, value, min, max);
    addPoint(parameter);
}

void Gui2dPad::addPoint()
{
    Parameter<ofPoint> *parameter = new Parameter<ofPoint>(name, new ofPoint(0, 0), min, max);
    addPoint(parameter);
}

void Gui2dPad::removePoint(int idx)
{
    if (idx < 0 || idx >= parameters.size())  return;
    delete parameters[idx];
    parameters.erase(parameters.begin() + idx);
    previous.erase(previous.begin() + idx);
    padValue.erase(padValue.begin() + idx);
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

void Gui2dPad::setupPad()
{
    idxActive = -1;
    lerpFrame = 0;
    lerpNumFrames = 0;
    elementWidth = GUI_DEFAULT_ELEMENT_WIDTH;
    elementHeight = GUI_DEFAULT_ELEMENT_WIDTH;
    connectPoints = false;
    stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
    setRectangle(0, 0, elementWidth, elementHeight);
    setList(true);
}

void Gui2dPad::setupGuiPositions()
{
    if (list)
    {
        headerRectangle = ofRectangle(rectangle.x, rectangle.y, rectangle.width, headerHeight);
        if (collapsed)
        {
            padRectangle.set(0, 0, 0, 0);
            rectangle.set(headerRectangle);
        }
        else
        {
            rectangle.set(rectangle.x, rectangle.y, rectangle.width, elementHeight + headerHeight);
            padRectangle.set(rectangle.x, rectangle.y + headerHeight, rectangle.width, rectangle.height - headerHeight);
        }
    }
    else
    {
        elementHeight = rectangle.height;
        padRectangle = rectangle;
    }
}

void Gui2dPad::setValue(int idx, ofPoint padValue_)
{
    this->padValue[idx] = padValue_;
    parameters[idx]->set(parameters[idx]->getMax() * padValue[idx] + parameters[idx]->getMin() * (1.0 - padValue[idx]));
    updateValueString();
    GuiElementEventArgs args(name, idx, (float) parameters[idx]->get().x);
    ofNotifyEvent(elementEvent, args, this);
}

void Gui2dPad::updateValueString()
{
    if (idxActive == -1)    return;
    valueStringNext = "(" + ofToString(idxActive) + " : " + ofToString(parameters[idxActive]->get().x, floor(parameters[idxActive]->get().x) == parameters[idxActive]->get().x ? 0 : 2) + "," + ofToString(parameters[idxActive]->get().y, floor(parameters[idxActive]->get().y) == parameters[idxActive]->get().y ? 0 : 2) + ")";
    toUpdateValueString = true;
}

void Gui2dPad::lerpTo(int idx, ofPoint nextValue, int numFrames)
{
    if (numFrames > 1)
    {
        this->idxLerp = idx;
        this->lerpNextValue = nextValue;
        this->lerpPrevValue = padValue[idx];
        this->lerpNumFrames = numFrames;
        this->lerpFrame = 0;
    }
    else {
        setValue(idx, nextValue);
    }
}

void Gui2dPad::choosePoint(float x, float y)
{
    float minDist = 2.0;
    idxActive = -1;
    for (int idx = 0; idx < padValue.size(); idx++)
    {
        float padValueDist = ofDist(x, y, padValue[idx].x, padValue[idx].y);
        if (padValueDist < minDist)
        {
            minDist = padValueDist;
            if (minDist < 0.1)
            {
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
    if (lerpFrame < lerpNumFrames)
    {
        float r = (float) lerpFrame / (lerpNumFrames-1);
        setValue(idxLerp, lerpPrevValue * (1.0 - r) + lerpNextValue * r);
        lerpFrame++;
    }
    for (int i = 0; i < parameters.size(); i++)
    {
        if (previous[i] != parameters[i]->get())
        {
            padValue[i] = ofPoint(ofClamp((parameters[i]->get().x - parameters[i]->getMin().x) / (parameters[i]->getMax().x - parameters[i]->getMin().x), 0.0, 1.0),
                                  ofClamp((parameters[i]->get().y - parameters[i]->getMin().y) / (parameters[i]->getMax().y - parameters[i]->getMin().y), 0.0, 1.0));
            updateValueString();
            previous[i] = parameters[i]->get();
        }
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
 
    GuiWidgetBase::draw();
    
    if (!collapsed)
    {
        ofPushStyle();
        
        ofFill();
        ofSetColor(colorBackground);
        ofSetLineWidth(1);
        ofRect(padRectangle);
        
        ofSetColor(colorForeground);
        ofNoFill();
        
        if (mouseOver && idxActive != -1)
        {
            ofLine(padRectangle.x, padRectangle.y + padValue[idxActive].y * padRectangle.height, padRectangle.x + padRectangle.width, padRectangle.y + padValue[idxActive].y * padRectangle.height);
            ofLine(padRectangle.x + padValue[idxActive].x * padRectangle.width, padRectangle.y, padRectangle.x + padValue[idxActive].x * padRectangle.width, padRectangle.y + padRectangle.height);
        }
        for (int i = 0; i < parameters.size(); i++) {
            ofCircle(padRectangle.x + padValue[i].x * padRectangle.width, padRectangle.y + padValue[i].y * padRectangle.height, 6);
        }
        
        if (connectPoints)
        {
            ofBeginShape();
            for (int i = 0; i < parameters.size(); i++) {
                ofVertex(padRectangle.x + padValue[i].x * padRectangle.width, padRectangle.y + padValue[i].y * padRectangle.height);
            }
            ofEndShape(true);
        }
        
        ofSetColor(colorOutline, 150);
        ofRect(padRectangle);
        
        if (mouseOver)
        {
            ofSetLineWidth(2);
            ofSetColor(colorActive);
            ofRect(padRectangle);
            ofSetLineWidth(1);

            ofSetColor(colorText);
            if (!list) {
                ofDrawBitmapString(name, padRectangle.x + 2, padRectangle.y + 2 + stringHeight);
            }
            if (idxActive != -1) {
                ofDrawBitmapString(valueString, padRectangle.x + padRectangle.width - valueStringWidth - 2, padRectangle.y + padRectangle.height - 2);
            }
        }
        
        ofPopStyle();
    }
}

bool Gui2dPad::mouseMoved(int mouseX, int mouseY)
{
    GuiWidgetBase::mouseMoved(mouseX, mouseY);
    mouseOverPad = padRectangle.inside(mouseX, mouseY);
    if (mouseOverPad)
    {
        choosePoint(ofClamp((float)(mouseX - padRectangle.x) / padRectangle.width, 0, 1),
                    ofClamp((float)(mouseY - padRectangle.y) / padRectangle.height, 0, 1));
    }
    return mouseOverPad;
}

bool Gui2dPad::mousePressed(int mouseX, int mouseY)
{
    GuiWidgetBase::mousePressed(mouseX, mouseY);
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
    return GuiWidgetBase::mouseReleased(mouseX, mouseY);
}

bool Gui2dPad::mouseDragged(int mouseX, int mouseY)
{
    GuiWidgetBase::mouseDragged(mouseX, mouseY);
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
    GuiWidgetBase::keyPressed(key);
    if (key == 'n')
    {
        float x = ofMap(ofClamp((float) (ofGetMouseX() - padRectangle.x) / padRectangle.width,  0, 1), 0, 1, min.x, max.x);
        float y = ofMap(ofClamp((float) (ofGetMouseY() - padRectangle.y) / padRectangle.height, 0, 1), 0, 1, min.y, max.y);
        addPoint(new ofPoint(x, y));
        idxActive = parameters.size() - 1;
        return true;
    }
    else if (key == OF_KEY_BACKSPACE)
    {
        removePoint(idxActive);
        idxActive = -1;
        return true;
    }
    else if (key == OF_KEY_LEFT && idxActive != -1)
    {
        setValue(idxActive, ofPoint(ofClamp(padValue[idxActive].x - 0.01, 0, 1), padValue[idxActive].y));
        return true;
    }
    else if (key == OF_KEY_RIGHT && idxActive != -1)
    {
        setValue(idxActive, ofPoint(ofClamp(padValue[idxActive].x + 0.01, 0, 1), padValue[idxActive].y));
        return true;
    }
    else if (key == OF_KEY_UP && idxActive != -1)
    {
        setValue(idxActive, ofPoint(padValue[idxActive].x, ofClamp(padValue[idxActive].y - 0.01, 0, 1)));
        return true;
    }
    else if (key == OF_KEY_DOWN && idxActive != -1)
    {
        setValue(idxActive, ofPoint(padValue[idxActive].x, ofClamp(padValue[idxActive].y + 0.01, 0, 1)));
        return true;
    }
    return false;
}
