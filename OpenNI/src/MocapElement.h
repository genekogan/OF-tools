/*
 Adapted from: [ofxKinectFeatures](https://github.com/asarasua/ofxKinectFeatures)
 */

#pragma once

#include "ofMain.h"

class MocapElement
{
public:
    MocapElement();
    MocapElement(int elementId, int depth);
    
    void setElementId(int newId) {elementId_ = newId;}
    unsigned int getElementId() {return elementId_;}
    
    void setHistoryDepth(int depth);
    int getHistoryDepth() {return historyDepth_;}
    
    void setPosition(ofPoint position);
    void setPositionFiltered(ofPoint positionFiltered);
    void setVelocity(ofPoint velocity);
    void setVelocityMagnitude(float velocityMagnitude);
    void setVelocityMean(float velocityMean);
    void setAcceleration(ofPoint acceleration);
    void setAccelerationMagnitude(float accelerationMagnitude);
    void setAccelerationMean(float accelerationMean);
    void setAccelerationTrajectory(float accelerationTrajectory);
    void setDistanceToTorso(float distanceToTorso);
    void setRelativePositionToTorso(ofPoint relativePositionToTorso);

    vector<ofPoint> getPosition() {return position_;}
    vector<ofPoint> getPositionFiltered() {return positionFiltered_;}
    vector<ofPoint> getVelocity() {return velocity_;}
    vector<float>   getVelocityMagnitude() {return velocityMagnitude_;}
    vector<float>   getVelocityMean() {return velocityMean_;}
    vector<ofPoint> getAcceleration() {return acceleration_;}
    vector<float>   getAccelerationMagnitude() {return accelerationMagnitude_;}
    vector<float>   getAccelerationMean() {return accelerationMean_;}
    vector<float>   getAccelerationTrajectory() {return accelerationTrajectory_;}
    vector<float>   getDistanceToTorso() {return distanceToTorso_;}
    vector<ofPoint> getRelativePositionToTorso() {return relativePositionToTorso_;}
    
    vector<ofPoint> & getPositionRef() {return position_;}
    vector<ofPoint> & getPositionFilteredRef() {return positionFiltered_;}
    vector<ofPoint> & getVelocityRef() {return velocity_;}
    vector<float>   & getVelocityMagnitudeRef() {return velocityMagnitude_;}
    vector<float>   & getVelocityMeanRef() {return velocityMean_;}
    vector<ofPoint> & getAccelerationRef() {return acceleration_;}
    vector<float>   & getAccelerationMagnitudeRef() {return accelerationMagnitude_;}
    vector<float>   & getAccelerationMeanRef() {return accelerationMean_;}
    vector<float>   & getAccelerationTrajectoryRef() {return accelerationTrajectory_;}
    vector<float>   & getDistanceToTorsoRef() {return distanceToTorso_;}
    vector<ofPoint> & getRelativePositionToTorsoRef() {return relativePositionToTorso_;}
    
private:
    
    void initialize();
    
    int historyDepth_;
    float historyWeight_;
    int elementId_;
    
    float velocityMeanRunning_;
    float accelerationMeanRunning_;

    vector<ofPoint> position_;
    vector<ofPoint> positionFiltered_;
    vector<ofPoint> velocity_;
    vector<float> velocityMagnitude_;
    vector<float> velocityMean_;
    vector<ofPoint> acceleration_;
    vector<float> accelerationMagnitude_;
    vector<float> accelerationMean_;
    vector<float> accelerationTrajectory_;
    vector<float> distanceToTorso_;
    vector<ofPoint> relativePositionToTorso_;
};
