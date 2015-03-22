#pragma once

#include "ofMain.h"
#include "Control.h"


class Scene
{
public:
    
    Scene();
    virtual ~Scene();

    string getName() {return name;}
    void setName(string name);
    
    bool getAutoUpdate() {return autoUpdate;}
    void setAutoUpdate(bool autoUpdate);
    
    void setGuiPosition(int x, int y) {control.setPosition(x, y);}
    GuiPanel & getControl() {return control;}
    
    void setUpsideDown(bool upsideDown) {this->upsideDown = upsideDown;}
    
    virtual void setup() { }
    virtual void update() { }
    virtual void draw() { }
    
    virtual void setup(int width, int height);
    void update(ofEventArgs &data);
    virtual void draw(int x, int y);
    
protected:
    
    GuiPanel control;
    string name;
    int width, height;
    ofFloatColor bgColor;
    bool toClear;
    bool upsideDown;
    bool autoUpdate;
};

