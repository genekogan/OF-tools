#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "ofxAudioUnit.h"



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
    
    ofPoint head, ctr, ctr2;
    
    bool auto1 = false;
    
    float d1, d2, d3, d4;
    
    ofxAudioUnit reverb;
	ofxAudioUnitMixer mixer;
	ofxAudioUnitFilePlayer p1, p2, p3, p4;
    
	ofxAudioUnitTap tap;
	ofxAudioUnitOutput output;
	ofPolyline waveform;
    

	
    
    float v1, v2, v3, v4;
    
    ofPoint min, max;
    
    GuiPanel panel;

};

