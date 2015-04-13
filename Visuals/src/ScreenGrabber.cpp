#include "ScreenGrabber.h"


void ScreenGrabber::setup(int width, int height, bool clearControls)
{
    Scene::setup(width, height, clearControls);
    
    gwidth = width;
    gheight = height;
    gx = 0;
    gy = 0;
    retina = true;

    grabber.setup(gwidth, gheight, retina);

    control.addSlider("gx", &gx, 0, width);
    control.addSlider("gy", &gy, 0, height);
    control.addSlider("width", &gwidth, 0, ofGetScreenWidth(), this, &ScreenGrabber::eventSetWidth);
    control.addSlider("height", &gheight, 0, ofGetScreenHeight(), this, &ScreenGrabber::eventSetHeight);
    control.addToggle("retina", &retina, this, &ScreenGrabber::eventSetRetina);
}

void ScreenGrabber::update()
{
    grabber.grabScreen(gx, gy);
}

void ScreenGrabber::draw(int x, int y)
{
    Scene::beginDraw(x, y);
    grabber.draw(0, 0, width, height);
    Scene::endDraw();
}

void ScreenGrabber::eventSetWidth(GuiSliderEventArgs<int> &evt)
{
    grabber.setup(gwidth, gheight, retina);
}

void ScreenGrabber::eventSetHeight(GuiSliderEventArgs<int> &evt)
{
    grabber.setup(gwidth, gheight, retina);
}

void ScreenGrabber::eventSetRetina(GuiButtonEventArgs &evt)
{
    grabber.setup(gwidth, gheight, retina);
}
