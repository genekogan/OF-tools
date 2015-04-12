#pragma once

#include "ofMain.h"
#include "Scene.h"


class MediaPlayer : public Scene
{
public:
    enum MediaMode { NONE, VIDEO, IMAGE };
    
    MediaPlayer() {setName("MediaPlayer");}
    
    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);
    
    void loadMovie(string path);
    void loadImage(string path);
    
    void selectVideo(int index);
    void selectImage(int index);

private:
    
    void setPicturePosition(int dimx, int dimy);

    void eventLoadMedia(GuiButtonEventArgs &evt);
    void eventChooseVideo(GuiMenuEventArgs &evt);
    void eventChooseImage(GuiMenuEventArgs &evt);
    void eventSetPaused(GuiButtonEventArgs &evt);
    void eventSetSpeed(GuiSliderEventArgs<float> &evt);
    void eventJumpFrame(GuiSliderEventArgs<float> &evt);
    void eventJumpFrameRandom(GuiButtonEventArgs &evt);
    
    MediaMode mode;
    vector<ofVideoPlayer> player;
    vector<ofImage> images;
    
    int active;
    int x0, y0, w, h;
    float speed;
    float jumpPosition;
    bool paused;
    bool stretched;
    ofFloatColor tint;

    GuiMenu *menuPlayers, *menuImages;
};



