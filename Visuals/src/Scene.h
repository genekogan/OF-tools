#pragma once

#include "ofMain.h"
#include "Control.h"


class Scene
{
public:
    Scene();
    virtual ~Scene();

    void setName(string name);
    string getName() {return name;}
    
    void setAutoUpdate(bool autoUpdate);
    bool getAutoUpdate() {return autoUpdate;}
    
    void setGuiPosition(int x, int y) {control.setPosition(x, y);}
    GuiPanel & getControl() {return control;}
    
    void setUpsideDown(bool upsideDown) {this->upsideDown = upsideDown;}
    bool getUpsideDown() {return upsideDown;}
    
    virtual void setup(int width, int height, bool clearControls=true);
	virtual void update();
    virtual void draw(int x, int y);

protected:
	
    void beginDraw(int x, int y);
	void endDraw();
    
    void update(ofEventArgs &data);
    
    GuiPanel control;
    string name;
    int width, height;
    ofFloatColor bgColor;
    bool toClear;
    bool upsideDown;
    bool autoUpdate;
};

