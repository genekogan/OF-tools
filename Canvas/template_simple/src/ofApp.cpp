#include "ofApp.h"

void ofApp::setup()
{
    // ofSetBackgroundAuto(false);
    //ofSetFrameRate(4);
    
    canvas.setup("Canvas", 800, 600);
    
    canvas.setGuiDisplayMode(Canvas::EXPANDED_ATTACHED);
    
    canvas.addCreator(new Amoeba());
    canvas.addCreator(new Agents());
    canvas.addCreator(new Bubbles());
    canvas.addCreator(new Cubes());
    canvas.addCreator(new DebugScreen());
    canvas.addCreator(new GridFly());
    canvas.addCreator(new Letters());
    canvas.addCreator(new Meshy());
    canvas.addCreator(new MediaPlayer());
    canvas.addCreator(new Polar());
    canvas.addCreator(new Rivers());
    canvas.addCreator(new ScreenGrabber());
    canvas.addCreator(new Shader(true));
    canvas.addCreator(new Subdivide());
    canvas.addCreator(new Syphon());
    
    
    //    canvas.addShaderModifier();
    //    canvas.addPostGlitch();
    //    canvas.addPostProcessing();
    //    canvas.addPostFX();
    
    /*
     vector<ParameterBase*> pm = panel.getParameters();
     for (auto p : pm) {
     cout << p->getName() << " ---- " << p->getOscAddress() << endl;
     }
     */
    
    
    
    //    osc.addParameters(canvas.getControl().getName(), canvas.getControl().getParameters());
    //    osc.addParameters(widget.getName(), widget.getParameters());
    
    //    osc.addParameter(v);
    //    osc.addParameter(v2);
    
    //    vector<ParameterBase*> & params = canvas.getControl().getParameters();
    
    /*
     vector<ParameterBase*> & params = canvas.getCreatorPanel().getParameters();
     for (auto p : params) {
     cout << " -> " << p->getName() << endl;
     }
     */
    
    vector<ParameterBase*> parameters;
    canvas.getCreatorPanel().getParameters(parameters);
    
    /// osc.addParameters("CreatorPanel", parameters);
    
    // osc.setPosition(600, 10);
    
}

void ofApp::update()
{
    // osc.update();
    
    canvas.update();
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void ofApp::draw()
{
    canvas.draw(ofGetWidth() - 810, ofGetHeight() - 610);
    //  osc.draw();
}

void ofApp::keyPressed(int key)
{
    if (key=='1') {
        ofSetVerticalSync(true);
    }
    if (key=='2') {
        ofSetVerticalSync(false);
    }
    if (key=='g') {
        canvas.toggleGuiVisible();
    }
    
}
