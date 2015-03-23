#pragma once

#include "Control.h"
#include "Shader.h"


class Modifier
{
public:
    
    
    
    virtual void setup(int width, int height)
    {
        this->width = width;
        this->height = height;
    }
    
    virtual GuiPanel & getControl() { }
    
    virtual void render(ofFbo *fbo) { }
    
    int width, height;
};
