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
    if (sendChangeNotification) {
        GuiButtonEventArgs args(name, value);
        ofNotifyEvent(buttonEvent, args, this);
    }
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
	ofPushMatrix();
	ofPushStyle();
    
	ofFill();
    ofSetLineWidth(lineWidthActive);
	parameter->get() ? ofSetColor(colorFore) : ofSetColor(colorBack);
	ofRect(rectangle);
    
    ofNoFill();
    ofSetLineWidth(lineWidthActive);
    parameter->get() ? ofSetColor(colorBack) : ofSetColor(colorFore);
    ofRect(rectangle);
    
	if (active)
	{
	    ofNoFill();
	    ofSetLineWidth(lineWidthActive);
	    ofSetColor(colorActive);
	    ofRect(rectangle);
	}
    
	ofSetColor(colorText);
	ofDrawBitmapString(name,
                       rectangle.x + 0.5 * (rectangle.width - stringWidth),
                       rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);
	ofPopStyle();
	ofPopMatrix();
}

void GuiButton::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (active)
    {
        setValue(true, true);
    }
}

void GuiButton::mouseReleased(int mouseX, int mouseY)
{
    if (active)
    {
        GuiElement::mouseReleased(mouseX, mouseY);
        setValue(false, true);
    }
}

void GuiToggle::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (active)
    {
        bool value = !parameter->get();
        previous = value;
        setValue(value, true);
    }
}
