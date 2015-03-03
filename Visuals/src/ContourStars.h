#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "OpenNI.h"




class Contour3
{
public:
    Contour3(vector<ofVec2f> & points, ofPoint center, int label);
    void setPoints(vector<ofVec2f> & points, ofPoint center);
    void draw();
    
    vector<ofVec2f> points;
    ofPoint center;
    int label;
    int age;
    ofColor color;
};




class ContourStars
{
public:
    void setup(int width, int height);
    void update();
    void draw();
    
    void recordContours(OpenNI & openNi);
    
private:
    
    GuiPanel panel;
    
    // tracking
    vector<Contour3 *> contours;
    vector<vector<ofVec2f> > currentContours;
    bool calibrated;
    int width, height;
    
    
    vector<int> labels;
    
    
    
    // physics
    ofxBox2d box2d;
	vector<ofPtr<ofxBox2dCircle> > circles;
	vector<ofPolyline> lines;
    vector<ofPtr<ofxBox2dEdge> > edges;
    int rate;
    float tolerance;
    float circleDensity, circleBounce, circleFriction;
    ofImage img;

    
    
};