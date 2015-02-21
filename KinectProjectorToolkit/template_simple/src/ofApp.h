#pragma once

#include "ofMain.h"

#include "OpenNI.h"
#include "ProjectorCalibration.h"
#include "ofxSecondWindow.h"


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
    
    
    bool done = false;
    bool curved = false;
   
};
