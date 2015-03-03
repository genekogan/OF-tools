#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    player.setup();
    
    //player.loadClip("wave", "/Users/Gene/Downloads/Pantha Du Prince & The Bell Laboratory - Elements Of Light (2013) [320]/01 Wave.mp3");
    //player.loadClip("fast", "/Users/Gene/Downloads/Pantha Du Prince & The Bell Laboratory - Elements Of Light (2013) [320]/02 Particle.mp3");
    
    //player.loadClip("kick", "/Users/Gene/Code/openFrameworks/addons/ofxAudioUnit/example-3-busses/bin/data/kick.wav");
    //player.loadClip("hats", "/Users/Gene/Code/openFrameworks/addons/ofxAudioUnit/example-3-busses/bin/data/hats.wav");
    //player.loadClip("snare", "/Users/Gene/Code/openFrameworks/addons/ofxAudioUnit/example-3-busses/bin/data/snare.wav");
    //player.loadClip("kick", "/Applications/Max 6.1/patches/docs/tutorial-patchers/msp-tut/bd+hh.aiff");

    player.loadClip("wave", "/Users/Gene/Desktop/WhiteBuilding/Wave.wav");
    player.loadClip("particle", "/Users/Gene/Desktop/WhiteBuilding/02 Particle.mp3");
    player.loadClip("track", "/Users/Gene/Desktop/WhiteBuilding/02 Track 2.wma");
    //player.loadSynth("synth", 'aufx', 'sing', 'SNSH');
    
    //player.loadSynth("synth", 'aufx', 'sing', 'SNSH');
    player.loadSynth("aalto", 'aumu', 'Aalt', 'MLbs');
    
    
    //player.blah();

    /*
    player.loadClip("1", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/406__tictacshutup__click-1-d.wav");
    player.loadClip("2", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/407__tictacshutup__click-1-off-click.wav");
    player.loadClip("3", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/408__tictacshutup__light-1.wav");
    player.loadClip("4", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/409__tictacshutup__light-2.wav");
    player.loadClip("5", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/410__tictacshutup__thump-1.wav");
    player.loadClip("6", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/411__tictacshutup__thump-3.wav");
    player.loadClip("7", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/412__tictacshutup__thump-4.wav");
    player.loadClip("8", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/413__tictacshutup__thump-5-big.wav");
    player.loadClip("9", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/414__tictacshutup__thump-6-big.wav");
    player.loadClip("10", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/415__tictacshutup__thump-7-big.wav");
    player.loadClip("11", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/416__tictacshutup__thump-8-big.wav");
    player.loadClip("12", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/417__tictacshutup__thump-fumble-2.wav");
    player.loadClip("13", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/418__tictacshutup__thump-fumble.wav");
    player.loadClip("14", "/Users/Gene/audio/Sounds/15__tictacshutup__household-percussion/419__tictacshutup__thump-treble.wav");
     */
}

//--------------------------------------------------------------
void ofApp::update(){

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //if (ofGetFrameNum()  > 200) player.blah();
}
