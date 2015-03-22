#pragma once

#include "ofMain.h"
#include "ofxDelaunay.h"


class PointGenerator
{
public:
    void setBounds(float x, float y, float w, float h);
    void setConvexBounds(vector<ofPoint> bounds);
    virtual void addPoints(int n) { }
    void clear() { points.clear(); }
    vector<ofPoint> &getPoints() { return points; }

protected:
    ofPoint samplePointWithinBounds();
    vector<ofPoint> bounds;
    vector<ofPoint> points;
};


class EvenPointsGenerator : public PointGenerator
{
public:
    void addPoints(int n);
    void setNumberCandidates(int numberCandidates);
private:
    int numberCandidates;
};


class CirclePackGenerator : public PointGenerator
{
public:
    void addPoints(int n);
    void setMinDist(float minDist);
private:
    float minDist;
};


class Triangulator
{
public:
    void triangulate(vector<ofPoint> &points);
    vector<ITRIANGLE> getTriangles() { return triangles; }
    vector<XYZ> getVertices() { return vertices; }
private:    
    ofxDelaunay         triangulation;
    vector<ITRIANGLE>   triangles;
    vector<XYZ>         vertices;
};

