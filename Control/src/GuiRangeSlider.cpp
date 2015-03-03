#include "GuiRangeSlider.h"


GuiRangeSliderBase::GuiRangeSliderBase(string name) : GuiElement(name)
{
    lerpFrame = 0;
    lerpNumFrames = 0;
}

GuiRangeSliderBase::~GuiRangeSliderBase()
{
    //
    //
    // who should delete parameter?
    //
    //
    //
    //
}

void GuiRangeSliderBase::selectSlider(float sliderValue)
{
    float dLow = abs(sliderValue - sliderLow);
    float dHigh = abs(sliderValue - sliderHigh);
    float dMiddle = abs(sliderValue - (0.5 * (sliderLow + sliderHigh)));

    if (dMiddle < dLow && dMiddle < dHigh)
    {
        selection = MIDDLE;
        sliderValuePrev = sliderValue;
    }
    else {
        selection = dLow < dHigh ? LOW : HIGH;
    }
}

void GuiRangeSliderBase::setValue(float sliderValue)
{
    if (selection == MIDDLE)
    {
        float diff = sliderValue - sliderValuePrev;
        setValueLow(sliderLow + diff);
        setValueHigh(sliderHigh + diff);
        sliderValuePrev = sliderValue;
    }
    else
    {
        if (sliderLow > sliderHigh)
        {
            swap(sliderLow, sliderHigh);
            selection = selection == LOW ? HIGH : LOW;
        }
        selection == LOW ? setValueLow(sliderValue) : setValueHigh(sliderValue);
    }
}

void GuiRangeSliderBase::setValueLow(float sliderLow)
{
    this->sliderLow = sliderLow;
}

void GuiRangeSliderBase::setValueHigh(float sliderHigh)
{
    this->sliderHigh = sliderHigh;
}

void GuiRangeSliderBase::lerpTo(float nextLow, float nextHigh, int numFrames)
{
    if (numFrames > 1)
    {
        this->lerpNextLow = nextLow;
        this->lerpNextHigh = nextHigh;
        this->lerpPrevLow = sliderLow;
        this->lerpPrevHigh = sliderHigh;
        this->lerpNumFrames = numFrames;
        this->lerpFrame = 0;
    }
    else {
        setValueLow(nextLow);
        setValueHigh(nextHigh);
    }
}


void GuiRangeSliderBase::setValueFromSequence(Sequence &sequence)
{
    float diff = sequence.getValueAtCurrentCursor() - sliderValuePrev;
    setValueLow(max(0.0f, sliderLow + diff));
    setValueHigh(min(1.0f, sliderHigh + diff));
    sliderValuePrev = sequence.getValueAtCurrentCursor();
}

void GuiRangeSliderBase::update()
{
    if (lerpFrame < lerpNumFrames)
    {
        setValueLow(ofLerp(lerpPrevLow, lerpNextLow, (float) lerpFrame / (lerpNumFrames-1)));
        setValueHigh(ofLerp(lerpPrevHigh, lerpNextHigh, (float) lerpFrame / (lerpNumFrames-1)));
        lerpFrame++;
    }
}

void GuiRangeSliderBase::draw()
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
    ofRect(rectangle.x + rectangle.width * sliderLow,
           rectangle.y,
           rectangle.width * (sliderHigh - sliderLow),
           rectangle.height);

    
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
    
    ofSetColor(colorText);
    ofDrawBitmapString(name,
                       rectangle.x + 1,
                       rectangle.y + 1 + 0.5 * (rectangle.height + 0.5 * stringHeight));
    ofDrawBitmapString(valueString,
                       rectangle.x + rectangle.width - valueStringWidth - 1,
                       rectangle.y + 1 + 0.5 * (rectangle.height + 0.5 * stringHeight));
    
    ofPopStyle();
}

void GuiRangeSliderBase::keyboardEdit(int key)
{
    if (key == OF_KEY_LEFT)
    {
        decrement();
    }
    else if (key == OF_KEY_RIGHT)
    {
        increment();
    }
}

bool GuiRangeSliderBase::mouseMoved(int mouseX, int mouseY)
{
    return GuiElement::mouseMoved(mouseX, mouseY);
}

bool GuiRangeSliderBase::mousePressed(int mouseX, int mouseY)
{
    GuiElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        float sliderValue = ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1);
        selectSlider(sliderValue);
        setValue(sliderValue);
    }
    return mouseOver;
}

bool GuiRangeSliderBase::mouseReleased(int mouseX, int mouseY)
{
    return GuiElement::mouseReleased(mouseX, mouseY);
}

bool GuiRangeSliderBase::mouseDragged(int mouseX, int mouseY)
{
    GuiElement::mouseDragged(mouseX, mouseY);
    if (mouseDragging)
    {
        setValue(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1));
    }
    return mouseOver;
}

bool GuiRangeSliderBase::keyPressed(int key)
{
    GuiElement::keyPressed(key);
    if (mouseOver)
    {
        float sliderValue = ofClamp((float)(ofGetMouseX() - rectangle.x) / rectangle.width, 0, 1);
        selectSlider(sliderValue);
        keyboardEdit(key);
    }
    return mouseOver;
}
