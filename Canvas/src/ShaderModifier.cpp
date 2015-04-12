#include "ShaderModifier.h"


void ShaderModifier::setup(int width, int height)
{
    Modifier::setup(width, height);
    shader = new Shader(false);
    shader->setup(width, height, false);
}

GuiPanel & ShaderModifier::getControl()
{
    return shader->getControl();
}

void ShaderModifier::render(ofFbo *fbo)
{
    shader->setTexture(fbo);
    shader->update();
    shader->draw(0, 0);
}
