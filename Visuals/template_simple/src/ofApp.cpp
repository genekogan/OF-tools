#include "ofApp.h"

void ofApp::setup()
{
    ofSetBackgroundAuto(false);
    scene.setup(ofGetWidth(), ofGetHeight());
}

void ofApp::update()
{
    scene.update();
}

void ofApp::draw()
{
    scene.draw(0, 0);
}

void ofApp::keyPressed(int key)
{

}
