#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    kinect.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");
    //kinect.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/ml.oni");
    
    kinect.setTrackingUsers(true);
    kinect.setTrackingUserFeatures(true);
}

//--------------------------------------------------------------
void ofApp::exit() {
    
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    
    map<int, OpenNIUser*>::iterator it = kinect.getUsers().begin();
    while (it != kinect.getUsers().end()) {
        head = it->second->getPosition(JOINT_HEAD);
        ctr = it->second->getUser()->getCenter();
        ++it;
    }
    
    ctr.x = ofMap(ctr.x, -500, 500, 0, 1);
    ctr.y = ofMap(ctr.y, 0, 1000, 0, 1);
    ctr.z = ofMap(ctr.z, 0, 2300, 0, 1);

    //cout << "CTR IS " << ofToString(ctr) << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    kinect.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}
