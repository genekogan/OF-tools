#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "SkeletonVisual.h"
#include "Rivers.h"


class SkeletonRivers : public SkeletonVisual
{
public:
    void setup(int width, int height);
    void update(OpenNI & openNi);
    void draw();
    
protected:
    
    Rivers rivers;
    
    float mult;
    float lerpRate;
    
    int SIZE = 16;
};