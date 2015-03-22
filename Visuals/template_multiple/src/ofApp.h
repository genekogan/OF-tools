#pragma once

#include "ofMain.h"

#include "Agents.h"
#include "Amoeba.h"
#include "Bubbles.h"
#include "Cubes.h"
#include "DebugScreen.h"
#include "GridFly.h"
#include "Letters.h"
#include "Meshy.h"
#include "Polar.h"
#include "Rivers.h"
#include "Subdivide.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    Rivers rivers;
    Amoeba amoeba;
    Bubbles bubbles;
    Polar polar;
    
    GuiPanel meta;
};
