#include "ContourBodyMap.h"


void ContourBodyMap::setup(int width, int height)
{
    ContourVisual::setup(width, height);

    mask.allocate(width,height);
    final.allocate(width,height);
    
    shader.load("standard.vert", "alphamask.frag");

    // final texture
    final.begin();
    ofClear(0,0,0,0);
    final.end();
}

void ContourBodyMap::update(OpenNI & openNi)
{
    ContourVisual::update(openNi);

    mask.begin();
    ofClear(0, 0);
    for (int i=0; i<currentContours.size(); i++)
    {
        ofBeginShape();
        ofFill();
        ofSetColor(255);
        for (int j=0; j<currentContours[i].size(); j++) {
            ofCurveVertex(currentContours[i][j].x, currentContours[i][j].y);
        }
        ofEndShape();
    }
    mask.end();
}

void ContourBodyMap::begin()
{
    // make final texture
    final.begin();
    ofClear(0, 0, 0, 0);
    
    shader.begin();
    shader.setUniformTexture("maskTex", mask.getTextureReference(), 1);
}

void ContourBodyMap::end()
{
    //player.draw(0, 0, final.getWidth(), final.getHeight());
    
    shader.end();
    final.end();

//    eraseContours();

    final.draw(0, 0);
}
