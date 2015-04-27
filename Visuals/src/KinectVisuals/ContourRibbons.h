#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "ContourVisual.h"


class Ribbon
{
public:
    Ribbon(Contour *contour,
           int maxAge, int speed, int length, int skip,
           int margin, float noiseFactor, float ageFactor,
           float lineWidth, int maxAlpha, int updateRate,
           float lerpRate, float dilate, bool curved, bool match);
    
    void update();
    void addPoint(int p);
    void draw();
    
    Contour *getContour() {return contour;}
    bool getActive() {return active;}
    
private:
    
    int idx, idxMatched;
    int age, maxAge;
    int speed;
    int length;
    int skip;
    bool active;
    int margin;
    float lineWidth;
    int maxAlpha;
    float noiseFactor, ageFactor;
    int updateRate;
    float lerpRate;
    float dilate;
    bool curved, match;
    
    Contour *contour;
    vector<ofVec2f> points;
    vector<int> lookup;
    vector<float> lookupMatched;
};


class ContourRibbons : public ContourVisual
{
public:
    void setup(int width, int height);
    void update(OpenNI & openNi);
    void draw();
    
private:
    
    void manageContours();
    void manageRibbons();
    
    vector<Ribbon *> ribbons;

    int maxAgeMin, maxAgeMax;
    int speedMin, speedMax;
    int lengthMin, lengthMax;
    int skipMin, skipMax;
    int marginMin, marginMax;
    float noiseFactorMin, noiseFactorMax;
    float ageFactorMin, ageFactorMax;
    float lineWidthMin, lineWidthMax;
    int maxAlphaMin, maxAlphaMax;
    float lerpRateMin, lerpRateMax;
    int updateRateMin, updateRateMax;
    float dilate;
    bool curved, match;
    int numNew;
    int frameSkip;
    ofFloatColor bgColor;
    
};
