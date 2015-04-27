#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    kinect.setup();//"/Users/gene/Downloads/hometest_single.oni");
    window.setup("projector", 800, 20, 800, 600, false);
    
    kinect.enableCalibration(window);
    kinect.setTrackingUsers(true);
    kinect.setTrackingUserFeatures(true);
    
    
    skels.push_back(new SkeletonSender("/live/tempo", -1, -1, 4, 999));
//    skels.push_back(new SkeletonSender("/theaddress2", -1, -1, 5, 20));
//    skels.push_back(new SkeletonSender("/theaddress3", 2, -1, 0, 10));
    

    for (int i=0; i<17; i++){
        joints.push_back(new JointTracker());
        
    }
    
    
    send.setup("192.168.1.12", 9000);

    
    skels[0]->setJoint(joints[0], 2);

    
}

//--------------------------------------------------------------
void ofApp::exit() {
    
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    
    map<int, OpenNIUser*>::iterator it = kinect.getUsers().begin();
    while (it != kinect.getUsers().end())
    {
        head = it->second->getPosition(JOINT_HEAD);
        for (int j=0; j<17; j++) {
            joints[j]->set(it->second->getPosition((Joint) j), trackMinMax, toNormalize);
        }


        ++it;
    }
    
    
    
    
    
    
    vector<SkeletonSender*>::iterator its = skels.begin();
    while (its != skels.end()) {
        if ((*its)->hasJoint) {
            ofxOscMessage m;
            (*its)->setupMessage(m);
            send.sendMessage(m);
        }
        
        ++its;
    }
    
    
    /*
    ofxOscMessage m;
    m.setAddress("/live/tempo");
    m.addFloatArg(ofMap(ofGetMouseX(), 0, ofGetWidth(), 4, 999));
    send.sendMessage(m);
*/
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    kinect.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}
