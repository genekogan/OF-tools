#pragma once

#include "ofMain.h"
#include "Scene.h"


#define FORCE_RESOLUTION 16


class Streak
{
public:
    void setup(float x, float y, int maxAge, float speed,
               int maxThickness, float strokeAlpha,
               int width, int height, ofFloatColor color);
    void update(ofPoint vel);
    void draw();
    bool isActive();

    ofVec2f pos, prev;
    float age, maxAge, speed, speedVar, maxThickness, strokeAlpha;
    ofFloatColor color;
    int width, height;
};


class Rivers : public Scene
{
public:
    Rivers() {setName("Rivers");}

    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);

    ofVec2f force[FORCE_RESOLUTION][FORCE_RESOLUTION];
    
    
private:
    
    void setupForceField(GuiButtonEventArgs &evt);
    void addNewStreak();
    void setupStreak(Streak &streak);
    
    int numStreaks;
    float complexity;
    float baseSpeed;
    float speedVar;
    int minAge;
    int maxAge;
    int refreshAlpha;
    int streakAlpha;
    float maxThickness;
    ofFloatColor color;
    int colorVar;
    
    vector<Streak> streaks;
    //ofVec2f force[FORCE_RESOLUTION][FORCE_RESOLUTION];
    float noiseSeed;
};
