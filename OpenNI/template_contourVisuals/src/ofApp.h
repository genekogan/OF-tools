#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "ofxSecondWindow.h"
#include "ContourRibbons.h"
#include "ContourStars.h"
#include "ContourFluids.h"
#include "ContourBackgrounding.h"
#include "SkeletonVisual.h"
#include "SkeletonRivers.h"
#include "SkeletonFluids.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
    
    ofxSecondWindow projector;
    OpenNI kinect;
    
    ContourRibbons ribbons;
    ContourFluids fluids;
    ContourStars stars;
    ContourBackgrounding backgrounding;
    
    SkeletonVisual skeleton;
    SkeletonFluids skeletonRivers;

    int mode;
};
