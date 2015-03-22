#include "Letters.h"


const char * Letters::alphanum = {
    "0123456789ABCDEFGHIJKLMNOPQR"
    "STUVWXYZabcdefghijklmnopqrst"
    "uvwxyz!@#$%^&*()_+;<>?~`"};


void Letters::setup()
{
    setName("Letters");

    nrows = 30;
    ncols = 40;
    letterRange = ofVec2f(0, 1);
    rate = 1;
    density = 0.5;
    rateNoise = ofVec2f(0.1, 0.1);
    rateNoiseTime = 0.1;
    letterNoise = ofVec2f(0.1, 0.1);
    letterNoiseTime = 0.1;
    color = ofFloatColor(0, 1, 0.1, 1.0);
    noisyLetters = false;
    chinese = false;
    
    control.addSlider("rows", &nrows, 2, 200);
    control.addSlider("cols", &ncols, 2, 200);
    control.addSlider("letterRange", &letterRange, ofVec2f(0, 0), ofVec2f(1,1));
    control.addColor("color", &color);
    control.addSlider("rate", &rate, 1, 10);
    control.addSlider("density", &density, 0.0f, 1.0f);
    control.addSlider("rateNoise", &rateNoise, ofVec2f(0, 0), ofVec2f(0.1, 0.1));
    control.addSlider("rateNoiseTime", &rateNoiseTime, 0.0f, 1.0f);
    control.addSlider("letterNoise", &letterNoise, ofVec2f(0, 0), ofVec2f(0.1, 0.1));
    control.addSlider("letterNoiseTime", &letterNoiseTime, 0.0f, 1.0f);
    control.addToggle("noisyLetters", &noisyLetters);
    control.addToggle("chinese", &chinese);
    
    update();
}

void Letters::update()
{
    if (chars.size() != nrows * ncols)
    {
        chars.resize(nrows * ncols);
        charsC.resize(nrows * ncols);
        font.loadFont("fonts/AndaleMono.ttf", (int) height / nrows);
        fontC.loadFont("fonts/Chinese4.ttf", 0.72 * (int) height / nrows, true, true, true);
    }
    
    if (ofGetFrameNum() % rate == 0)
    {
        int idx = 0;
        float noiseX;
        for (int i=0; i<ncols; i++)
        {
            for (int j=0; j<nrows; j++)
            {
                if (ofNoise(rateNoise.x*i, rateNoise.y*j, rateNoiseTime * ofGetFrameNum()) < density)
                {
                    if (noisyLetters)
                    {
                        noiseX = ofNoise(letterNoise.x*i, letterNoiseTime * ofGetFrameNum(), letterNoise.y*j);
                        chars[idx] = alphanum [ (int) ofMap(noiseX, 0, 1, 81*letterRange.x, 81*letterRange.y) ];
                        charsC[idx] = 38 + (int) ofMap(noiseX, 0, 1, 86 * letterRange.x, 86 * letterRange.y);
                    }
                    else
                    {
                        chars[idx] = alphanum[ (int) ofRandom(81 * letterRange.x, 81 * letterRange.y) ];
                        charsC[idx] = 38 + (int) ofRandom(86 * letterRange.x, 86 * letterRange.y);
                    }
                }
                idx++;
            }
        }
    }
}

void Letters::draw()
{
    int idx = 0;
    float x, y;
    for (int i=0; i<ncols; i++)
    {
        for (int j=0; j<nrows; j++)
        {
            x = ofMap(i, 0, ncols, 0, width);
            y = ofMap(j, 0, nrows, 0, height);
            ofSetColor(color);
            if (chinese) {
                fontC.drawString(ofToString(char(charsC[idx++])), x, y);
            }
            else {
                font.drawString(ofToString(chars[idx++]), x, y);
            }
        }
    }
}

void Letters::setup(int width, int height)
{
    Scene::setup(width, height);
}

void Letters::draw(int x, int y)
{
    Scene::draw(x, y);
}
