/*
 Based on [ofxBpm](https://github.com/mirrorboy714/ofxBpm)
 */

#pragma once

#include "ofMain.h"

class Bpm : private ofThread
{
public:
    static const int BPM_TICK = 960;
    
    explicit Bpm(float bpm = 120.0, int beatPerBar = 4);
    
    void start();
    void stop();
    void reset();
    
    void setBpm(float bpm);
    float getBpm() const;
    
    void setBeatPerBar(int beatPerBar);
    
    bool getPlaying() const;
    
    ofEvent<void> beatEvent;
    
private:
    
    void threadedFunction();
    
    inline int getCountOfTick() const;
    
    float bpm;
    bool isTick;
    bool isPlaying;
    int countOfTick;
    long totalTime;
    long durationOfTick;
    long remainderOfTick;
    
    float preTime;
    int beatPerBar;
};
