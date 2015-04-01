#include "ofApp.h"

void ofApp::setup()
{
   // ofSetBackgroundAuto(false);

    canvas.setup("Canvas", 800, 600);
    
    canvas.addCreator(new Amoeba());
    canvas.addCreator(new Agents());
    canvas.addCreator(new Bubbles());
    canvas.addCreator(new Cubes());
    canvas.addCreator(new DebugScreen());
    canvas.addCreator(new GridFly());
    canvas.addCreator(new Letters());
    canvas.addCreator(new Meshy());
    canvas.addCreator(new Polar());
    canvas.addCreator(new Rivers());
    canvas.addCreator(new Shader(true));
    canvas.addCreator(new Subdivide());
    
    canvas.addModifier();
    canvas.addPostGlitch();
    canvas.addPostProcessing();
    canvas.addPostFX();
}

void ofApp::update()
{
    canvas.update();
}

void ofApp::draw()
{
    canvas.draw(300, 50);
}

void ofApp::keyPressed(int key)
{

}
