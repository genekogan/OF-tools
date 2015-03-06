#include "Gui2dPad.h"


Gui2dPad::Gui2dPad(Parameter<ofPoint> *parameter) : GuiElement(parameter->getName())
{
    this->min = parameter->getMin();
    this->max = parameter->getMax();
    addPoint(parameter);
    setupPad();
}

Gui2dPad::Gui2dPad(string name, ofPoint *value, ofPoint min, ofPoint max) : GuiElement(name)
{
    this->min = min;
    this->max = max;
    Parameter<ofPoint> *parameter = new Parameter<ofPoint>(name, value, min, max);
    addPoint(parameter);
    setupPad();
}

Gui2dPad::Gui2dPad(string name, ofPoint min, ofPoint max) : GuiElement(name)
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
    elementWidth = 180;
    elementHeight = 180;
    connectPoints = false;
    stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
    setRectangle(0, 0, elementWidth, elementHeight);
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
    for (int i = 0; i < parameters.size(); i++) {
        if (previous[i] != parameters[i]->get()) {
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
    
    ofPushStyle();
    
    ofFill();
    cout << ofToString(colorBackground) << " - bg " << endl;
    ofSetColor(colorBackground);
    ofSetLineWidth(1);
    ofRect(rectangle);
    
    ofSetColor(colorForeground);
    ofNoFill();
    
    if (mouseOver && idxActive != -1)
    {
        ofLine(rectangle.x, rectangle.y + padValue[idxActive].y * rectangle.height, rectangle.x + rectangle.width, rectangle.y + padValue[idxActive].y * rectangle.height);
        ofLine(rectangle.x + padValue[idxActive].x * rectangle.width, rectangle.y, rectangle.x + padValue[idxActive].x * rectangle.width, rectangle.y + rectangle.height);
    }
    for (int i = 0; i < parameters.size(); i++) {
        ofCircle(rectangle.x + padValue[i].x * rectangle.width, rectangle.y + padValue[i].y * rectangle.height, 6);
    }
    
    if (connectPoints)
    {
        ofBeginShape();
        for (int i = 0; i < parameters.size(); i++) {
            ofVertex(rectangle.x + padValue[i].x * rectangle.width, rectangle.y + padValue[i].y * rectangle.height);
        }
        ofEndShape(true);
    }
    
    ofSetColor(colorOutline, 150);
    ofRect(rectangle);
    
    if (mouseOver)
    {
        ofSetLineWidth(2);
        ofSetColor(colorActive);
        ofRect(rectangle);
        ofSetLineWidth(1);

        ofSetColor(colorText);
        ofDrawBitmapString(name, rectangle.x + 2, rectangle.y + 2 + stringHeight);
        if (idxActive != -1) {
            ofDrawBitmapString(valueString, rectangle.x + rectangle.width - valueStringWidth - 2, rectangle.y + rectangle.height - 2);
        }
    }
    
    ofPopStyle();
}

bool Gui2dPad::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
    if (mouseOver)
    {
        choosePoint(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1),
                    ofClamp((float)(mouseY - rectangle.y) / rectangle.height, 0, 1));
    }
    return mouseOver;
}

bool Gui2dPad::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver && idxActive != -1)
    {
        setValue(idxActive,
                 ofPoint(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1),
                         ofClamp((float)(mouseY - rectangle.y) / rectangle.height, 0, 1)));
    }
    return mouseOver;
}

bool Gui2dPad::mouseReleased(int mouseX, int mouseY)
{
    return GuiElement::mouseReleased(mouseX, mouseY);
}

bool Gui2dPad::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
    if (mouseDragging && idxActive != -1)
    {
        setValue(idxActive,
                 ofPoint(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1),
                         ofClamp((float)(mouseY - rectangle.y) / rectangle.height, 0, 1)));
    }
    return mouseOver;
}

bool Gui2dPad::keyPressed(int key)
{
    GuiElement::keyPressed(key);
    if (key == 'n')
    {
        float x = ofMap(ofClamp((float) (ofGetMouseX() - rectangle.x) / rectangle.width,  0, 1), 0, 1, min.x, max.x);
        float y = ofMap(ofClamp((float) (ofGetMouseY() - rectangle.y) / rectangle.height, 0, 1), 0, 1, min.y, max.y);
        addPoint(new ofPoint(x, y));
        idxActive = parameters.size() - 1;
    }
    else if (key == OF_KEY_LEFT && idxActive != -1)
    {
        setValue(idxActive, ofPoint(ofClamp(padValue[idxActive].x - 0.01, 0, 1), padValue[idxActive].y));
    }
    else if (key == OF_KEY_RIGHT && idxActive != -1)
    {
        setValue(idxActive, ofPoint(ofClamp(padValue[idxActive].x + 0.01, 0, 1), padValue[idxActive].y));
    }
    else if (key == OF_KEY_UP && idxActive != -1)
    {
        setValue(idxActive, ofPoint(padValue[idxActive].x, ofClamp(padValue[idxActive].y - 0.01, 0, 1)));
    }
    else if (key == OF_KEY_DOWN && idxActive != -1)
    {
        setValue(idxActive, ofPoint(padValue[idxActive].x, ofClamp(padValue[idxActive].y + 0.01, 0, 1)));
    }
    return mouseOver;
}
