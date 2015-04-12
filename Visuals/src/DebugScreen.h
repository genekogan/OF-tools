#pragma once

#include "ofMain.h"
#include "Scene.h"


class DebugScreen : public Scene
{
public:
    DebugScreen() {setName("Debug");}
    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);

private:
    
    enum DebugType { FULL, GRADIENT, CHECKERBOARD, FRAMES };
    
    void drawFull();
    void drawCheckerboard();
    void drawFrames();
    void drawGradient();

    void setType(GuiMenuEventArgs &e);
    void setupGradient(GuiMenuEventArgs &e);
    void refresh(GuiButtonEventArgs &e);
    
    ofVboMesh gradientMesh;
    DebugType type = GRADIENT;
    int gradientMode;
    
    ofFloatColor color1;
    ofFloatColor color2;
    int numRects;
    int speed;
};
