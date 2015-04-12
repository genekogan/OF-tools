#include "MediaPlayer.h"


void MediaPlayer::setup(int width, int height, bool clearControls)
{
    Scene::setup(width, height, clearControls);

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
}

void MediaPlayer::update()
{
    if (mode == VIDEO) {
        player[active].update();
    }
}

void MediaPlayer::draw(int x, int y)
{
    Scene::beginDraw(x, y);

    ofSetColor(tint);
    
    if (mode == VIDEO)
    {
        if (stretched) {
            player[active].draw(0, 0, width, height);
        }
        else {
            player[active].draw(x0, y0, w, h);
        }
    }
    else if (mode == IMAGE)
    {
        if (stretched) {
            images[active].draw(0, 0, width, height);
        }
        else {
            images[active].draw(x0, y0, w, h);
        }
    }
    
    Scene::endDraw();
}

void MediaPlayer::loadMovie(string path)
{
    int idx = player.size();
    player.resize(player.size() + 1);
    
    player[idx].loadMovie(path);
    player[idx].setVolume(0);
    player[idx].setLoopState(OF_LOOP_NORMAL);
    player[idx].play();
    player[idx].update();
    player[idx].setPaused(true);
    if (player.size() + images.size() == 1)  selectVideo(0);
    
    vector<string> pathSplit = ofSplitString(path, "/");
    menuPlayers->addToggle(pathSplit[pathSplit.size()-1]);
}

void MediaPlayer::loadImage(string path)
{
    int idx = images.size();
    images.resize(images.size() + 1);
    
    images[idx].loadImage(path);
    if (player.size() + images.size() == 1)  selectImage(0);
    
    vector<string> pathSplit = ofSplitString(path, "/");
    menuImages->addToggle(pathSplit[pathSplit.size()-1]);
}

void MediaPlayer::selectVideo(int index)
{
    if (index > player.size() - 1)  return;
    mode = VIDEO;
    player[index].setPaused(paused);
    player[index].setSpeed(speed);
    active = index;
    setPicturePosition(player[active].getWidth(), player[active].getHeight());
    control.getElement("Speed")->setActive(true);
    control.getElement("Paused")->setActive(true);
    control.getElement("Jump")->setActive(true);
    control.getElement("Jump random")->setActive(true);
}

void MediaPlayer::selectImage(int index)
{
    if (index > images.size() - 1)  return;
    mode = IMAGE;
    player[active].setPaused(true);
    active = index;
    setPicturePosition(images[active].getWidth(), images[active].getHeight());
    control.getElement("Speed")->setActive(false);
    control.getElement("Paused")->setActive(false);
    control.getElement("Jump")->setActive(false);
    control.getElement("Jump random")->setActive(false);
}

void MediaPlayer::setPicturePosition(int dimx, int dimy)
{
    if ((float) width / height > (float) dimx / dimy)
    {
        h = height;
        w = dimx * h / dimy;
        x0 = 0.5 * (width - w);
        y0 = 0;
    }
    else
    {
        w = width;
        h = dimy * w / dimx;
        x0 = 0;
        y0 = 0.5 * (height - h);
    }
}

void MediaPlayer::eventLoadMedia(GuiButtonEventArgs &evt)
{
    ofFileDialogResult file = ofSystemLoadDialog();
    if (file.bSuccess)
    {
        vector<string> fileSplit = ofSplitString(file.filePath, ".");
        string extension = ofToLower(fileSplit[fileSplit.size()-1]);
        if (extension == "png" || extension == "jpg" || extension == "jpeg") {
            loadImage(file.filePath);
        }
        else if (extension == "mov" || extension == "mp4")
        {
            loadMovie(file.filePath);
        }
    }
}

void MediaPlayer::eventChooseVideo(GuiMenuEventArgs &evt)
{
    selectVideo(evt.index);
}

void MediaPlayer::eventChooseImage(GuiMenuEventArgs &evt)
{
    selectImage(evt.index);
}

void MediaPlayer::eventSetPaused(GuiButtonEventArgs &evt)
{
    if (active < player.size()) {
        player[active].setPaused(paused);
    }
}

void MediaPlayer::eventSetSpeed(GuiSliderEventArgs<float> &evt)
{
    if (active < player.size()) {
        player[active].setSpeed(speed);
    }
}

void MediaPlayer::eventJumpFrame(GuiSliderEventArgs<float> &evt)
{
    if (active < player.size()) {
        player[active].setPosition(evt.value);
    }
}

void MediaPlayer::eventJumpFrameRandom(GuiButtonEventArgs &evt)
{
    if (active < player.size()) {
        player[active].setPosition(ofRandom(1));
    }
}
