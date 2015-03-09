#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetFrameRate(15);
    
    AudioUnitFilePlayer *p1 = au.loadFilePlayer("wave", "/Users/Gene/media/WhiteBuilding/_FINAL/Wave.wav");
    AudioUnitFilePlayer *p2 = au.loadFilePlayer("particle", "/Users/Gene/Media/WhiteBuilding/02 Particle.mp3");
    
    AudioUnitInstrument *aalto = au.loadInstrument("aalto", 'aumu', 'Aalt', 'MLbs');

    au.connectToMixer(aalto);
    au.connectToMixer(p1);
    au.connectToMixer(p2);
    


    
    midi = new MidiSequencer("midi");
    

    
    aalto->setMidiSequencer(midi);

}

//--------------------------------------------------------------
void ofApp::update(){

    //ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){

    //if (ofGetFrameNum()  > 200) player.blah();
}
