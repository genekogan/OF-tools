#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    projector.setup("projector", ofGetScreenWidth(), 0, 1280, 800, true);
//    kinect.setup("/Users/gene/Downloads/hometest_single.oni");
    kinect.setup();
    kinect.enableCalibration(projector);
    kinect.loadCalibration("/Users/gene/Code/openFrameworks/tools_old/OpenNi/template-mask-calibrated/bin/data/calibration.xml");
    //kinect.loadCalibration("/Users/gene/Desktop/calibration.xml");
    
    ribbons.setup(projector.getWidth(), projector.getHeight());
    fluids.setup(projector.getWidth(), projector.getHeight());
    stars.setup(projector.getWidth(), projector.getHeight());
    backgrounding.setup(projector.getWidth(), projector.getHeight());
    skeleton.setup(projector.getWidth(), projector.getHeight());
    skeletonRivers.setup(projector.getWidth(), projector.getHeight());
    
    ribbons.getPanel().setPosition(0, 400);
    fluids.getPanel().setPosition(200, 400);
    stars.getPanel().setPosition(400, 400);
    backgrounding.getPanel().setPosition(600, 400);
    skeleton.getPanel().setPosition(800, 400);
    skeletonRivers.getPanel().setPosition(1000, 400);
    
    mode = 5;
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.stop();
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();

    if      (mode == 0) ribbons.update(kinect);
    else if (mode == 1) fluids.update(kinect);
    else if (mode == 2) stars.update(kinect);
    else if (mode == 3) backgrounding.update(kinect);
    else if (mode == 4) skeleton.update(kinect);
    else if (mode == 5) skeletonRivers.update(kinect);
}

//--------------------------------------------------------------
void ofApp::draw(){
    kinect.draw();
    
    if (!kinect.isCalibrating()) {
        projector.begin();
        if      (mode == 0) ribbons.draw();
        else if (mode == 1) fluids.draw();
        else if (mode == 2) stars.draw();
        else if (mode == 3) backgrounding.draw();
        else if (mode == 4) skeleton.draw();
        else if (mode == 5) skeletonRivers.draw();
        projector.end();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if      (key=='1')   mode = 0;
    else if (key=='2')   mode = 1;
    else if (key=='3')   mode = 2;
    else if (key=='4')   mode = 3;
    else if (key=='5')   mode = 4;
    else if (key=='6')   mode = 5;
}
