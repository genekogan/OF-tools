#include "ofApp.h"

void ofApp::setup()
{
    ofSetBackgroundAuto(false);
    
    rivers.setup(480, 420);
    amoeba.setup(480, 420);
    bubbles.setup(480, 420);
    polar.setup(480, 420);

    meta.setName("Visuals");
    meta.addWidget(&rivers.getControl());
    meta.addWidget(&amoeba.getControl());
    meta.addWidget(&bubbles.getControl());
    meta.addWidget(&polar.getControl());
}

void ofApp::update()
{

}

void ofApp::draw()
{
    rivers.draw(210, 0);
    amoeba.draw(210, 440);
    bubbles.draw(700, 0);
    polar.draw(700, 440);
}

void ofApp::keyPressed(int key)
{

}
