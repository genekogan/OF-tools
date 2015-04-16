#include "GuiTextBox.h"

GuiTextBox::GuiTextBox(Parameter<string> *parameter) : GuiElement(parameter->getName())
{
    this->parameter = parameter;
    setupTextBox();
}

GuiTextBox::GuiTextBox(string name, string *value) : GuiElement(name)
{
    parameter = new Parameter<string>(name, value);
    setupTextBox();
}

GuiTextBox::GuiTextBox(string name) : GuiElement(name)
{
    parameter = new Parameter<string>(name, new string(name));
    setupTextBox();
}

GuiTextBox::~GuiTextBox()
{
    delete parameter;
    //
    //
    // who should delete parameter?
    //
    //
    //
    //
}

void GuiTextBox::setupTextBox()
{
    setValue(parameter->get());
    stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
    setLeftJustified(true);
}

void GuiTextBox::updateParameterOscAddress()
{
    parameter->setOscAddress(getAddress());
}

void GuiTextBox::setValue(string value, bool sendChangeNotification)
{
    string previous = parameter->get();
    parameter->set(value);
    stringWidth = ofBitmapStringGetBoundingBox(parameter->get(), 0, 0).width;
    if (sendChangeNotification && (value != previous))
    {
        GuiTextBoxEventArgs args(this, parameter->get());
        ofNotifyEvent(textBoxEvent, args, this);
    }
}

void GuiTextBox::getParameters(vector<ParameterBase*> & parameters)
{
    parameters.push_back(parameter);
}

string GuiTextBox::getValue()
{
    return parameter->get();
}

void GuiTextBox::update()
{
    
}

void GuiTextBox::draw()
{
	ofPushStyle();
    
    ofFill();
    ofSetLineWidth(1);
    ofSetColor(colorBackground);
    ofRect(rectangle);
    
    ofNoFill();
    ofSetColor(colorOutline);
    ofRect(rectangle);
    
	if (mouseOver)
	{
	    ofNoFill();
	    ofSetLineWidth(2);
        ofSetColor(editing ? GUI_DEFAULT_COLOR_ACTIVE_EDIT : colorActive);
	    ofRect(rectangle);
        ofSetLineWidth(1);
	}
    
    ofSetColor(colorText);
    
    ofDrawBitmapString(parameter->get(),
                       rectangle.x + (leftJustified ? 4 : 0.5 * (rectangle.width - stringWidth)),
                       rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);
    if (editing)
    {
        int x = rectangle.x + (leftJustified ? 4 : 0.5 * (rectangle.width - stringWidth)) + GUI_DEFAULT_CHAR_WIDTH * cursorPosition;
        ofSetColor(GUI_DEFAULT_COLOR_ACTIVE_EDIT, fmodf(floor(2.0 * ofGetElapsedTimef()), 2.0) == 0.0 ? 255 : 0);
        ofSetLineWidth(1);
        ofLine(x, rectangle.y + 1, x, rectangle.y + rectangle.height - 2);
    }
    
	ofPopStyle();
}

bool GuiTextBox::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        editing = true;
        cursorPosition = ofClamp(round((float) (mouseX - (rectangle.x + (leftJustified ? 4 : 0.5 * (rectangle.width - stringWidth)))) / GUI_DEFAULT_CHAR_WIDTH), 0, parameter->get().length());
        return true;
    }
    return false;
}

bool GuiTextBox::keyPressed(int key)
{
    GuiElement::keyPressed(key);
    
    if (mouseOver && key == OF_KEY_RETURN)
    {
        editing = !editing;
        return true;
    }
    
    if (editing)
    {
        if (key == OF_KEY_BACKSPACE)
        {
            if (cursorPosition > 0)
            {
                cursorPosition = ofClamp(cursorPosition - 1, 0, parameter->get().size());
                setValue(parameter->get().erase(cursorPosition, 1), true);
                return true;
            }
        }
        else if (key == OF_KEY_LEFT)
        {
            cursorPosition = ofClamp(cursorPosition - 1, 0, parameter->get().size());
            return true;
        }
        else if (key == OF_KEY_RIGHT)
        {
            cursorPosition = ofClamp(cursorPosition + 1, 0, parameter->get().size());
            return true;
        }
        else if (key > 31 && key < 127)
        {
            setValue(parameter->get().insert(cursorPosition, 1, key), true);
            cursorPosition++;
            return true;
        }
    }
    return false;
}

void GuiTextBox::getXml(ofXml &xml)
{
    xml.addValue<string>("Value", parameter->get());
}

void GuiTextBox::setFromXml(ofXml &xml)
{
    parameter->set(xml.getValue<string>("Value"));
}
