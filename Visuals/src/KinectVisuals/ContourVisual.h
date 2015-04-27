#pragma once

#include "ofMain.h"
#include "OpenNI.h"



class Contour
{
public:
    Contour(vector<ofVec2f> & points, ofPoint center, int label);
    void setPoints(vector<ofVec2f> & points, ofPoint center);
    void draw();
    
    vector<ofVec2f> points;
    ofPoint center;
    int label;
    int age;
    ofColor color;
};


class ContourVisual
{
public:
    virtual void setup(int width, int height);
    virtual void update(OpenNI & openNi);
    virtual void draw();
    
    GuiPanel & getPanel() {return panel;}
    
protected:

    void recordContours(OpenNI & openNi);
    void eraseContours();
    GuiPanel panel;
    
    // tracking
    vector<Contour *> contours;
    vector<int> labels;
    vector<vector<ofVec2f> > currentContours;
    bool calibrated;
    int width, height;
    int contourSmoothness;

};