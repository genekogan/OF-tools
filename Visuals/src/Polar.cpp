#include "Polar.h"


PolarEq::PolarEq(bool *is3d, bool *isRibbon, ofVec3f *speedRotation)
{
    this->is3d = is3d;
    this->isRibbon = isRibbon;
    this->speedRotation = speedRotation;
    //ribbon = new ofxTwistedRibbon();
    refresh();
}

void PolarEq::refresh()
{
    numPoints = 16;
    age = (int) ofRandom(100, 800);
    ang = 0;
    time = 0;
    noiseOffset = ofVec3f(ofRandom(100), ofRandom(100), ofRandom(100));
}

void PolarEq::update()
{
    float r, x, y, z;

    time += delTime;
    
    dAng = ofMap(ofNoise(angNoise * time + 5, noiseOffset.x), 0, 1, 0, dAngMax);
    dRate = ofMap(ofNoise(rateNoise * time + 10, noiseOffset.y), 0, 1, -dRateMax, dRateMax);
    
    ang  += (delTime*dAng);
    rate += (delTime*dRate);

    rad0 = ofMap(ofNoise(radNoise * time, noiseOffset.z), 0, 1, rad-radMargin, rad+radMargin);
    r = rad0 * cos(rate * ang);
    
    x = r * cos(ang);
    y = r * sin(ang);
    z = *is3d ? r * sin(0.5*ang+1.0) * cos(0.25*ang+2.0) : 0;
    
    if (*is3d) {
        rotAngle.x = ofLerp(rotAngle.x, speedRotation->x * time, 0.05);
        rotAngle.y = ofLerp(rotAngle.y, speedRotation->y * time, 0.05);
        rotAngle.z = ofLerp(rotAngle.z, speedRotation->z * time, 0.05);
    }
    else {
        rotAngle.x = ofLerp(rotAngle.x, 0, 0.03);
        rotAngle.y = ofLerp(rotAngle.y, 0, 0.03);
        rotAngle.z = ofLerp(rotAngle.z, 0, 0.03);
    }
    
    pts.push_back(ofVec3f(x, y, z));
    while (pts.size() > numPoints) {
        pts.erase(pts.begin());
    }
    if (*isRibbon)
    {
        ribbon.length = numPoints;
        ribbon.color = color;
        ribbon.thickness = lineWidth;
        ribbon.update(ofVec3f(x, y, z));
    }
}

void PolarEq::draw()
{
    ofSetColor(color);
    ofSetLineWidth(lineWidth);
    
    ofRotateX(rotAngle.x);
    ofRotateX(rotAngle.y);
    ofRotateZ(rotAngle.z);

    if (*isRibbon) {
        ribbon.draw();
    }
    else
    {
        ofNoFill();
        ofBeginShape();
        for (int i=0; i<pts.size(); i++) {
            ofCurveVertex(pts[i].x, pts[i].y, pts[i].z);
        }
        ofEndShape();
    }
}

void Polar::setup()
{
    setName("Polar");
    
    nx = 3;
    ny = 3;
    delTime = 1.0;
    numPoints = 12;
    color = ofFloatColor(1, 0.7);
    lineWidth = 2;
    rad = 125;
    radMargin = 0;
    dRateMax = 0.03;
    dAngMax = 1.0;
    angNoise = 0.0004;
    rateNoise = 0.0005;
    radNoise = 0.0004;
    is3d = false;
    isRibbon = false;

    control.addColor("color", &color);
    control.addSlider("lineWidth", &lineWidth, 0.0f, 16.0f);
    control.addSlider("nx", &nx, 1, 12);
    control.addSlider("ny", &ny, 1, 12);
    control.addSlider("delTime", &delTime, 0.0f, 2.0f);
    control.addSlider("numPoints", &numPoints, 3, 100);
    control.addSlider("rad", &rad, 0.0f, 500.0f);
    control.addSlider("radMargin", &radMargin, 0.0f, 300.0f);
    control.addSlider("dRateMax", &dRateMax, 0.0f, 0.1f);
    control.addSlider("dAngMax", &dAngMax, 0.0f, 3.0f);
    control.addSlider("angNoise", &angNoise, 0.0f, 0.0015f);
    control.addSlider("rateNoise", &rateNoise, 0.0f, 0.0015f);
    control.addSlider("radNoise", &radNoise, 0.0f, 0.0015f);
    control.addToggle("3d", &is3d);
    control.addToggle("ribbons", &isRibbon);
    control.addSlider("speedRotation", &speedRotation, ofVec3f(0, 0, 0), ofVec3f(1, 1, 1));
    control.addButton("refresh", this, &Polar::refresh);
    
    managePolarCount();
}

void Polar::managePolarCount()
{
    while (polars.size() > nx * ny) {
        polars.erase(polars.begin());
    }
    while (polars.size() < nx * ny) {
        polars.push_back(new PolarEq(&is3d, &isRibbon, &speedRotation));
    }
}

void Polar::refresh(GuiElementEventArgs &evt)
{
    for (int i=0; i<polars.size(); i++) {
        polars[i]->refresh();
    }
}

void Polar::update()
{
    managePolarCount();
    for (int i=0; i<polars.size(); i++)
    {
        polars[i]->setColor(color);
        polars[i]->setLineWidth(lineWidth);
        polars[i]->setDelTime(delTime);
        polars[i]->setNumPoints(numPoints);
        polars[i]->setRad(rad);
        polars[i]->setRadMargin(radMargin);
        polars[i]->setDRateMax(dRateMax);
        polars[i]->setDAngMax(dAngMax);
        polars[i]->setAngNoise(angNoise);
        polars[i]->setRateNoise(rateNoise);
        polars[i]->setRadNoise(radNoise);
        polars[i]->update();
    }
}

void Polar::draw()
{
    for (int i=0; i<nx; i++)
    {
        for (int j=0; j<ny; j++)
        {
            float x = ofMap(i+0.5, 0, nx, 0, width);
            float y = ofMap(j+0.5, 0, ny, 0, height);
            ofPushMatrix();
            ofTranslate(x, y);
            polars[ofClamp(i*ny + j, 0, polars.size()-1)]->draw();
            ofPopMatrix();
        }
    }
}

PolarEq::~PolarEq()
{
    cam.disableMouseMiddleButton();
    cam.disableMouseInput();
}

Polar::~Polar()
{
    for (int i=0; i<polars.size(); i++) {
        delete polars[i];
    }
    polars.clear();
    ofRemoveListener(ofEvents().update, (Scene*) this, &Scene::update);
}

void Polar::setup(int width, int height)
{
    Scene::setup(width, height);
}

void Polar::draw(int x, int y)
{
    Scene::draw(x, y);
}
