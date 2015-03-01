#include "Gui2dPad.h"


Gui2dPad::Gui2dPad(Parameter<ofPoint> *parameter) : GuiElement(parameter->getName())
{
    this->parameter = parameter;
    setupPad();
}

Gui2dPad::Gui2dPad(string name, ofPoint *value, ofPoint min, ofPoint max) : GuiElement(name)
{
    parameter = new Parameter<ofPoint>(name, value, min, max);
    setupPad();
}

Gui2dPad::Gui2dPad(string name, ofPoint min, ofPoint max) : GuiElement(name)
{
    parameter = new Parameter<ofPoint>(name, new ofPoint(0.5 * (min.x + max.x), 0.5 * (min.y + max.y)), min, max);
    setupPad();
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
    setValue(ofPoint(ofClamp((parameter->get().x - parameter->getMin().x) / (parameter->getMax().x - parameter->getMin().x), 0.0, 1.0),
                     ofClamp((parameter->get().y - parameter->getMin().y) / (parameter->getMax().y - parameter->getMin().y), 0.0, 1.0)));
    lerpFrame = 0;
    lerpNumFrames = 0;
    elementWidth = 180;
    elementHeight = 180;
    setRectangle(0, 0, elementWidth, elementHeight);
}

void Gui2dPad::setValue(ofPoint padValue)
{
    this->padValue = padValue;
    parameter->set(parameter->getMax() * padValue + parameter->getMin() * (1.0 - padValue));
    updateValueString();
    GuiElementEventArgs args(name, 0, (float) parameter->get().x);
    ofNotifyEvent(elementEvent, args, this);
}

void Gui2dPad::updateValueString()
{
    valueStringNext = "(" + ofToString(parameter->get().x, floor(parameter->get().x) == parameter->get().x ? 0 : 2) + "," + ofToString(parameter->get().y, floor(parameter->get().y) == parameter->get().y ? 0 : 2) + ")";
    toUpdateValueString = true;
}

void Gui2dPad::lerpTo(ofPoint nextValue, int numFrames)
{
    if (numFrames > 1)
    {
        this->lerpNextValue = nextValue;
        this->lerpPrevValue = padValue;
        this->lerpNumFrames = numFrames;
        this->lerpFrame = 0;
    }
    else {
        setValue(nextValue);
    }
}

void Gui2dPad::setValueFromSequence(Sequence &sequence)
{
    //setValue(sequence.getValueAtCurrentCursor());
}

void Gui2dPad::update()
{
    if (lerpFrame < lerpNumFrames)
    {
        float r = (float) lerpFrame / (lerpNumFrames-1);
        setValue(lerpPrevValue * (1.0 - r) + lerpNextValue * r);
        lerpFrame++;
    }
    if (previous != parameter->get())
    {
        this->padValue = ofPoint(ofClamp((parameter->get().x - parameter->getMin().x) / (parameter->getMax().x - parameter->getMin().x), 0.0, 1.0),
                                 ofClamp((parameter->get().y - parameter->getMin().y) / (parameter->getMax().y - parameter->getMin().y), 0.0, 1.0));
        updateValueString();
        previous = parameter->get();
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
    ofSetColor(colorBackground);
    ofSetLineWidth(1);
    ofRect(rectangle);
    
    ofSetColor(colorForeground);
    ofLine(rectangle.x, rectangle.y + padValue.y * rectangle.height, rectangle.x + rectangle.width, rectangle.y + padValue.y * rectangle.height);
    ofLine(rectangle.x + padValue.x * rectangle.width, rectangle.y, rectangle.x + padValue.x * rectangle.width, rectangle.y + rectangle.height);
    ofNoFill();
    ofCircle(rectangle.x + padValue.x * rectangle.width, rectangle.y + padValue.y * rectangle.height, 6);
    
    ofSetColor(colorOutline, 150);
    ofRect(rectangle);
    
    if (mouseOver)
    {
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(colorActive);
        ofRect(rectangle);
        ofSetLineWidth(1);
    }
    
    ofSetColor(colorText);
    ofDrawBitmapString(name, rectangle.x + 2, rectangle.y + 2 + stringHeight);
    ofDrawBitmapString(valueString, rectangle.x + rectangle.width - valueStringWidth - 2, rectangle.y + rectangle.height - 2);
    
    ofPopStyle();
}

bool Gui2dPad::mouseMoved(int mouseX, int mouseY)
{
    return GuiElement::mouseMoved(mouseX, mouseY);
}

bool Gui2dPad::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        setValue(ofPoint(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1),
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
    if (mouseDragging)
    {
        setValue(ofPoint(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1),
                         ofClamp((float)(mouseY - rectangle.y) / rectangle.height, 0, 1)));
    }
    return mouseOver;
}
