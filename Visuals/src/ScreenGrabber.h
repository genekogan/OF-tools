#pragma once

#include "ofMain.h"
#include "ofxScreenGrab.h"
#include "Scene.h"


class ScreenGrabber : public Scene
{
public:
    ScreenGrabber() {setName("ScreenGrabber");}
    
    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);
    
    void eventSetWidth(GuiSliderEventArgs<int> &evt);
    void eventSetHeight(GuiSliderEventArgs<int> &evt);
    void eventSetRetina(GuiButtonEventArgs &evt);
    
private:
    
    ofxScreenGrab grabber;
    int gx, gy, gwidth, gheight;
    bool retina;
};



