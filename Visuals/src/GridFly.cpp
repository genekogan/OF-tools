#include "GridFly.h"


void GridFly::setup()
{
    setName("GridFly");
    
    nx = 100;
    ny = 100;
    length = ofVec2f(2000, 2000);
    margin = 100;
    noiseFactor = ofVec2f(0.001, 0.001);
    speed = 20;
    angle = 90;
    axis = ofVec3f(1, 0, 0);
    color = ofFloatColor(1, 0.4);

    control.addSlider("nx", &nx, 10, 400);
    control.addSlider("ny", &ny, 10, 400);
    control.addSlider("length", &length, ofVec2f(100,100), ofVec2f(4000,4000));
    control.addSlider("margin", &margin, 0.0f, 1000.0f);
    control.addSlider("noiseFactor", &noiseFactor, ofVec2f(0, 0), ofVec2f(0.01, 0.01));
    control.addSlider("speed", &speed, 0.0f, 100.0f);
    control.addSlider("angle", &angle, 0.0f, 360.0f);
    control.addSlider("axis", &axis, ofVec3f(0, -1, -1), ofVec3f(1, 1, 1));
    control.addColor("color", &color);
    
    vertices = &mesh.getVertices();
    for (int j=0; j<ny; j++)
    {
        addRow();
        y++;
    }
    minY = -length.y;
    dy = length.y / ny;
    y = 0;
}

void GridFly::update()
{
    Z += speed;
    
    cam.resetTransform();
    cam.rotate(angle, axis.x, axis.y, axis.z);
    cam.setPosition(camPosition.x, Z, camPosition.y);
    
    if (vertices->size() > nx*ny*6)
    {
        vertices->erase(vertices->begin(), vertices->begin() + (vertices->size() - nx*ny));
    }
    while (cam.getPosition().y > minY)
    {
        vertices->erase(vertices->begin(), vertices->begin() + nx*6);
        minY = vertices->at(0).y;
        addRow();
        y++;
    }
}

void GridFly::addRow()
{
    for (int x=0; x<nx; x++)
    {
        float x1 = ofMap(x, 0, nx, -0.5*length.x, 0.5*length.x);
        float x2 = ofMap(x+1, 0, nx, -0.5*length.x, 0.5*length.x);
        float y1 = ofMap(y, 0, ny, -length.y, 0);
        float y2 = ofMap(y+1, 0, ny, -length.y, 0);
        
        float z11 = -margin * (-1.0 + 2.0*ofNoise(noiseFactor.x * x1, noiseFactor.y * y1));
        float z12 = -margin * (-1.0 + 2.0*ofNoise(noiseFactor.x * x1, noiseFactor.y * y2));
        float z21 = -margin * (-1.0 + 2.0*ofNoise(noiseFactor.x * x2, noiseFactor.y * y1));
        float z22 = -margin * (-1.0 + 2.0*ofNoise(noiseFactor.x * x2, noiseFactor.y * y2));
        
        mesh.addVertex(ofVec3f(x1, y1, z11));
        mesh.addVertex(ofVec3f(x2, y1, z21));
        mesh.addVertex(ofVec3f(x2, y2, z22));
        
        mesh.addVertex(ofVec3f(x1, y1, z11));
        mesh.addVertex(ofVec3f(x1, y2, z12));
        mesh.addVertex(ofVec3f(x2, y2, z22));
    }
}

void GridFly::draw()
{
    ofNoFill();
    ofSetColor(color);
    cam.begin();
    mesh.drawWireframe();
    cam.end();
}

GridFly::~GridFly()
{
    cam.disableOrtho();
    cam.disableMouseMiddleButton();
    cam.disableMouseInput();
}

void GridFly::setup(int width, int height)
{
    Scene::setup(width, height);
}

void GridFly::draw(int x, int y)
{
    Scene::draw(x, y);
}
