#include "ofApp.h"


void CustomScene::setup()
{
    setName("Custom");
    
    control.addSlider("radius", &radius, 50.0f, 250.0f);
    control.addSlider("resolution", &resolution, 3, 16);
    control.addSlider("line width", &lineWidth, 4.0f, 12.0f);
    
    control.addToggle("filled", &filled);
    control.addSlider("position", &position, ofVec2f(100, 100), ofVec2f(width, height));
    control.addColor("my color", &myColor);
    
    GuiMenu *menu1 = control.addMenu("multi choice menu", this, &CustomScene::multiChoiceMenuSelect);
    menu1->setMultipleChoice(true);
    menu1->addToggle("new york", &b1);
    menu1->addToggle("los angeles", &b2);
    menu1->addToggle("chicago", &b3);
    
    GuiMenu *menu2 = control.addMenu("single choice menu", this, &CustomScene::menuSelect);
    menu2->addToggle("kamusta mundo");
    menu2->addToggle("xin chao the gioi");
    menu2->addToggle("namaste varlda");
    menu2->addToggle("suesday piphoplok");
    menu2->addToggle("suwati lok");
}

void CustomScene::update()
{
    
}

void CustomScene::draw()
{
    ofSetCircleResolution(resolution);
    filled ? ofFill() : ofNoFill();
    ofSetColor(myColor);
    ofSetLineWidth(lineWidth);
    ofCircle(position.x, position.y, radius);
    
    ofSetColor(0);
    ofDrawBitmapString(greeting+"\n"+city, position.x, position.y);
    
    ofSetColor(255);
}

void CustomScene::menuSelect(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "menu selection event: " + e.name + " = " + ofToString(e.value));
    greeting = e.name;
}

void CustomScene::multiChoiceMenuSelect(GuiElementEventArgs & e)
{
    ofLog(OF_LOG_NOTICE, "panel menu event: " + e.name + " = " + ofToString(e.value));
    city  = b1 ? "New York " : "";
    city += b2 ? "Los Angeles " : "";
    city += b3 ? "Chicago" : "";
}

void CustomScene::setup(int width, int height)
{
    Scene::setup(width, height);
}

void CustomScene::draw(int x, int y)
{
    Scene::draw(x, y);
}



//=============================================

void ofApp::setup()
{
    custom1.setup(600, 400);
    custom2.setup(600, 400);
    
    meta.setName("Visuals");
    meta.addWidget(&custom1.getControl());
    meta.addWidget(&custom2.getControl());
}

void ofApp::update()
{

}

void ofApp::draw()
{
    custom1.draw(580, 5);
    custom2.draw(580, 460);
}

void ofApp::keyPressed(int key)
{

}
