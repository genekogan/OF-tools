#pragma once

#include "ofMain.h"
#include "ofxQuad.h"



class Quad
{
public:
    Quad();
    
    void setDebug(bool debugging, bool debugMe);
    void drawDebug();
    
    void begin();
    void end();
    
    ofPoint getInputCorner(int idx);
    ofPoint getOutputCorner(int idx);
    
    void setInputCorner(int idx, int x, int y);
    void setOutputCorner(int idx, int x, int y);
    
    void moveOutputCorner(int x, int y);
    void grabOutputCorner(int x, int y);
    void releaseOutputCorner();
    
private:
    
    bool debugging, debugMe;
    int selectedCorner;
    ofxQuad quad;
    ofPoint lastGrabbedPoint;
};



class ProjectionMapping
{
public:
    ProjectionMapping();
    
    bool getMouseEventsAuto() {return mouseAuto;}
    void setMouseEventsAuto(bool mouseAuto);
    
    void setMouseResolution(int x, int y);
    
    void addQuad(int width, int height);
    Quad * getQuad(int idx) {return quads[idx];}
    
    void begin(int idx) { quads[idx]->begin(); }
    void end(int idx)   { quads[idx]->end(); }
    
    void mousePressed(ofMouseEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void mouseReleased(ofMouseEventArgs &e);
    void keyPressed(ofKeyEventArgs &e);
    
    void setDebug(bool debug);
    void toggleDebug();
    void toggleSelectedQuad(int shift=1);
    bool getDebug() { return debug; }
    
    void loadPreset(string path);
    void savePreset(string path);
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
private:
    
    vector<Quad *> quads;
    bool debug;
    bool mouseAuto;
    int selected;
    ofPoint mouseResolution;
};

