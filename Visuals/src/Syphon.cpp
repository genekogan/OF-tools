#include "Syphon.h"


void Syphon::setup(int width, int height, bool clearControls)
{
    Scene::setup(width, height, clearControls);
    client.setup();
}

void Syphon::setClient(string serverName, string appName)
{
    client.set(serverName, appName);
}

void Syphon::update() {
}

void Syphon::draw(int x, int y)
{
    Scene::beginDraw(x, y);
    client.draw(0, 0, width, height);
    Scene::endDraw();
}

