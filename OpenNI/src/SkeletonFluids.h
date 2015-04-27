#pragma once

#include "ofMain.h"
#include "ofxFX.h"
#include "ofxFluid.h"
#include "OpenNI.h"
#include "SkeletonVisual.h"


class SkeletonFluids : public SkeletonVisual
{
public:
    void setup(int width, int height);
    void update(OpenNI & openNi);
    void draw();
    
private:
    
    ofxFluid fluid;
    vector<vector<ofVec2f> > pContourPoints;
    vector<vector<ofVec2f> > displace;
    float simplify;
    float displaceLerp;
    int numContourPts;
    int skip;
    int maxUsers;
    float dissipation, velDissipation;
    float displacement;
    float strength;
    float gravityX, gravityY;
};