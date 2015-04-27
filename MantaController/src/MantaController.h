#pragma once

#include "ofMain.h"
#include "ofxManta.h"
#include "ofxConvexHull.h"
#include "Control.h"


class MantaController : public ofxManta
{
public:
    MantaController();
    virtual ~MantaController();
    
    GuiPanel & getControl() {return panel;}
    
    void setMouseActive(bool active);
    void setVisible(bool visible);
    void toggleVisible() {setVisible(!visible);}
    
    bool setup();
    void update();
    void draw(int x, int y, int width);
    void drawStats(int x, int y, int w);
    
    
    float & getVelocityLerpRate() {return velocityLerpRate;}
    void setVelocityLerpRate(float v) {velocityLerpRate=v;}
    
    
    void mousePressed(ofMouseEventArgs &evt);
    void mouseDragged(ofMouseEventArgs &evt);
    void mouseReleased(ofMouseEventArgs &evt);
    void keyPressed(ofKeyEventArgs &e);
    void keyReleased(ofKeyEventArgs &e);
    void setMouseResponders();
    
    ofPoint getPositionAtPad(int row, int col);
    
    
    int getSizeSelection();
    void getMantaElementsInBox(int x, int y);
    
    
    ofRectangle padPositions[48], sliderPositions[2], buttonPositions[4];
    ofRectangle mainDrawRect, statsDrawRect;//, paramSwitchDrawRect;
    int x, y, width, height;
    int px, py, pwidth;
    bool visible, animated;
    bool mouseActive;
    bool shift;
    bool drawHelperLabel;
    //bool viewParameters;
    
    //vector<MantaTypeFeature> features;
    //MantaTypeFeature selectedFeature;
    bool featureValueSelected, featureVelocitySelected;
    
    
    
    
    bool isConnected;
    ofxConvexHull convexHull;
    
    // parameters
    float velocityLerpRate;
    
    // finger trackers
    vector<ofPoint> fingers, fingersHull, fingersHullNormalized;
    vector<float> fingerValues;
    
    // tracking pads and sliders (for velocity)
    float padVelocity[8][6];
    float sliderVelocity[2];
    float buttonVelocity[4];
    float prevPad[8][6];
    float prevSlider[2];
    float prevButton[4];
    
    // finger statistics
    float numPads;
    float padSum, padAverage;
    float centroidX, centroidY;
    float weightedCentroidX, weightedCentroidY;
    float averageInterFingerDistance;
    float perimeter;
    float padWidth, padHeight, whRatio;
    
    // velocity statistics
    float numPadsVelocity;
    float padSumVelocity, padAverageVelocity;
    float centroidVelocityX, centroidVelocityY;
    float weightedCentroidVelocityX, weightedCentroidVelocityY;
    float averageInterFingerDistanceVelocity;
    float perimeterVelocity;
    float widthVelocity, heightVelocity, whRatioVelocity;
    
    
    
    //    int selll = 1;
    int selection = 0;
    ofPoint dragPoint1, dragPoint2;
    
    
    bool dddd;
    //int ddddddd;
    bool dragging;
    
    
    
    // get selection
    /*
     vector<int> getPadSelection() {return getPadSelection(0);}
     vector<int> getPadVelocitySelection() {return getPadSelection(1);}
     vector<int> getSliderSelection() {return getSliderSelection(0);}
     vector<int> getSliderVelocitySelection() {return getSliderSelection(1);}
     vector<int> getButtonSelection() {return getButtonSelection(0);}
     vector<int> getButtonVelocitySelection() {return getButtonSelection(1);}
     */
    // set selection
    
    void setPadSelection(vector<int> idx, int selection);
    void setSliderSelection(vector<int> idx, int selection);
    void setButtonSelection(vector<int> idx, int selection);
    
    
    
    void markPad(int row, int col, bool mark);
    void markSlider(int index, int column);
    void markButton(int index, bool mark);
    void markAllPads(bool mark);
    void markAllSliders(int column);
    void markAllButtons(bool mark);
    
    
    
    GuiPanel panel;
  
    
    ofEvent<int> eventPadClick;
    ofEvent<int> eventSliderClick;
    ofEvent<int> eventButtonClick;
};