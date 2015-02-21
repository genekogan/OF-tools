#pragma once

#include "ofMain.h"
#include "Control.h"



class ofApp : public ofBaseApp
{
    
public:
    void setup();
    void update();
    void draw();
    
    void menuSelect(GuiElementEventArgs & sel) {
        cout << "menu selection: " << sel.name<< " :: " << sel.value << endl;
    }
    
    GuiPanel panel;
    
    vector<float> floatValues, floatMins, floatMaxs;
    vector<ofVec3f> vec3Values, vec3Mins, vec3Maxs;
    bool boolValue;
    float floatValue;
    ofVec2f vec2;
    ofVec3f vec3;
    ofFloatColor color;
    
};
