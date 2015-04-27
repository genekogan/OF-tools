#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    manta.setup();
    
    
    ofSetVerticalSync(true);
    ofBackground(255);
    
    
    ofAddListener(manta.getManta().padEvent, this, &ofApp::PadEvent);
    ofAddListener(manta.getManta().sliderEvent, this, &ofApp::SliderEvent);
    ofAddListener(manta.getManta().buttonEvent, this, &ofApp::ButtonEvent);
    ofAddListener(manta.getManta().padVelocityEvent, this, &ofApp::PadVelocityEvent);
    ofAddListener(manta.getManta().buttonVelocityEvent, this, &ofApp::ButtonVelocityEvent);


}

//--------------------------------------------------------------
void ofApp::update(){
    manta.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    manta.draw(50, 50, 400);
    manta.drawStats(470, 50, 400);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//----------
void ofApp::PadEvent(ofxMantaEvent & evt) {
    cout << "Pad event: " << ofGetElapsedTimeMicros() << ", id " << evt.id << ", row "<< evt.row <<", column "<< evt.col << ", value "<< evt.value << endl;
}

//----------
void ofApp::SliderEvent(ofxMantaEvent & evt) {
    cout << "Slider event: id " << evt.id << ", value "<< evt.value << endl;
}

//----------
void ofApp::ButtonEvent(ofxMantaEvent & evt) {
    cout << "Button event: id " << evt.id << ", value "<< evt.value << endl;
}

//----------
void ofApp::PadVelocityEvent(ofxMantaEvent & evt) {
    cout << "Pad velocity event: id " << evt.id << ", row "<< evt.row <<", column "<< evt.col << ", value "<< evt.value << endl;
}

//----------
void ofApp::ButtonVelocityEvent(ofxMantaEvent & evt) {
    cout << "Button velocity event: id " << evt.id << ", value "<< evt.value << endl;
}
