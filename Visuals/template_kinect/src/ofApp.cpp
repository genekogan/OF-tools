#include "ofApp.h"

void ofApp::setup()
{
    projector.setup("projection", ofGetScreenWidth()-500, 0, 1280, 800, true);

    kinect.setup("/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");
    kinect.enableCalibration(projector);
    kinect.loadCalibration("/Users/Gene/Desktop/calibration.xml");
    kinect.setTrackingContours(true);
    
    ribbons.setup(projector.getWidth(), projector.getHeight());
    //fluids.setup(projector.getWidth(), projector.getHeight());
    //stars.setup(projector.getWidth(), projector.getHeight());
    //map.setup(projector.getWidth(), projector.getHeight());
    
    

}

void ofApp::exit()
{
    kinect.stop();
}

void ofApp::update()
{
    kinect.update();
    
    ribbons.recordContours(kinect);
    ribbons.update();
    //fluids.recordContours(kinect);
    //fluids.update();
    //stars.recordContours(kinect);
    //stars.update();
//    map.recordContours(kinect);
//    map.update();
    
}

void ofApp::draw()
{
    kinect.draw();
    if (!kinect.isCalibrating())
    {
        projector.begin();
        ofClear(0, 0);
        ofBackground(0);
        
        ribbons.draw();
        //fluids.draw();
        //stars.draw();
        //map.draw();
        
        projector.end();
    }
}

void ofApp::keyPressed(int key)
{

}
