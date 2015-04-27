#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    projector.setup("projector", 800, 20, 800, 600, false);
    kinect.setup("/Users/gene/Downloads/hometest_single.oni");
    kinect.enableCalibration(projector);
    kinect.loadCalibration("/Users/gene/Code/of_v0.8-4.4_osx_release/addons/ofxKinectProjectorToolkit/calibration/bin/data/calibration.xml");
    
    player.loadMovie("/Users/gene/Media/german_train_grid.mov");
    player.setLoopState(OF_LOOP_NORMAL);
    player.play();
    
    mask.allocate(projector.getWidth(), projector.getHeight());
    final.allocate(projector.getWidth(), projector.getHeight());
    shader.load("standard.vert", "alphamask.frag");
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.stop();
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    player.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    kinect.draw();
    
    // update mask
    mask.begin();
    ofClear(0, 0);
    for (int i=0; i<kinect.getNumContours(); i++)
    {
        vector<ofVec2f> calibratedPoints;
        kinect.getCalibratedContour(i, calibratedPoints, projector.getWidth(), projector.getHeight(), 2.0);
        ofBeginShape();
        ofFill();
        ofSetColor(255);
        for (int j=0; j<calibratedPoints.size(); j++) {
            ofCurveVertex(calibratedPoints[j].x, calibratedPoints[j].y);
        }
        ofEndShape();
    }
    mask.end();
    
    // make final texture
    final.begin();
    ofClear(0, 0);
    shader.begin();
    shader.setUniformTexture("maskTex", mask.getTextureReference(), 1);
    
    player.draw(0, 0, final.getWidth(), final.getHeight());
    
    shader.end();
    final.end();

    // draw to window
    projector.begin();
    ofClear(0, 10);
    final.draw(0, 0);
    projector.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}
