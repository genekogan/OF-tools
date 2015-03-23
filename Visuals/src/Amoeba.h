#pragma once

#include "ofMain.h"
#include "Scene.h"


class Amoeba : public Scene
{
public:
    Amoeba() {setName("Amoeba");}
    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);

private:
    
    void drawAmoeba();
    
    float time;
    int numVertices;
    ofVec2f radRange;
    ofVec2f center;
    float speed;
    float noiseFactor;
    float noiseRegion;
    float offset;
    ofFloatColor colorFill;
    ofFloatColor colorStroke;
    float lineWidth;
    bool filled;
    bool stroked;
    bool curvedVertices;
    int cycleStroke;
    float densityStroke;
    int cycleFill;
    float densityFill;
};