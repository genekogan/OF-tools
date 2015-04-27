#pragma once

#include "ofMain.h"
#include "OpenNI.h"


class SkeletonVisual
{
public:
    
    struct Skeleton
    {
        int xnId;
        vector<ofVec2f> points;
        vector<ofVec2f> velocity;
    };
    
    
    virtual void setup(int width, int height);
    virtual void update(OpenNI & openNi);
    virtual void draw();
    
    GuiPanel & getPanel() {return panel;}
    
protected:
    
    GuiPanel panel;
    
    int width, height;
    
    
    map<int, Skeleton*> users;
    
};