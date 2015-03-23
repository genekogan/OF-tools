#pragma once

#include "ofMain.h"
#include "Shader.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    Shader scene;
};
