#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    secondWindow.setup("projection", ofGetScreenWidth()-500, 0, 1280, 800, false);

    kinect.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");
    kinect.setTrackingContours(true);
    kinect.loadCalibration("calibration.xml");
    
    calibration.setup(kinect, secondWindow);
    calibration.loadCalibration("calibration.xml");
    
    ribbons.setup();
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.stop();
}

//--------------------------------------------------------------
void ofApp::update()
{
    bool newFrame = kinect.update();
    if (newFrame) {
        //calibration.update();
    }
    
    
    ribbons.recordContours(kinect);
    ribbons.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    //kinect.draw();
    
    if (!done) {
//        calibration.draw();
    }
    else {
        kinect.draw();
    }
    
    if (done) {
       // secondWindow.begin();
//        ofClear(0, 0);
//        ofBackground(255);
        
        
        ofPushStyle();
        ofFill();
        ofSetColor(255, 255, 100, 255);
        
        
        for(int i = 0; i < kinect.getNumContours(); i++)
        {
            vector<cv::Point> points = kinect.getContour(i);
            vector<ofVec2f> calibratedPoints;
            kinect.getCalibratedContour(i, calibratedPoints, secondWindow.getWidth(), secondWindow.getHeight(), 2.0f);
            
            ofBeginShape();
            if (curved) {
                for (int j=0; j<calibratedPoints.size(); j++) {
                    ofCurveVertex(calibratedPoints[j].x, calibratedPoints[j].y);
                }
            }
            else {
                for (int j=0; j<calibratedPoints.size(); j++) {
                    ofVertex(calibratedPoints[j].x, calibratedPoints[j].y);
                }
            }
            ofEndShape();
            
            

        }
        
        
        
        ofPopStyle();
        
        
        
        secondWindow.begin();
        ofClear(0, 0);
        ofBackground(0);
        ribbons.draw();
        secondWindow.end();
        
       // secondWindow.end();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
