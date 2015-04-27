#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    projector.setup("projector", ofGetScreenWidth(), 0, 1280, 800, true);
    
    kinect.setup("/Users/gene/Desktop/Parsons/cvVideos/hometest_single.oni");
    kinect.enableCalibration(projector);

    
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.stop();
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    kinect.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}
