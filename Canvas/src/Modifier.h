#pragma once

#include "Control.h"
#include "Shader.h"


class Modifier
{
public:
    
    Modifier()
    {
        type = "Modifier";
        setActive(false);
        index = 0;
    }
    
    void setIndex(int index)
    {
        this->index = index;
    }
    
    int getIndex() {return index;}
    
    string getDisplayText()
    {
        return index == 0 ? getType() : getType() + "("+ofToString(index+1)+")";
    }
    
    virtual void setup(int width, int height)
    {
        this->width = width;
        this->height = height;
    }
    
    virtual void setActive(bool active)
    {
        this->active = active;
    }
    
    bool getActive() {return active;}
    
    virtual GuiPanel & getControl() { }
    
    virtual void render(ofFbo *fbo) { }
    
    string getType() {return type;}
    
    int width, height;
    string type;
    bool active;
    int index;
};
