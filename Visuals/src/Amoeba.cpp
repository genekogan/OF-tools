#include "Amoeba.h"


void Amoeba::setup(int width, int height, bool clearControls)
{
    Scene::setup(width, height, clearControls);
    
    numVertices = 200;
    center = ofVec2f(width/2, height/2);
    radRange = ofVec2f(0, width/3);
    speed = 0.01;
    noiseFactor = 0.07;
    offset = 0.0;
    noiseRegion = 0.0;
    time = 0.0;
    colorFill = ofFloatColor(255, 100);
    colorStroke = ofFloatColor(255, 100);
    filled = false;
    stroked = true;
    curvedVertices = false;
    lineWidth = 1;
    cycleFill = 50;
    cycleStroke = 50;
    densityStroke = 1;
    densityFill = 1;
    
    control.addSlider("center", &center, ofVec2f(0, 0), ofVec2f(width, height));
    control.addSlider("numVertices", &numVertices, 3, 1000);
    control.addSlider("radRange", &radRange, ofVec2f(-width, -width), ofVec2f(width, width));
    control.addSlider("speed", &speed, 0.0f, 0.1f);
    control.addSlider("noiseFactor", &noiseFactor, 0.0f, 0.1f);
    control.addSlider("noiseRegion", &noiseRegion, -5.0f, 5.0f);
    control.addSlider("offset", &offset, -4.0f, 4.0f);
    control.addColor("colorFill", &colorFill);
    control.addColor("colorStroke", &colorStroke);
    control.addSlider("lineWidth", &lineWidth, 0.0f, 16.0f);
    control.addToggle("filled", &filled);
    control.addSlider("filledCycle", &cycleFill, 4, 200 );
    control.addSlider("filledDensity", &densityFill, 0.0f, 1.0f);
    control.addToggle("stroked", &stroked);
    control.addSlider("strokedCycle", &cycleStroke, 4, 200);
    control.addSlider("strokedDensity", &densityStroke, 0.0f, 1.0f);
    control.addToggle("curvedVertices", &curvedVertices);
}

void Amoeba::update()
{
    time += speed;
}

void Amoeba::draw(int x, int y)
{
    Scene::beginDraw(x, y);

    if (filled)
    {
        float t = (float) (ofGetFrameNum() % cycleFill) / cycleFill;
        if (t < densityFill)
        {
            ofFill();
            ofSetColor(colorFill);
            ofSetLineWidth(0);
            drawAmoeba();
        }
    }
    if (stroked)
    {
        float t = (float) (ofGetFrameNum() % cycleStroke) / cycleStroke;
        if (t < densityStroke)
        {
            ofNoFill();
            ofSetColor(colorStroke);
            ofSetLineWidth(lineWidth);
            drawAmoeba();
        }
    }
    
    Scene::endDraw();
}

void Amoeba::drawAmoeba()
{
    ofBeginShape();
    float ang, rad0, rad, x, y;
    for (int i=0; i<numVertices; i++)
    {
        ang = ofMap(i, 0, numVertices, 0, TWO_PI);
        rad0 = ofNoise(offset + noiseFactor * i, noiseRegion, time);
        rad = ofMap(rad0, 0, 1, radRange.x, radRange.y);
        x = center.x + rad * cos(ang);
        y = center.y + rad * sin(ang);
        if (curvedVertices) ofCurveVertex(x, y);
        else                ofVertex(x, y);
    }
    ofEndShape(true);
}

