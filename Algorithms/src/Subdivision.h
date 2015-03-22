#pragma once

#include "ofMain.h"


class SubdividingPolygon
{
public:
    void setup(vector<ofPoint> points, ofColor color);
    ofColor perturbColor(float dr, float dg, float db);
    void subdivide(int n);
    void draw();
    
private:
    vector<ofPoint> points;
    vector<SubdividingPolygon *> children;
    float totalDist;
    vector<float> pointDist;
    ofColor color;
};
