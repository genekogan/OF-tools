#pragma once

#include "Control.h"
#include "Shader.h"
#include "Modifier.h"


class ShaderModifier : public Modifier
{
public:
    
    ShaderModifier() : Modifier()
    {
        type = "Shader";
    }

    void setActive(bool active)
    {
        this->active = active;
        shader->getControl().setActive(active);
    }

    void setup(int width, int height);
    void render(ofFbo *fbo);
    
    GuiPanel & getControl();

    Shader *shader;
};
