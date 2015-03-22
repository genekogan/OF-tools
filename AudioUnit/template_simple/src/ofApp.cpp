#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

//    AudioUnitFilePlayer *p1 = au.loadFilePlayer("wave", "/Users/Gene/media/WhiteBuilding/_FINAL/Wave.wav");
//    AudioUnitFilePlayer *p2 = au.loadFilePlayer("particle", "/Users/Gene/Media/WhiteBuilding/02 Particle.mp3");
    
    AudioUnitInstrument *aalto = au.loadInstrument("aalto", 'aumu', 'Aalt', 'MLbs');
    AudioUnitInstrument *kaivo = au.loadInstrument("kaivo", 'aumu', 'Kaiv', 'MLbs');

    au.connectToMixer(aalto);
    au.connectToMixer(kaivo);
//    au.connectToMixer(p1);
//    au.connectToMixer(p2);
    

    // can make a midi controller independently and bind
    midi = new MidiSequencer("midi");
    midi->setRectangle(50, 400, 900, 300);
    aalto->setMidiSequencer(midi);
//    kaivo->setMidiSequencer(midi);

    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}
