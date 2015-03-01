#pragma once

#include "ofMain.h"
#include "OpenNI.h"


class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void exit();
    
    void keyPressed(int key);
	
    int id;
    float *x;
    ofPoint *p;
    bool done;
    OpenNI kinect;
    
    ofPoint head, ctr;
    
    
};
