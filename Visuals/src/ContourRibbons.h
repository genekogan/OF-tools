#pragma once

#include "ofMain.h"

#include "OpenNI.h"


class Contour
{
public:
    Contour(vector<ofVec2f> & points, ofPoint center, int label);
    void setPoints(vector<ofVec2f> & points, ofPoint center);
    void draw();
    
    vector<ofVec2f> points;
    ofPoint center;
    int label;
    int age;
    ofColor color;
};



class Ribbon {
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




class ContourRibbons
{
public:
    void setup();
    void update();
    
    
    void draw();

    void recordContours(OpenNI & openNi);

    void manageContours();
    void manageRibbons();

    void renderRibbons();
    
    
    //OpenNi *openNi;
    //ofxKinectProjectorToolkit *kpt;
    
    
    // tracking
    vector<Contour *> contours;
    vector<vector<ofVec2f> > currentContours;

    bool calibrated = false;
    int width, height;
    
    // ribbons
    vector<Ribbon *> ribbons;
    vector<int> labels;
    
    GuiPanel panel;
    
    // ribbons
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
    int threshold;
    int frameSkip;
    
    
};