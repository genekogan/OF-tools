#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "TimeFunction.h"


class Cube
{
public:
    void setup(ofVec3f mainPosition, ofVec3f marginPosition, ofVec3f targetSize, ofVec3f marginSize, ofVec3f ang, ofVec3f dAng, ofColor color, ofVec3f *pan);
    void draw();
    
private:
    
    ofVec3f mainPosition, marginPosition, wPosition;
    ofVec3f mainSize, targetSize, marginSize, wSize;
    ofVec3f ang, dAng;
    ofVec3f offsetP, offsetS;
    ofColor color;
    ofVec3f *pan;
};


class Cubes : public Scene
{
public:
    Cubes() {setName("Cubes");}
    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);

private:

    void selectPreset(GuiElementEventArgs &evt);
    void preset1();
    void preset2();
    void preset3();
    void preset4();
    void preset5();
    void preset6();
    void preset7();
    
    void resetTimeFunctions();
    void addNewBox();

    int MAXBOXES = 200;
    vector<Cube> cubes;
    ofVec3f panPosition;
    
    TimeFunction<ofVec3f> mainPosition, marginPosition;
    TimeFunction<ofVec3f> targetSize, marginSize;
    TimeFunction<ofVec3f> ang, dAng;
    
    ofVec2f translation;
    ofVec3f pan;
    int newBoxRate;
    ofFloatColor color;
    int colorVar;
};

