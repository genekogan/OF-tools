#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "ContourVisual.h"


class ContourBackgrounding : public ContourVisual
{
public:
    void setup(int width, int height);
    void update(OpenNI & openNi);
    void draw();
    
private:
        
    int mode;
    float refreshAlpha;
    ofFloatColor color;
    int skip;
    int numPoints;
    bool horiz;
    float mult;
    float offset;
};