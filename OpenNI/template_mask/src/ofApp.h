#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "Control.h"
#include "ofxSecondWindow.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);
	
    OpenNI kinect;
    ofxSecondWindow projector;
    
    
    
    
    ofShader shader;
    ofFbo gfx;
    ofFbo mask;
    ofFbo final;
    
    
    ofVideoPlayer player;
    ofVideoPlayer bgPlayer;

    
    GuiPanel panel;
    float smoothness;
    float position, position2;
    float volume, volume2;
    
    void setVolume(GuiElementEventArgs &e) {
        player.setVolume(volume);
    }
    void setPosition(GuiElementEventArgs &e) {
        player.setFrame(floor(position * player.getTotalNumFrames()));
    }
    void setVolumeBg(GuiElementEventArgs &e) {
        bgPlayer.setVolume(volume2);
    }
    void setPositionBg(GuiElementEventArgs &e) {
        bgPlayer.setFrame(floor(position2 * player.getTotalNumFrames()));
    }

};
