#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    for (int i=0; i<5; i++) {
        floatValues.push_back(ofRandom(3,8));
        floatMins.push_back(ofRandom(0,3));
        floatMaxs.push_back(ofRandom(8,10));
        vec3Values.push_back(ofVec3f(ofRandom(2,4), ofRandom(2,4), ofRandom(2,4)));
        vec3Mins.push_back(ofVec3f(ofRandom(2), ofRandom(2), ofRandom(2)));
        vec3Maxs.push_back(ofVec3f(ofRandom(4,10), ofRandom(4,10), ofRandom(4,10)));
    }

    vector<string> items;
    items.push_back("hello menu");
    items.push_back("new item");
    items.push_back("another item");
    
    panel.setName("hello world");
    panel.setPosition(20, 20);

    panel.addParameter("float slider", &floatValue, 20.0f, 220.0f);
    panel.addParameter("vec2", &vec2, ofVec2f(200, 0), ofVec2f(1024, 768));
    panel.addParameter("vector (floats)", &floatValues, floatMins, floatMaxs);
    panel.addParameter("my button", &boolValue);
    panel.addParameter("vec3", &vec3, ofVec3f(0, 0, 0), ofVec3f(4,3,5));
    panel.addParameter("vector (vec3)", &vec3Values, vec3Mins, vec3Maxs);
    panel.addColor("fore color", &color);
    panel.addMenu("my menu", items, this, &ofApp::menuSelect);
    
    vec2.set(500, 400);
    color.set(0.1, 0.14, 0.21);
}

//--------------------------------------------------------------
void ofApp::update(){
    panel.update();
    
    floatValue = ofMap(mouseX, 0, ofGetWidth(), 20, 220);
    color.set(ofMap(mouseY, 0, ofGetHeight(), 0, 1), color.g, color.b);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(color);
    
    ofSetColor(255);
    if (boolValue) {
        ofFill();
    }
    else {
        ofNoFill();
    }
    ofCircle(vec2.x, vec2.y, floatValue);
    
    panel.draw();
    
    //cout << ofToString(vec3Values) << endl;
}
