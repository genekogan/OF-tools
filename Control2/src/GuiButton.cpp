#include "GuiButton.h"

GuiButtonBase::GuiButtonBase(string name, Parameter<bool> *parameter) : GuiElement(name)
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

void GuiButtonBase::setupButton()
{
    setValue(parameter->get());
    stringWidth = ofBitmapStringGetBoundingBox(name, 0, 0).width;
    stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
}

void GuiButtonBase::setValue(bool value, bool sendChangeNotification)
{
    parameter->set(value);
    if (sendChangeNotification && value)
    {
        GuiButtonEventArgs args(name, 0, value);
        ofNotifyEvent(buttonEvent, args, this);
    }
}

void GuiButtonBase::setValueFromSequence(Sequence &sequence)
{
    setValue(sequence.getValueAtCurrentIndex() > 0.5, true);
}

void GuiButtonBase::mouseMoved(int mouseX, int mouseY)
{
    GuiElement::mouseMoved(mouseX, mouseY);
}

void GuiButtonBase::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
}

void GuiButtonBase::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
}

void GuiButtonBase::mouseReleased(int mouseX, int mouseY)
{
    GuiElement::mouseReleased(mouseX, mouseY);
}

bool GuiButtonBase::getValue()
{
    return parameter->get();
}

void GuiButtonBase::update()
{
    if (previous != parameter->get())
    {
        setValue(parameter->get());
        previous = parameter->get();
    }
}

void GuiButtonBase::draw()
{
	ofPushStyle();
    
	ofFill();
    ofSetLineWidth(style.lineWidthActive);
	parameter->get() ? ofSetColor(style.colorForeground) : ofSetColor(style.colorBackground);
	ofRect(rectangle);
    
    ofNoFill();
    ofSetLineWidth(style.lineWidthActive);
    parameter->get() ? ofSetColor(style.colorBackground) : ofSetColor(style.colorForeground);
    ofRect(rectangle);
    
	if (mouseOver)
	{
	    ofNoFill();
	    ofSetLineWidth(style.lineWidthActive);
	    ofSetColor(style.colorActive);
	    ofRect(rectangle);
	}
    
    if (stringWidth < rectangle.width)
    {
        ofSetColor(style.colorText);
        ofDrawBitmapString(name,
                           rectangle.x + 0.5 * (rectangle.width - stringWidth),
                           rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);
    }
    
	ofPopStyle();
}

void GuiButton::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        setValue(true, true);
    }
}

void GuiButton::mouseReleased(int mouseX, int mouseY)
{
    if (mouseOver)
    {
        GuiElement::mouseReleased(mouseX, mouseY);
        setValue(false, true);
    }
}

void GuiToggle::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        bool value = !parameter->get();
        previous = value;
        setValue(value, true);
    }
}
