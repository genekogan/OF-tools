#pragma once

#include "ofMain.h"


//#include "ofxOpenNi.h"
//#include "ofxCv.h"
//#include "ofxOpenCv.h"
//#include "ofxKinectFeatures.h"


//#include "Control.h"
#include "OpenNI.h"


//using namespace ofxCv;
//using namespace cv;


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
