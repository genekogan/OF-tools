#pragma once

#include "ofMain.h"

#include "ofxSecondWindow.h"
#include "OpenNI.h"

#include "ContourRibbons.h"
#include "ContourFluids.h"
#include "ContourStars.h"
#include "ContourBodyMap.h"


class ofApp : public ofBaseApp
{    
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
	
    ofxSecondWindow projector;
    OpenNI kinect;
    
    ContourRibbons ribbons;
    //ContourFluids ribbons;
    //ContourStars ribbons;
    //ContourBodyMap ribbons;
};
