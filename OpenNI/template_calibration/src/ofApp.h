#pragma once

#include "ofMain.h"

#include "ofxSecondWindow.h"
#include "OpenNI.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void exit();
    
    void keyPressed(int key);
	
    OpenNI kinect;
    ofxSecondWindow projector;
};
