#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "ofxSecondWindow.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
    
    ofxSecondWindow projector;
    OpenNI kinect;
    
    ofShader shader;
    ofFbo mask;
    ofFbo final;
    
    ofVideoPlayer player;
};
