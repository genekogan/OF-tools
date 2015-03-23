#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "ofxTwistedRibbon.h"


class PolarEq
{
public:
    ~PolarEq();
    PolarEq(bool *is3d, bool *isRibbon, ofVec3f *speedRotation);
    
    void refresh();
    void update();
    void draw();
    
    void setDelTime(float delTime) { this->delTime = delTime; }
    void setNumPoints(int n) { this->numPoints = n; }
    
    void setColor(ofColor a) { this->color = a; }
    void setLineWidth(float a) { this->lineWidth = a; }
    
    void setRad(float rad) { this->rad = rad; }
    void setRadMargin(float radMargin) { this->radMargin = radMargin; }
    void setDRateMax(float dRateMax) { this->dRateMax = dRateMax; }
    void setDAngMax(float dAngMax) { this->dAngMax = dAngMax; }

    void setAngNoise(float angNoise) { this->angNoise = angNoise; }
    void setRateNoise(float rateNoise) { this->rateNoise = rateNoise; }
    void setRadNoise(float radNoise) { this->radNoise = radNoise; }
    
private:
    
    vector<ofVec3f> pts;

    ofColor color;
    float lineWidth;

    int numPoints;
    int age;

    float time, delTime;
    float rad, rad0, radMargin;
    float ang, dAng;
    float rate, dRate;
    float dAngMax, dRateMax;
    float angNoise, rateNoise, radNoise;
    ofVec3f noiseOffset, rotAngle;
    
    bool *is3d, *isRibbon;
    ofVec3f *speedRotation;
    
    ofxTwistedRibbon ribbon;
    ofEasyCam cam;
};



class Polar : public Scene
{
public:
    Polar() {setName("Polar");}
    ~Polar();
    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);
    
private:
    void managePolarCount();
    void refresh(GuiElementEventArgs &evt);
    
    vector<PolarEq *> polars;
    
    int nx, ny;
    float delTime;
    ofFloatColor color;
    float lineWidth;
    int numPoints;
    float rad, radMargin;
    float dRateMax, dAngMax;
    float angNoise, rateNoise, radNoise;
    bool is3d;
    bool isRibbon;
    ofVec3f speedRotation;
};
