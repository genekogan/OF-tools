#pragma once

#include "ofMain.h"
#include "Scene.h"


class CustomScene : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
    void setup(int width, int height);
    void draw(int x, int y);
    
private:
    
    void menuSelect(GuiElementEventArgs & e);
    void multiChoiceMenuSelect(GuiElementEventArgs & e);

    bool filled;
    float radius;
    float lineWidth;
    int resolution;
    ofVec2f position;
    ofFloatColor myColor;
    bool b1, b2, b3;
    string greeting;
    string city;
};


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
	CustomScene custom1;
	CustomScene custom2;
    
    GuiPanel meta;
};
