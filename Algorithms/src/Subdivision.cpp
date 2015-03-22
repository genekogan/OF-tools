#include "Subdivision.h"


void SubdividingPolygon::setup(vector<ofPoint> points, ofColor color)
{
    this->points = points;
    pointDist.resize(points.size());
    totalDist = 0;
    for (int i=0; i<points.size(); i++)
    {
        float d = ofDist(points[i].x, points[i].y, points[(i+1)%points.size()].x, points[(i+1)%points.size()].y);
        pointDist[i] = d;
        totalDist += d;
    }
    this->color = color;
}

ofColor SubdividingPolygon::perturbColor(float dr, float dg, float db)
{
    return ofColor((int) (color.r + ofRandom(-dr, dr)) % 255,
                   (int) (color.g + ofRandom(-dg, dg)) % 255,
                   (int) (color.b + ofRandom(-db, db)) % 255);
}

void SubdividingPolygon::subdivide(int n)
{
    if (n==0)   return;
    
    vector<ofPoint> points1, points2;
    
    float r1=0;
    float r2=0;
    while (abs(r1-r2) < 0.001)
    {
        r1 = ofRandom(1);
        r2 = ofRandom(1);
    }
    
    float div1 = min(r1, r2) * totalDist;
    float div2 = max(r1, r2) * totalDist;
    
    float currDist = 0;
    int section = 0;
    for (int i=0; i<points.size(); i++)
    {
        if (section == 0)
        {
            points1.push_back(points[i]);
            if (currDist + pointDist[i] > div1)
            {
                section = 1;
                float r = (div1 - currDist) / pointDist[i];
                ofPoint subPoint = ofPoint(ofLerp(points[i].x, points[(i+1)%points.size()].x, r),
                                           ofLerp(points[i].y, points[(i+1)%points.size()].y, r));
                points1.push_back(subPoint);
                points2.push_back(subPoint);
            }
        }
        else if (section == 1)
        {
            points2.push_back(points[i]);
            if (currDist + pointDist[i] > div2)
            {
                section = 2;
                float r = (div2 - currDist) / pointDist[i];
                ofPoint subPoint = ofPoint(ofLerp(points[i].x, points[(i+1)%points.size()].x, r),
                                           ofLerp(points[i].y, points[(i+1)%points.size()].y, r));
                points1.push_back(subPoint);
                points2.push_back(subPoint);
                
            }
        }
        else if (section == 2) {
            points1.push_back(points[i]);
        }
        currDist += pointDist[i];
    }
    
    if (points1.size() > 2)
    {
        ofColor color1 = perturbColor(24, 24, 24);
        SubdividingPolygon *p1 = new SubdividingPolygon();
        p1->setup(points1, color1);
        p1->subdivide(n-1);
        children.push_back(p1);
    }
    if (points2.size() > 2)
    {
        ofColor color2 = perturbColor(24, 24, 24);
        SubdividingPolygon *p2 = new SubdividingPolygon();
        p2->setup(points2, color2);
        p2->subdivide(n-1);
        children.push_back(p2);
    }
}

void SubdividingPolygon::draw()
{
    if (children.size() > 0)
    {
        for (int i=0; i<children.size(); i++) {
            children[i]->draw();
        }
    }
    else
    {
        ofSetColor(color);
        ofFill();
        ofBeginShape();
        for (int i=0; i<points.size(); i++) {
            ofVertex(points[i].x, points[i].y);
        }
        ofEndShape(close);
    }
}
