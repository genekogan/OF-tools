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


#define SUBDIVIDE


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

#ifdef AGENTS
    Agents scene;
#endif
#ifdef AMOEBA
    Amoeba scene;
#endif
#ifdef BUBBLES
    Bubbles scene;
#endif
#ifdef CUBES
    Cubes scene;
#endif
#ifdef DEBUG
    DebugScreen scene;
#endif
#ifdef GRIDFLY
    GridFly scene;
#endif
#ifdef LETTERS
    Letters scene;
#endif
#ifdef MESHY
    Meshy scene;
#endif
#ifdef POLAR
    Polar scene;
#endif
#ifdef RIVERS
    Rivers scene;
#endif
#ifdef SUBDIVIDE
    Subdivide scene;
#endif
    
};
