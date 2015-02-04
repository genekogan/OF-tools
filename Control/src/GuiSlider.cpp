#include "GuiSlider.h"


void GuiSliderBase::setValueString(string valueString)
{
    this->valueString = valueString;
    valueStringWidth = ofBitmapStringGetBoundingBox(valueString, 0, 0).width;
    stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
}

void GuiSliderBase::draw()
{
    ofPushStyle();
    
    ofFill();
    ofSetColor(colorBack);
    ofSetLineWidth(lineWidthActive);
    ofRect(rectangle);

    ofSetColor(colorFore);
    ofRect(rectangle.x,
           rectangle.y,
           rectangle.width * sliderValue,
           rectangle.height);

    ofNoFill();
    ofSetLineWidth(lineWidthActive);
    ofSetColor(colorFore);
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
                       rectangle.x + 1,
                       rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);
    ofDrawBitmapString(valueString,
                       rectangle.x + rectangle.width - valueStringWidth - 1,
                       rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);
    
    ofPopStyle();
}

