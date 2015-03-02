#include "GuiButton.h"

GuiButtonBase::GuiButtonBase(Parameter<bool> *parameter) : GuiElement(parameter->getName())
{
    this->parameter = parameter;
    setupButton();
}

GuiButtonBase::GuiButtonBase(string name, bool *value) : GuiElement(name)
{
    parameter = new Parameter<bool>(name, value);
    setupButton();
}

GuiButtonBase::GuiButtonBase(string name) : GuiElement(name)
{
    parameter = new Parameter<bool>(name, new bool());
    setupButton();
}

GuiButtonBase::~GuiButtonBase()
{
    //
    //
    // who should delete parameter?
    //
    //
    //
    //
}

void GuiButtonBase::setupButton()
{
    setValue(parameter->get());
    stringWidth = ofBitmapStringGetBoundingBox(name, 0, 0).width;
    stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
    setTriggerAll(false);
}

void GuiButtonBase::setTriggerAll(bool triggerAll)
{
    this->triggerAll = triggerAll;
}

void GuiButtonBase::setValue(bool value, bool sendChangeNotification)
{
    bool previous = parameter->get();
    parameter->set(value);
    if (sendChangeNotification && ((value != previous) || triggerAll))
    {
        GuiElementEventArgs args(name, 0, value ? 1.0 : 0.0);
        ofNotifyEvent(elementEvent, args, this);
    }
}

void GuiButtonBase::lerpTo(float nextValue, int numFrames)
{
    this->lerpNextValue = nextValue;
    this->lerpNumFrames = numFrames;
    lerpFrame = 0;
}

void GuiButtonBase::setValueFromSequence(Sequence &sequence)
{
    setValue(sequence.getValueAtCurrentIndex() > 0.5, true);
}

bool GuiButtonBase::getValue()
{
    return parameter->get();
}

void GuiButtonBase::update()
{
    if (lerpFrame < lerpNumFrames)
    {
        lerpFrame++;
        if (lerpFrame == lerpNumFrames) {
            setValue(lerpNextValue > 0.5, true);
        }
    }
}

void GuiButtonBase::draw()
{
	ofPushStyle();
    
    ofFill();
    ofSetLineWidth(1);
    parameter->get() ? ofSetColor(colorForeground) : ofSetColor(colorBackground);
    ofRect(rectangle);
    
    ofNoFill();
    ofSetColor(colorOutline);
    ofRect(rectangle);
    
	if (mouseOver)
	{
	    ofNoFill();
	    ofSetLineWidth(2);
	    ofSetColor(colorActive);
	    ofRect(rectangle);
        ofSetLineWidth(1);
	}
    
    if (stringWidth < rectangle.width)
    {
        ofSetColor(colorText);
        ofDrawBitmapString(name,
                           rectangle.x + 0.5 * (rectangle.width - stringWidth),
                           rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);
    }
    
	ofPopStyle();
}

bool GuiButton::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        setValue(true, true);
        return true;
    }
    return false;
}

bool GuiButton::mouseReleased(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        GuiElement::mouseReleased(mouseX, mouseY);
        setValue(false, false);
        return true;
    }
    return false;
}

bool GuiToggle::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        bool value = !parameter->get();
        setValue(value, true);
        return true;
    }
    return false;
}

bool GuiToggle::keyPressed(int key)
{
    GuiElement::keyPressed(key);
    if (mouseOver && key==' ')
    {
        bool value = !parameter->get();
        setValue(value, true);
        return true;
    }
    return false;
}