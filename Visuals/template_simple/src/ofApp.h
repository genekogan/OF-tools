#pragma once

#include "ofMain.h"

#include "ofxSecondWindow.h"

#include "OpenNI.h"
#include "ProjectorCalibration.h"
#include "ContourRibbons.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
	
    OpenNI kinect;
    
    ProjectorCalibration calibration;
    ofxSecondWindow secondWindow;
    
    ContourRibbons ribbons;
    
    bool done = true;
    bool curved = false;
   
};
