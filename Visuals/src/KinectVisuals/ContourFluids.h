#pragma once

#include "ofMain.h"
#include "ofxFX.h"
#include "ofxFluid.h"
#include "OpenNI.h"



class Contour2
{
public:
    Contour2(vector<ofVec2f> & points, ofPoint center, int label);
    void setPoints(vector<ofVec2f> & points, ofPoint center);
    void draw();
    
    vector<ofVec2f> points;
    ofPoint center;
    int label;
    int age;
    ofColor color;
};


class ContourFluids
{
public:
    void setup(int width, int height);
    void update();
    void draw();
    
    void recordContours(OpenNI & openNi);
    
    GuiPanel & getPanel() {return panel;}

    
private:
    
    GuiPanel panel;

    // tracking
    vector<Contour2 *> contours;
    vector<vector<ofVec2f> > currentContours;
    bool calibrated;
    int width, height;
    
    
    vector<int> labels;
    
    
    ofxFluid fluid;
    vector<vector<ofVec2f> > pContourPoints;
    vector<vector<ofVec2f> > displace;
    float simplify;
    float displaceLerp;
    int numContourPts;
    int skip;
    int maxUsers = 3;
    float dissipation, velDissipation;
    float displacement;
    float strength;
    float gravityX, gravityY;

};