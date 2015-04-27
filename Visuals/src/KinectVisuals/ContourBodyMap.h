#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "ContourVisual.h"




class ContourBodyMap : public ContourVisual
{
public:
    void setup(int width, int height);
    void update(OpenNI & openNi);
    void begin();
    void end();

private:
    
    ofShader shader;
    ofFbo gfx;
    ofFbo mask;
    ofFbo final;    
    //ofVideoPlayer player;
};