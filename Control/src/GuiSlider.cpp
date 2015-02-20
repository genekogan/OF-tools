#include "GuiSlider.h"


GuiSliderBase::GuiSliderBase(string name) : GuiElement(name)
{
    lerpFrame = 0;
    lerpNumFrames = 0;
}

GuiSliderBase::~GuiSliderBase()
{
//    sliderEvent.clear();
//    sliderEvent.disable();

    
    //
    //
    // who should delete parameter?
    //
    //
    //
    //
}

void GuiSliderBase::setValue(float sliderValue)
{
    this->sliderValue = sliderValue;
}

void GuiSliderBase::setValueString(string _valueString)
{
    this->valueStringNext = _valueString;
    toUpdateValueString = true;
}

void GuiSliderBase::lerpTo(float nextValue, int numFrames)
{
    if (numFrames > 1)
    {
        this->lerpNextValue = nextValue;
        this->lerpPrevValue = sliderValue;
        this->lerpNumFrames = numFrames;
        this->lerpFrame = 0;
    }
    else {
        setValue(nextValue);
    }
}

void GuiSliderBase::setValueFromSequence(Sequence &sequence)
{
    setValue(sequence.getValueAtCurrentCursor());
}

void GuiSliderBase::update()
{
    if (lerpFrame < lerpNumFrames)
    {
        setValue(ofLerp(lerpPrevValue, lerpNextValue, (float) lerpFrame / (lerpNumFrames-1)));
        lerpFrame++;
    }
}

void GuiSliderBase::draw()
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
    ofSetColor(style.colorBackground);
    ofSetLineWidth(GUI_DEFAULT_LINE_WIDTH_ACTIVE);
    ofRect(rectangle);

    ofSetColor(style.colorForeground);
    ofRect(rectangle.x,
           rectangle.y,
           rectangle.width * sliderValue,
           rectangle.height);

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
    
    ofSetColor(style.colorText);
    ofDrawBitmapString(name,
                       rectangle.x + 1,
                       rectangle.y + 1 + 0.5 * (rectangle.height + 0.5 * stringHeight));
    ofDrawBitmapString(valueString,
                       rectangle.x + rectangle.width - valueStringWidth - 1,
                       rectangle.y + 1 + 0.5 * (rectangle.height + 0.5 * stringHeight));
    
    ofPopStyle();
}

