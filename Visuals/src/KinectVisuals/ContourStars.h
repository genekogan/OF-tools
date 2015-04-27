#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "OpenNI.h"
#include "ContourVisual.h"



class ContourStars : public ContourVisual
{
public:
    void setup(int width, int height);
    void update(OpenNI & openNi);
    void draw();
    
private:
    
    struct IdCircle
    {
        ofPtr<ofxBox2dCircle> circle;
        int idx;
        IdCircle(ofPtr<ofxBox2dCircle> circle, int idx) {
            this->circle = circle;
            this->idx = idx;
        }
    };
    
    void clearCircles(GuiButtonEventArgs & evt);
    
    ofxBox2d box2d;
    
    //vector<ofPtr<ofxBox2dCircle> > circles;
    vector<IdCircle*> circles;
    
    vector<ofPolyline> lines;
    vector<ofPtr<ofxBox2dEdge> > edges;
    int rate;
    float tolerance;
    float circleDensity, circleBounce, circleFriction;

    vector<ofImage> img;
    
    ofFloatColor contourColor;
};
