#include "GuiSlider.h"


void GuiSliderBase::setValueString(string _valueString)
{
    valueStringNext = _valueString;
    toUpdateValueString = true;
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
    ofSetLineWidth(style.lineWidthActive);
    ofRect(rectangle);

    ofSetColor(style.colorForeground);
    ofRect(rectangle.x,
           rectangle.y,
           rectangle.width * sliderValue,
           rectangle.height);

    ofNoFill();
    ofSetLineWidth(style.lineWidthActive);
    ofSetColor(style.colorForeground);
    ofRect(rectangle);

    if (mouseOver)
    {
        ofNoFill();
        ofSetLineWidth(style.lineWidthActive);
        ofSetColor(style.colorActive);
        ofRect(rectangle);
    }
    
    ofSetColor(style.colorText);
    ofDrawBitmapString(name,
                       rectangle.x + 1,
                       rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);
    ofDrawBitmapString(valueString,
                       rectangle.x + rectangle.width - valueStringWidth - 1,
                       rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);
    
    ofPopStyle();
}

