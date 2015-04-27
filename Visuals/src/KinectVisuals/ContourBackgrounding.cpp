#include "ContourBackgrounding.h"


void ContourBackgrounding::setup(int width, int height)
{
    ContourVisual::setup(width, height);
    panel.setName("backgrounding");
    panel.addSlider("mode", &mode, 0, 5);
    panel.addColor("color", &color);
    panel.addSlider("refreshAlpha", &refreshAlpha, 0.0f, 255.0f);
    panel.addSlider("skip", &skip, 1, 30);
    panel.addSlider("numPoints", &numPoints, 1, 300);
    panel.addToggle("horizontal", &horiz);
    panel.addSlider("mult", &mult, 0.0f, 30.0f);
    panel.addSlider("offset", &offset, (float)-TWO_PI, (float)TWO_PI);

    refreshAlpha = 10;
    color = ofFloatColor(1.0, 1.0, 1.0f, 0.1f);
    skip = 5;
    numPoints = 100;
    horiz = false;
    mult = 10;
    offset = 0;
}

void ContourBackgrounding::update(OpenNI & openNi)
{
    eraseContours();
    ContourVisual::update(openNi);
}

void ContourBackgrounding::draw()
{
    ofSetColor(0, refreshAlpha);
    ofFill();
    ofRect(0, 0, width, height);
    ofSetColor(color);

    if      (mode == 0)
    {
        for (auto c : contours) {
            for (int i=0; i<c->points.size(); i+=skip) {
                ofCircle(c->points[i], 5);
            }
        }

    }
    else if (mode == 1)
    {
        for (auto c : contours)
        {
            for (int i = 0; i < numPoints; i++)
            {
                int idx1 = ofRandom(c->points.size());
                int idx2 = ofRandom(c->points.size());
                ofLine(c->points[idx1], c->points[idx2]);
            }
        }
    }
    else if (mode == 2)
    {
        for (auto c : contours)
        {
            for (int i = 0; i < numPoints; i++)
            {
                int idx1 = ofRandom(c->points.size());
                if (horiz) {
                    ofLine(c->points[idx1].x, c->points[idx1].y, 0, c->points[idx1].y);
                }
                else {
                    ofLine(c->points[idx1].x, c->points[idx1].y, c->points[idx1].x, 0);
                }
            }
        }
    }
    else if (mode == 3)
    {
        for (auto c : contours)
        {
            for (int i = 0; i < numPoints; i++)
            {
                int idx1 = ofRandom(c->points.size());
                ofPoint p = (c->points[idx1] - c->center);
                ofLine(c->points[idx1].x, c->points[idx1].y, c->points[idx1].x + mult * p.x, c->points[idx1].y + mult * p.y);
            }
        }
    }
    else if (mode == 4)
    {
        for (auto c : contours)
        {
            for (int i = 0; i < c->points.size(); i+=10)
            {
                ofSetLineWidth(1);
                float ang = ofMap(i, 0, c->points.size(), 0, TWO_PI) + offset;
                ofPoint p = c->center + ofPoint(1000 * cos(ang), 1000 * sin(ang));
                //ofPoint p = ofPoint(width/2, height/2) + ofPoint(1000 * cos(ang), 1000 * sin(ang));
                ofLine(c->points[i].x, c->points[i].y, p.x, p.y);
            }
        }
    }
    else if (mode == 5)
    {
        float z = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 10);
        ofNoFill();
        ofSetLineWidth(1);
        for (auto c : contours)
        {
            
            ofPoint ctr = c->center;
            ofCircle(ctr.x, ctr.y, 20);
            
            ofBeginShape();
            for (auto p : c->points) {
                ofVertex(p.x + z*(p.x-ctr.x), p.y + z*(p.y-ctr.y));
            }
            ofEndShape();
        }
    }

    
    
    
    
    
    
    contours.clear();
}

