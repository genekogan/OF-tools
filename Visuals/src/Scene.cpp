#include "Scene.h"


Scene::Scene()
{
    bgColor.set(0, 0, 0, 1);
    toClear = false;
    upsideDown = false;
    width = ofGetWidth();
    height = ofGetHeight();
    
    control.addToggle("clear", &toClear);
    control.addColor("bgColor", &bgColor);
    
    setName("Scene");
    setAutoUpdate(true);
}

Scene::~Scene()
{
    setAutoUpdate(false);
}

void Scene::setName(string name)
{
    this->name = name;
    control.setName(name);
}

void Scene::setAutoUpdate(bool autoUpdate)
{
    this->autoUpdate = autoUpdate;
    if (autoUpdate) {
        ofAddListener(ofEvents().update, this, &Scene::update);
    }
    else {
        ofRemoveListener(ofEvents().update, this, &Scene::update);
    }
}

void Scene::setup(int width, int height)
{
    this->width = width;
    this->height = height;
    setup();
}

void Scene::update(ofEventArgs &data)
{
    update();
}

void Scene::draw(int x, int y)
{
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(x, y);
    
    if (upsideDown)
    {
        ofTranslate(width, height);
        ofRotate(180);
    }
    
    if (toClear)
    {
        ofClear(0, 0);
    }
    
    if (bgColor.a > 0)
    {
        ofFill();
        ofSetColor(bgColor);
        ofRect(0, 0, width, height);
    }
    
    ofSetColor(255, 255);
    
    draw();
    
    ofPopStyle();
    ofPopMatrix();
}
