#pragma once

#include "ofMain.h"
#include "OpenNI.h"



class Contour4
{
public:
    Contour4(vector<ofVec2f> & points, ofPoint center, int label);
    void setPoints(vector<ofVec2f> & points, ofPoint center);
    void draw();
    
    vector<ofVec2f> points;
    ofPoint center;
    int label;
    int age;
    ofColor color;
};


class ContourBodyMap
{
public:
    void setup(int width, int height);
    void update();
    void draw();
    
    void recordContours(OpenNI & openNi);
    
private:
    
    GuiPanel panel;
    
    
    // tracking
    vector<Contour4 *> contours;
    vector<vector<ofVec2f> > currentContours;
    bool calibrated;
    int width, height;
    vector<int> labels;

    
    ofShader shader;
    ofFbo gfx;
    ofFbo mask;
    ofFbo final;    
    ofVideoPlayer player;


};