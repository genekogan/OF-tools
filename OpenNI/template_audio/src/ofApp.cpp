#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    panel.setName("vol");
    panel.setPosition(ofGetWidth()-250, 5);
    panel.addParameter("vol1", &v1, 0.0f, 1.0f);
    panel.addParameter("vol2", &v2, 0.0f, 1.0f);
    panel.addParameter("vol3", &v3, 0.0f, 1.0f);
    panel.addParameter("vol4", &v4, 0.0f, 1.0f);

    panel.addParameter("d1", &d1, 0.0f, 1.0f);
    panel.addParameter("d2", &d2, 0.0f, 1.0f);
    panel.addParameter("d3", &d3, 0.0f, 1.0f);
    panel.addParameter("d4", &d4, 0.0f, 1.0f);

    //kinect.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");
    kinect.setup();
    
    kinect.setTrackingUsers(true);
    kinect.setTrackingUserFeatures(true);
    
    
    
    
    p1.setFile("/Users/Gene/audio/FieldRec/India/Coorg/Coorg_water_trickle.mp3");
    p1.connectTo(mixer, 0);
    p2.setFile("/Users/Gene/audio/FieldRec/delaplane VA/fencewires.wav");
    p2.connectTo(mixer, 1);

    p3.setFile("/Users/Gene/audio/FieldRec/germany/woodblocks_in_koblenz.wav");
    p3.connectTo(mixer, 2);

    p4.setFile("/Users/Gene/audio/FieldRec/India/Tzilike_crickets.wav");
    p4.connectTo(mixer, 3);
    


	reverb = ofxAudioUnit('aufx', 'mrev', 'appl');
	

    
    cout << "PARAMETERS " << endl;
    mixer.printParameterList();
	

    vector<AudioUnitParameterInfo> pps =    mixer.getParameterList();
    cout << "size " << pps.size() << endl;
    

    int idx = 0;
    for (auto a : pps) {
        cout << idx << " " << a.name << " ( " << a.minValue << " -> " << a.maxValue <<")"<<endl;
    }
    
    
    mixer.connectTo(reverb).connectTo(output);
	output.start();
	
    p1.loop();
    p2.loop();
    p3.loop();
    p4.loop();
    
    
    min.set(99999, 99999, 999999);
    max.set(-999999, -999999, -99999);
    
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.stop();
}

//--------------------------------------------------------------
void ofApp::update(){
    panel.update();

    //tap.getLeftWaveform(waveform, ofGetWidth(), ofGetHeight());
    
    
    kinect.update();
    
    
    
    
    map<int, OpenNIUser*>::iterator it = kinect.getUsers().begin();
    while (it != kinect.getUsers().end()) {
        head = it->second->getPosition(JOINT_HEAD);
        ctr = it->second->getUser()->getCenter();
        ++it;
        
        
        if (ctr.x > max.x)  max.x = ctr.x;
        if (ctr.x < min.x)  min.x = ctr.x;
        if (ctr.y > max.y)  max.y = ctr.y;
        if (ctr.y < min.y)  min.y = ctr.y;
        if (ctr.z > max.z)  max.z = ctr.z;
        if (ctr.z < min.z)  min.z = ctr.z;
    }
    
    
//    ctr.x = ofMap(ctr.x, -500, 500, 0, 1);
//    ctr.y = ofMap(ctr.y, 0, 1000, 0, 1);
//    ctr.z = ofMap(ctr.z, 0, 2300, 0, 1);
//    cout << "HEAD IS " << ofToString(head) << endl;
    
  
 
    ctr2.x = ofLerp(ctr2.x, ofMap(ctr.x, min.x, max.x, 0, 1), 0.1);
    ctr2.y = ofLerp(ctr2.y, ofMap(ctr.y, min.y, max.y, 0, 1), 0.1);
    ctr2.z = ofLerp(ctr2.z, ofMap(ctr.z, min.z, max.z, 0, 1), 0.1);
    
    
    
    cout << "CTR IS " << ofToString(ctr) << " :::  " << ofToString(ctr2)<<endl;

    
    d1 = ofDist(ctr2.x, ctr2.z, 0.0, 0.5);
    d2 = ofDist(ctr2.x, ctr2.z, 0.0, 1.0);
    d3 = ofDist(ctr2.x, ctr2.z, 1.0, 0.5);
    d4 = ofDist(ctr2.x, ctr2.z, 1.0, 1.0);
    
    cout << "dist " << d1 << " " << d2 << " " << d3 << " " << d4 << endl;
    
    if (auto1){
    v1 = ofClamp(ofMap(d1, 0.45, 0.0, 0.0, 1.0), 0.0, 1.0);
    v2 = ofClamp(ofMap(d2, 0.45, 0.0, 0.0, 1.0), 0.0, 1.0);
    v3 = ofClamp(ofMap(d3, 0.45, 0.0, 0.0, 1.0), 0.0, 1.0);
    v4 = ofClamp(ofMap(d4, 0.45, 0.0, 0.0, 1.0), 0.0, 1.0);
    }
    
    mixer.setInputVolume(v1, 0);
    mixer.setInputVolume(v2, 1);
    mixer.setInputVolume(v3, 2);
    mixer.setInputVolume(v4, 3);

    
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    kinect.draw();

    
    

    panel.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == '1') reverb.showUI();

    if (key=='2') mixer.showUI();
    
    if (key==' ') {
        cout << "MIN " << ofToString(min) << endl;
        cout << "MAX " << ofToString(max) << endl;
    }
    if (key==' ') {
        auto1 = !auto1;
    }
}
