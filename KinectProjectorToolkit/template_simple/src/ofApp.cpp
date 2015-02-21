#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    kinect.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");

    secondWindow.setup("projection", ofGetScreenWidth(), 0, 1280, 800, true);
    calibration.setup(kinect, secondWindow);
    calibration.start();
    
    
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
        calibration.update();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //kinect.draw();
    

    if (!done) {
        calibration.draw();
    }
    else {
        
        kinect.draw();
         
        ofSetColor(0);
        ofDrawBitmapString("fps : "+ofToString(ofGetFrameRate()), 20, 700);

    }
    
    if (done) {
        secondWindow.begin();
        ofClear(0, 0);
        ofBackground(255);
        
        
        
        
        
        
        ofPushStyle();
        ofFill();
        ofSetColor(255, 255, 100, 255);
        
        
        ofPoint p = calibration.getProjectedPoint(min(639,ofGetMouseX()), min(479,ofGetMouseY()));
        
        
        ofCircle(secondWindow.getWidth() * p.x, secondWindow.getHeight() * p.y, 20);
        

        ContourFinder & cf = kinect.getContourFinder();
        RectTracker& tracker = cf.getTracker();
        for(int i = 0; i < cf.size(); i++) {
            vector<ofVec2f> calibratedPoints;
            vector<cv::Point> points = cf.getContour(i);
            
            
            calibration.getCalibratedContour(points, calibratedPoints, secondWindow.getWidth(), secondWindow.getHeight());
            /*
            for (int j=0; j<points.size(); j++) {
                ofVec2f p = calibration.getProjectedPoint(points[j].x, points[j].y);
                calibratedPoints.push_back(ofVec2f(p.x * 1280.0f, p.y * 800.0f));
            }
            */
            
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
        
        secondWindow.end();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key=='s') {
        calibration.stop();
        vector<double> coefficients = calibration.getCalibration();
        done = true;
    }
 
    
}
