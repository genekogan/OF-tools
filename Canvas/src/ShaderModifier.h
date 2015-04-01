#pragma once

#include "Control.h"
#include "Shader.h"
#include "Modifier.h"



class ShaderModifier : public Modifier
{
public:
    void setup(int width, int height)
    {
        Modifier::setup(width, height);
        
        shader = new Shader(false);
        shader->setup(width, height, false);
        //        shader->getControl().setPosition(ofRandom(200, 1000), ofRandom(0, 500));
        //shader->getControl().disableControlRow();
    }
    
    GuiPanel & getControl()
    {
        return shader->getControl();
    }

    void render(ofFbo *fbo)
    {
        shader->setTexture(fbo);
        shader->update();
        shader->draw(0, 0);
    }
    
    Shader *shader;
};
