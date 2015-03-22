#pragma once

#include "ofMain.h"
#include "Scene.h"


class Letters : public Scene
{
public:
    void setup();
    void update();
    void draw();

    void setup(int width, int height);
    void draw(int x, int y);

private:
    
    int nrows;
    int ncols;
    ofVec2f letterRange;
    int rate;
    float density;
    ofFloatColor color;
    ofVec2f rateNoise;
    float rateNoiseTime;
    ofVec2f letterNoise;
    float letterNoiseTime;
    bool noisyLetters;
    bool chinese;
    
    vector<char> chars;
    vector<int> charsC;
    ofTrueTypeFont font, fontC;
    static const char * alphanum;
};
