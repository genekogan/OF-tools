#include "ofApp.h"

void ofApp::setup()
{
    // ofSetBackgroundAuto(false);
    //ofSetFrameRate(4);
    
    projector.setup("projector", ofGetScreenWidth(), 0, 1280, 800, true);
    
    
    canvas.setup("Canvas", 1280, 800);
    
    canvas.setGuiDisplayMode(Canvas::EXPANDED_ATTACHED);
    
    canvas.addCreator(new Amoeba());
    canvas.addCreator(new Agents());
    canvas.addCreator(new Bubbles());
    canvas.addCreator(new Cubes());
    canvas.addCreator(new DebugScreen());
    canvas.addCreator(new GridFly());
    canvas.addCreator(new Letters());
    canvas.addCreator(new Meshy());
    canvas.addCreator(new MediaPlayer());
    canvas.addCreator(new Polar());
    canvas.addCreator(new Rivers());
    canvas.addCreator(new ScreenGrabber());
    canvas.addCreator(new Shader(true));
    canvas.addCreator(new Subdivide());
    canvas.addCreator(new Syphon());
    
    
//    kinect.setup("/Users/gene/Downloads/hometest_single.oni");
    kinect.setup();
    kinect.enableCalibration(projector);
    kinect.loadCalibration("/Users/gene/Code/of_v0.8-4.4_osx_release/addons/ofxKinectProjectorToolkit/calibration/bin/data/calibration.xml");
    
    map.setup(projector.getWidth(), projector.getHeight());
    
    
    canvas.getControl().setPosition(200, 0);
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.stop();
}

//--------------------------------------------------------------
void ofApp::update()
{
    // osc.update();    
    canvas.update();
    kinect.update();
    map.update(kinect);
    
}

void ofApp::draw()
{
    kinect.draw();
 
    if (!kinect.isCalibrating()) {
        projector.begin();
        ofClear(0, 0);

        map.begin();
        canvas.draw(0, 0);
        map.end();
        
        projector.end();
    }

}

void ofApp::keyPressed(int key)
{
    if (key=='g') {
        canvas.toggleGuiVisible();
    }
}
