#include "ScreenGrabber.h"


void ScreenGrabber::setup(int width, int height, bool clearControls)
{
    Scene::setup(width, height, clearControls);

    grabber.setup(ofGetWidth(), ofGetHeight());

    /*
    mode = NONE;
    active = 0;
    paused = false;
    speed = 1.0;
    
    control.addButton("Load media", this, &MediaPlayer::eventLoadMedia);
    menuPlayers = control.addMenu("choose video", this, &MediaPlayer::eventChooseVideo);
    menuImages = control.addMenu("choose image", this, &MediaPlayer::eventChooseImage);
    
    control.addToggle("Stretched", &stretched);
    control.addSlider("Speed", &speed, 0.0f, 4.0f, this, &MediaPlayer::eventSetSpeed)->setActive(false);
    control.addToggle("Paused", &paused, this, &MediaPlayer::eventSetPaused)->setActive(false);
    control.addSlider("Jump", &jumpPosition, 0.0f, 1.0f, this, &MediaPlayer::eventJumpFrame)->setActive(false);
    control.addButton("Jump random", this, &MediaPlayer::eventJumpFrameRandom)->setActive(false);
    control.addColor("Tint", &tint);
     */
}

void ScreenGrabber::update()
{
    grabber.grabScreen(0, 0);
}

void ScreenGrabber::draw(int x, int y)
{
    Scene::beginDraw(x, y);
    
    //ofTexture tex = grabber.getTextureReference();
    grabber.draw(0, 0);
    
    Scene::endDraw();
}
