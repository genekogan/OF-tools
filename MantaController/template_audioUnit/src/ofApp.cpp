#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    AudioUnitInstrument *aalto = audioUnit.loadInstrument("aalto", 'aumu', 'Aalt', 'MLbs');
    audioUnit.connectToMixer(aalto);
    manta.setup(aalto);
}

//--------------------------------------------------------------
void ofApp::update(){
    manta.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    manta.draw(0, 0, 400);
    manta.drawStats(0, 400, 400);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='z') {
        manta.blah();
    }
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
