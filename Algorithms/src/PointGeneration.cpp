#include "PointGeneration.h"


void PointGenerator::setBounds(float x, float y, float w, float h)
{
    bounds.push_back(ofPoint(x, y));
    bounds.push_back(ofPoint(x+w, y));
    bounds.push_back(ofPoint(x+w, y+h));
    bounds.push_back(ofPoint(x, y+h));
}

void PointGenerator::setConvexBounds(vector<ofPoint> bounds)
{
    this->bounds = bounds;
}

void CirclePackGenerator::setMinDist(float minDist)
{
    this->minDist = minDist;
}

void EvenPointsGenerator::setNumberCandidates(int numberCandidates)
{
    this->numberCandidates = numberCandidates;
}

void EvenPointsGenerator::addPoints(int n)
{
    vector<ofPoint>::iterator it;
    ofPoint newPoint, candidate;
    float thisMinDist, currMinDist;
    bool done;
    
    for (int i=0; i<n; i++)
    {
        done = false;
        while (!done)
        {
            done = true;
            currMinDist = 0;
            for (int j=0; j<numberCandidates; j++)
            {
                thisMinDist = ofGetWidth();
                newPoint = samplePointWithinBounds();
                for(it = points.begin(); it != points.end(); ++it)
                {
                    float dist = ofDist(newPoint.x, newPoint.y, it->x, it->y);
                    thisMinDist = min(dist, thisMinDist);
                }
                for(it = bounds.begin(); it != bounds.end(); ++it)
                {
                    float dist = ofDist(newPoint.x, newPoint.y, it->x, it->y);
                    thisMinDist = min(dist, thisMinDist);
                }
                if (thisMinDist > currMinDist)
                {
                    candidate.set(newPoint.x, newPoint.y);
                    currMinDist = thisMinDist;
                }
            }
        }
        points.push_back(ofPoint(candidate.x, candidate.y));
    }
}

ofPoint PointGenerator::samplePointWithinBounds()
{
    // randomly pick two segments
    int idx1 = (int) ofRandom(bounds.size());
    int idx2 = idx1;
    while (idx2 == idx1) {
        idx2 = (int) ofRandom(bounds.size());
    }
    
    // interpolate random point between them
    float r1 = ofRandom(1);
    float r2 = ofRandom(1);
    float r3 = ofRandom(1);

    ofPoint p1 = ofPoint(
         ofLerp(bounds[idx1].x, bounds[(idx1+1)%bounds.size()].x, r1),
         ofLerp(bounds[idx1].y, bounds[(idx1+1)%bounds.size()].y, r1));
    ofPoint p2 = ofPoint(
         ofLerp(bounds[idx2].x, bounds[(idx2+1)%bounds.size()].x, r2),
         ofLerp(bounds[idx2].y, bounds[(idx2+1)%bounds.size()].y, r2));
    ofPoint point = ofPoint(
         ofLerp(p1.x, p2.x, r3),
         ofLerp(p1.y, p2.y, r3));
    
    return point;
}

void CirclePackGenerator::addPoints(int n)
{
    vector<ofPoint>::iterator it;
    ofPoint candidate;
    float radius, currMinDist;
    bool done;    
    for (int i=0; i<n; i++)
    {
        done = false;
        while (!done)
        {
            done = true;
            candidate = samplePointWithinBounds();
            radius = 0;
            currMinDist = minDist;
            for(it = points.begin(); it != points.end(); ++it)
            {
                float dist = ofDist(candidate.x, candidate.y, it->x, it->y) - it->z;
                if (dist < 0)
                {
                    done = false;
                    break;
                }
                else if (dist < currMinDist) {
                    currMinDist = dist;
                }
            }
            radius = currMinDist;
        }
        points.push_back(ofPoint(candidate.x, candidate.y, radius));
    }
}

void Triangulator::triangulate(vector<ofPoint> &points)
{
    triangulation.reset();
    for (int i=0; i<points.size(); i++) {
        triangulation.addPoint(points[i]);
    }
    triangulation.triangulate();
    triangles = triangulation.getTriangles();
    vertices = triangulation.getVertices();
}
