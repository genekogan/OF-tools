#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "TimeFunction.h"
#include "ofxBlurShader.h"


class Bubbles : public Scene
{
public:
    Bubbles() {setName("Bubbles");}
    ~Bubbles();  
    void setup(int width, int height, bool clearControls=true);
    void update();
    void draw(int x, int y);
    
private:
    
    class BubbleCreator : public ofThread
    {
    public:
        void setup(vector<TimeFunction<ofVec2f> *> &position,
                   vector<ofVec3f> &colorMargin,
                   vector<float> &blurLevel,
                   vector<float> &alpha,
                   vector<float> &size,
                   int &numBubbles,
                   int width, int height);
        void setRunning(bool b);
        
    protected:
        void threadedFunction();
        
        vector<TimeFunction<ofVec2f> *> *position;
        vector<ofVec3f> *colorMargin;
        vector<float> *blurLevel;
        vector<float> *alpha;
        vector<float> *size;
        int *numBubbles;
        int width, height;
    };

    void fboParametersChanged(GuiSliderEventArgs<int> &e);
    void setupBubblesFbo();
    
    ofxBlurShader blur;
    vector<ofFbo> bubbleFbo;

    // main parameters
    float speed;
    int numBubbles;
    int maxSizeInitial;
    int maxSize;
    ofFloatColor baseColor;
    ofVec3f varColor;
    ofVec2f positionNoiseMargin;
    ofVec2f positionNoiseFactor;
    ofVec2f positionSineMargin;
    ofVec2f positionSineFreq;
    ofVec2f positionTimeConstant;
    int maxPasses;
    int blurAmt;
    
    // for each bubble
    vector<TimeFunction<ofVec2f> *> position;
    vector<ofVec3f> colorMargin;
    vector<float> blurLevel;
    vector<float> alpha;
    vector<float> size;
    
    float time;
    BubbleCreator bubbleCreator;
};