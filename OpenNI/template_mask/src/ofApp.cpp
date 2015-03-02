#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    projector.setup("projector", ofGetScreenWidth()-100, 0, 1280, 800, false);
    kinect.setup();//"/Users/Gene/Code/openFrameworks/templates/Kinect/openni_oniRecording/bin/data/alecsroom.oni");
    kinect.enableCalibration(projector);
    //kinect.loadCalibration("/Users/Gene/Desktop/calibration.xml");
    kinect.loadCalibration("/Users/Gene/Code/openFrameworks/tools/Visuals/template_simple/bin/data/calibration.xml");
    
    //player.loadMovie("/Users/Gene/Media/german_train_grid.mov");
    player.loadMovie("/Users/Gene/Desktop/feel-white_building_high.mp4");
    player.setLoopState(OF_LOOP_NORMAL);
    player.play();
    
    
    bgPlayer.loadMovie("/Users/Gene/Desktop/balloon_final-building_high.mp4");
    bgPlayer.setLoopState(OF_LOOP_NORMAL);
    bgPlayer.play();
    
    
    ofEnableAlphaBlending();
    
    int width = ofGetWidth();
    int height = ofGetHeight();
    
    
    mask.allocate(width,height);
    final.allocate(width,height);
    
    shader.load("/Users/Gene/Code/openFrameworks/templates_old/etc/mask/bin/data/standard.vert",
                "/Users/Gene/Code/openFrameworks/templates_old/etc/mask/bin/data/alphamask.frag");
    
    
    // graphics texture
    gfx.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    gfx.begin();
    ofBackground(0, 255, 0);
    for (int i=0; i<200; i++) {
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        ofRect(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 200, 200);
    }
    gfx.end();
    
    
    // final texture
    final.begin();
    ofClear(0,0,0,0);
    final.end();

    
    panel.setName("OpenNI");
    panel.setPosition(10, 500);
    panel.addSlider("volume", &volume, 0.0f, 1.0f, this, &ofApp::setVolume);
    panel.addSlider("position", &position, 0.0f, 1.0f, this, &ofApp::setPosition);
    panel.addSlider("bg volume", &volume2, 0.0f, 1.0f, this, &ofApp::setVolumeBg);
    panel.addSlider("bg position", &position2, 0.0f, 1.0f, this, &ofApp::setPositionBg);
    panel.addSlider("smoothness", &smoothness, 0.1f, 20.0f);

}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.stop();
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    
    player.update();
    bgPlayer.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofDisableAlphaBlending();
    
    
    kinect.draw();
    

    
    
    // update mask
    mask.begin();
    ofClear(0, 0);
    
    for (int i=0; i<kinect.getNumContours(); i++)
    {
        vector<ofVec2f> calibratedPoints;
        kinect.getCalibratedContour(i, calibratedPoints, projector.getWidth(), projector.getHeight(), smoothness);
        
        ofBeginShape();
        ofFill();
        ofSetColor(240, 210, 45);
        for (int j=0; j<calibratedPoints.size(); j++) {
            ofCurveVertex(calibratedPoints[j].x, calibratedPoints[j].y);
        }
        ofEndShape();
    }
    mask.end();
    
    
    // make final texture
    final.begin();
    ofClear(0, 0, 0, 0);
    
    shader.begin();
    shader.setUniformTexture("maskTex", mask.getTextureReference(), 1);
    
    //gfx.draw(0,0);
    player.draw(0, 0, final.getWidth(), final.getHeight());
    
    shader.end();
    final.end();

    
    
    projector.begin();
    ofClear(0, 10);
    bgPlayer.draw(0, 0, projector.getWidth(), projector.getHeight());
    final.draw(0, 0);
    projector.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
}
