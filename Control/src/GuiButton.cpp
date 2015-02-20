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

GuiButtonBase::~GuiButtonBase()
{
//    buttonEvent.clear();
//    buttonEvent.disable();
    
    
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
}

void GuiButtonBase::setValue(bool value, bool sendChangeNotification)
{
    bool valueChanged = (value != parameter->get());
    parameter->set(value);
    if (sendChangeNotification && valueChanged)
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
    if (lerpFrame < lerpNumFrames)
    {
        lerpFrame++;
        if (lerpFrame == lerpNumFrames)
        {
            setValue(lerpNextValue > 0.5, true);
            previous = parameter->get();
        }
    }
    if (previous != parameter->get())
    {
        setValue(parameter->get(), false);
        previous = parameter->get();
    }
}

void GuiButtonBase::draw()
{
	ofPushStyle();
    
    ofFill();
    ofSetLineWidth(GUI_DEFAULT_LINE_WIDTH_ACTIVE);
    parameter->get() ? ofSetColor(style.colorForeground) : ofSetColor(style.colorBackground);
    ofRect(rectangle);
    
    ofNoFill();
    ofSetLineWidth(GUI_DEFAULT_LINE_WIDTH_ACTIVE);
    ofSetColor(style.colorText, 150);
    ofRect(rectangle);

    
	if (mouseOver)
	{
	    ofNoFill();
	    ofSetLineWidth(GUI_DEFAULT_LINE_WIDTH_ACTIVE);
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
