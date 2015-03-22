#pragma once

#include "ofMain.h"
#include "Scene.h"


class DebugScreen : public Scene
{
public:
    void setup();
    void update();
    void draw();
    
    void setup(int width, int height);
    void draw(int x, int y);

private:
    
    enum DebugType { FULL, GRADIENT, CHECKERBOARD, FRAMES };
    
    void drawFull();
    void drawCheckerboard();
    void drawFrames();
    void drawGradient();

    void setType(GuiElementEventArgs &e);
    void setupGradient(GuiElementEventArgs &e);
    void refresh(GuiElementEventArgs &e);
    
    ofVboMesh gradientMesh;
    DebugType type = GRADIENT;
    string gradientMode;
    
    ofFloatColor color1;
    ofFloatColor color2;
    int numRects;
    int speed;
};