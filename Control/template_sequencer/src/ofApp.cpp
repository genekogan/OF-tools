#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    panel.setName("panel");
    panel.setPosition(20, 20);
    
    panel.addSlider("radius", &radius, 50.0f, 250.0f);
    panel.addSlider("resolution", &resolution, 3, 16);
    panel.addSlider("line width", &lineWidth, 4.0f, 12.0f);
    
    panel.addToggle("filled", &filled);
    panel.addSlider("position", &position, ofVec2f(500, 400), ofVec2f(ofGetWidth(), ofGetHeight()));
    panel.addColor("my color", &myColor);

    GuiMenu *menu1 = panel.addMenu("multi choice menu", this, &ofApp::multiChoiceMenuSelect);
    menu1->setMultipleChoice(true);
    menu1->addToggle("new york", &b1);
    menu1->addToggle("los angeles", &b2);
    menu1->addToggle("chicago", &b3);

    GuiMenu *menu2 = panel.addMenu("single choice menu", this, &ofApp::menuSelect);
    menu2->addToggle("kamusta mundo");
    menu2->addToggle("xin chao the gioi");
    menu2->addToggle("namaste varlda");
    menu2->addToggle("suesday piphoplok");
    menu2->addToggle("suwati lok");
    

    // enable sequencer (can be done through gui also)
    panel.enableSequencer();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetCircleResolution(resolution);
    filled ? ofFill() : ofNoFill();
    ofSetColor(myColor);
    ofSetLineWidth(lineWidth);
    ofCircle(position.x, position.y, radius);
    
    ofSetColor(0);
    ofDrawBitmapString(greeting+"\n"+city, position.x, position.y);
    
}

//--------------------------------------------------------------
void ofApp::menuSelect(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "menu selection event: " + e.name + " = " + ofToString(e.value));
    greeting = e.name;
}

//--------------------------------------------------------------
void ofApp::multiChoiceMenuSelect(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "panel menu event: " + e.name + " = " + ofToString(e.value));
    city  = b1 ? "New York " : "";
    city += b2 ? "Los Angeles " : "";
    city += b3 ? "Chicago" : "";
}

