#include "Meshy.h"


void Meshy::setup(int width, int height, bool clearControls)
{
    Scene::setup(width, height, clearControls);
 
    realtime = true;
    circles = true;
    filled = false;
    numCandidates = 10;
    numPoints = 1000;
    minDist = 130;
    color = ofColor(255, 50);
    varColor = ofColor(20);
    margin = 8;
    triNoise = 0.01;
    colorNoise = 0.001;
    
    control.addToggle("realtime", &realtime);
    control.addToggle("circles", &circles);
    control.addToggle("filled", &filled);
    control.addSlider("numCandidates", &numCandidates, 1, 50);
    control.addSlider("numPoints", &numPoints, 10, 3000);
    control.addSlider("minDist", &minDist, 0.0f, 500.0f);
    control.addSlider("margin", &margin, 0.0f, 60.0f);
    control.addSlider("triNoise", &triNoise, 0.0f, 0.01f);
    control.addSlider("colorNoise", &colorNoise, 0.0f, 0.01f);
    control.addColor("color", &color);
    control.addColor("varColor", &varColor);
    control.addSlider("lineWidth", &lineWidth, 0.0f, 5.0f);
    control.addButton("triangulate", this, &Meshy::triangulate);
 
    GuiElementEventArgs s("", 0, 0);
    triangulate(s);
}

void Meshy::triangulate(GuiElementEventArgs &evt)
{
    if (circles)
    {
        circlePacker.clear();
        circlePacker.setBounds(0, 0, width, height);
        circlePacker.setMinDist(minDist);
        circlePacker.addPoints(numPoints);
        triangulator.triangulate(circlePacker.getPoints());
    }
    else
    {
        pointGenerator.clear();
        pointGenerator.setBounds(0, 0, width, height);
        pointGenerator.setNumberCandidates(numCandidates);
        pointGenerator.addPoints(numPoints);
        triangulator.triangulate(pointGenerator.getPoints());
    }
}

void Meshy::update()
{
    if (realtime)
    {
        points.clear();
        float x, y;
        for (int i=0; i<numPoints; i++) {
            x = width  * ofNoise(10+0.7*i, triNoise * ofGetFrameNum()+10, 15);
            y = height * ofNoise(20+1.2*i, triNoise * ofGetFrameNum(),    25);
            points.push_back(ofPoint(x, y));
        }
        triangulator.triangulate(points);
    }
    triangles = triangulator.getTriangles();
    vertices = triangulator.getVertices();
}

void Meshy::draw(int x, int y)
{
    Scene::beginDraw(x, y);

    for (int i=0; i<triangles.size(); i++)
    {
        ITRIANGLE t0 = triangles[i];
        XYZ v1 = vertices[t0.p1];
        XYZ v2 = vertices[t0.p2];
        XYZ v3 = vertices[t0.p3];
        
        float z = abs(v1.x-v2.x) + abs(v3.x-v2.x) + abs(v2.y-v1.y) + abs(v3.y-v2.y);
        if (z > 800) continue;
        
        ofVec3f colorMargin = ofVec3f(ofSignedNoise(colorNoise * ofGetFrameNum(), i, 50),
                                      ofSignedNoise(colorNoise * ofGetFrameNum(), i, 60),
                                      ofSignedNoise(colorNoise * ofGetFrameNum(), i, 70));
        
        ofSetColor(255*color.r + colorMargin.x * varColor.r,
                   255*color.g + colorMargin.y * varColor.g,
                   255*color.b + colorMargin.z * varColor.b,
                   255*color.a);

        if (filled)
        {
            ofFill();
            ofSetLineWidth(0);
        }
        else
        {
            ofNoFill();
            ofSetLineWidth(lineWidth);
        }
        
        ofTriangle(v1.x + margin * ofSignedNoise( 0, i, triNoise * ofGetFrameNum()),
                   v1.y + margin * ofSignedNoise( 5, i, triNoise * ofGetFrameNum()),
                   v2.x + margin * ofSignedNoise(10, i, triNoise * ofGetFrameNum()),
                   v2.y + margin * ofSignedNoise(15, i, triNoise * ofGetFrameNum()),
                   v3.x + margin * ofSignedNoise(20, i, triNoise * ofGetFrameNum()),
                   v3.y + margin * ofSignedNoise(25, i, triNoise * ofGetFrameNum()));
    }

    Scene::endDraw();
}
