#include "SkeletonRivers.h"



void SkeletonRivers::setup(int width, int height)
{
    SkeletonVisual::setup(width, height);
    this->width = width;
    this->height = height;
    
    rivers.setup(width, height);
    
    panel.addSlider("mult", &mult, 0.0f, 10.0f);
    panel.addSlider("lerp", &lerpRate, 0.0f, 1.0f);
    panel.addWidget(&rivers.getControl());
    
    mult = 1.0;
    lerpRate = 0.1;
}

void SkeletonRivers::update(OpenNI & openNi)
{
    SkeletonVisual::update(openNi);
    
    
    map<int, Skeleton*>::iterator it = users.begin();
    for (; it !=users.end(); ++it) {
        
        for (int j=0; j<it->second->points.size(); j++) {
            
            for (int i1=0; i1<SIZE; i1++) {
                for (int j1=0; j1<SIZE; j1++) {
                    
                    float x = ofMap(i1, 0, SIZE, 0, width);
                    float y = ofMap(j1, 0, SIZE, 0, height);
                    float d = abs(x - it->second->points[j].x) + abs(y - it->second->points[j].y);
                    
                    float wt = ofClamp(ofMap(d, 0, 20, 1, 0), 0, 1);
                    
                    
                    rivers.force[i1][j1].set(ofLerp(rivers.force[i1][j1].x, mult * it->second->velocity[j].x, wt*lerpRate),
                                             ofLerp(rivers.force[i1][j1].y, mult * it->second->velocity[j].y, wt*lerpRate));
                }
            }
        }
        
    }
    rivers.update();
    
    
    
}

void SkeletonRivers::draw()
{
    //ofClear(0, 0);
    
    rivers.draw(0, 0);
    
    
    
    map<int, Skeleton*>::iterator it = users.begin();
    for (; it !=users.end(); ++it) {
        
        for (auto p : it->second->points) {
            ofCircle(p.x, p.y, 4);
        }
        
        for (int j=0; j<it->second->points.size(); j++) {
            ofLine(it->second->points[j].x, it->second->points[j].y,
                   it->second->points[j].x + 10*it->second->velocity[j].x, it->second->points[j].y + 10*it->second->velocity[j].y);
        }
        
    }
    
    
}
