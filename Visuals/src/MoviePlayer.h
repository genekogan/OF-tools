#pragma once

#include "ofMain.h"
#include "Scene.h"



class MoviePlayer : public Scene
{
public:
    enum MediaMode { IMAGE, MOVIE };
    
    void setup();
    void update();
    void draw();

    void loadMovie(string path);
    void loadImage(string path);
    
    void jumpBack(string &s);
    void jumpRandom(string &s);
    void selectMedia(string &s);
    void chooseMovie(string &s);
    void chooseImage(string &s);
    
    void triggerImage(int idx);
    void triggerImage();
    void triggerMovie(int idx);
    void triggerMovie();
    void triggerCallback(string &s) { mode == MOVIE ? triggerMovie() : triggerImage(); }

    void toggleClipsHidden(string & s);
    void setClipsHidden(bool clipsHidden);
    
    void clearMovies();
    void setupControl();
    
private:
    
    vector<ofVideoPlayer> player;
    vector<string> moviePaths, imagePaths;
    int active;
    
    //ofImage img;
    vector<ofImage> img;
    
    int w, h;
    float speed;
    float oldSpeed;
    bool playing;
    
    MediaMode mode;
    bool centeredHoriz;
    
    bool clipsHidden;
    
    float alpha;
    
    int idxLoad;
    int idxImgLoad;
};



