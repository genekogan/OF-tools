#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "PointGeneration.h"


class Meshy : public Scene
{
public:
    Meshy() {setName("Meshy");}
    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);

private:
    
    void triangulate(GuiButtonEventArgs &evt);
    
    CirclePackGenerator circlePacker;
    EvenPointsGenerator pointGenerator;
    Triangulator triangulator;
    
    vector<ITRIANGLE> triangles;
    vector<XYZ> vertices;
    vector<ofPoint> points;
    
    bool realtime;
    bool circles;
    bool filled;
    int numCandidates;
    int numPoints;
    float minDist;
    float margin;
    float triNoise;
    float colorNoise;
    ofFloatColor color;
    ofFloatColor varColor;
    float lineWidth;
};



