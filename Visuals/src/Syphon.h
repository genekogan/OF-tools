#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "ofxSyphon.h"


/*
 requires Syphon Framework added to build phases of app
*/

class Syphon : public Scene
{
public:
    
    Syphon() : Scene() {setName("Syphon");}
    
    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);

    void setClient(string serverName, string appName);

private:
    ofxSyphonClient client;
};



