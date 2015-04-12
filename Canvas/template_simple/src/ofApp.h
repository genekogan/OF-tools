#pragma once

#include "ofMain.h"

#include "Control.h"

#include "Agents.h"
#include "Amoeba.h"
#include "Bubbles.h"
#include "Cubes.h"
#include "DebugScreen.h"
#include "GridFly.h"
#include "Letters.h"
#include "Meshy.h"
#include "MediaPlayer.h"
#include "Polar.h"
#include "Rivers.h"
#include "ScreenGrabber.h"
#include "Subdivide.h"
#include "Shader.h"

#include "Canvas.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    Canvas canvas;
    
    //OscManager osc;
    
};
