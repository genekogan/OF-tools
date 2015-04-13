#pragma once

#include "ofMain.h"
#include "Control.h"


class ofApp : public ofBaseApp
{
    
public:
    
    void setup();
    void update();
    void draw();
    
    
    void buttonEvent(GuiButtonEventArgs & e);
    void menuSelect(GuiMenuEventArgs & e);
    void sliderEvent(GuiMultiSliderEventArgs<ofVec2f> & e);
    void colorEvent(GuiColorEventArgs & e);
    void panelToggleEvent(GuiButtonEventArgs & e);
    void multiChoiceMenuSelect(GuiMenuEventArgs & e);
    
    void textBoxEvent(GuiTextBoxEventArgs & e) {
        cout << "text box: " << e.textBox->getName() << " : " << e.value <<endl;
    }
    
    void keyPressed(int key) {
        if (key=='q') {
            //panel.savePreset("/Users/Gene/Desktop/testXml2.xml");
        }
        else if (key=='w') {
            //panel.loadPreset("/Users/Gene/Desktop/testXml2.xml");
        }
        
        else if (key=='1') {
            panel.removeElement("float slider");
        }
        else if (key=='2') {
            cout << "==== KEY 2 ===== " <<endl;
            panel.removeElement("widget inside panel");
            cout << "==== KEY 2 ===== " <<endl;
        }
    }
    
    
    GuiPanel panel;
    GuiWidget widget;
    
    bool button, toggle;
    float floatSlider;
    int intSlider;
    double doubleSlider;
    ofVec4f vec4slider;
    float rangeLow, rangeHigh;
    ofVec2f vec2RangeLow, vec2RangeHigh;
    ofPoint padValue;
    ofFloatColor color, color2;
    ofVec2f vec2slider;
    
    
    OscManager osc;

};
