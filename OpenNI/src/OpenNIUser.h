/*
 Adapted from: [ofxKinectFeatures](https://github.com/asarasua/ofxKinectFeatures)
 Real-time filters for MoCap by Skogstad et al ( http://www.uio.no/english/research/groups/fourms/projects/sma/subprojects/mocapfilters/ )
*/

#pragma once

#include "ofMain.h"
#include "MocapElement.h"
#include "ofxOpenNI.h"


class OpenNIUser
{
public:
    
    static float lpf_soft_a[], lpf_soft_b[], lpf_med_a[], lpf_med_b[], lpf_hard_a[], lpf_hard_b[], lpd1_soft_a[], lpd1_soft_b[], lpd1_med_a[], lpd1_med_b[], lpd1_hard_a[], lpd1_hard_b[], lpd2_soft_a[], lpd2_soft_b[], lpd2_med_a[], lpd2_med_b[], lpd2_hard_a[], lpd2_hard_b[];
    enum FilterLevel {SOFT, MEDIUM, HARD };

    OpenNIUser(ofxOpenNIUser * user);
    ~OpenNIUser();
    
    void setFeatureTrackingEnabled(bool trackingEnabled);
    bool getFeatureTrackingEnabled() {return trackingEnabled;}
    
    ofxOpenNIUser * getUser() {return user;}
    int getSkeletonId() {return user->getXnID();}

    void setFilterLevel(FilterLevel filterLevel);
    FilterLevel getFilterLevel() {return filterLevel;}
    
    void setDepth(int depth) {depth_ = depth;}
    int getDepth() {return depth_;}

    //bool isNewDataAvailable();
    void update();
    
    // Getters
    
    ofPoint getPosition(int j);
    vector<ofPoint> getPositionHistory(int j);
    vector<ofPoint> getPositionHistory(int j, int frames);
    
    ofPoint getPositionFiltered(int j);
    vector<ofPoint> getPositionFilteredHistory(int j);
    vector<ofPoint> getPositionFilteredHistory(int j, int frames);
    
    ofPoint getVelocity(int j);
    vector<ofPoint> getVelocityHistory(int j);
    vector<ofPoint> getVelocityHistory(int j, int frames);
    float getVelocityMagnitude(int j);
    float getVelocityMean(int j, int frames = 30);
    
    ofPoint getAcceleration(int j);
    vector<ofPoint> getAccelerationHistory(int j);
    vector<ofPoint> getAccelerationHistory(int j, int frames);
    float getAccelerationMagnitude(int j);
    float getAccelerationMean(int j, int frames = 30);
    
    float getAccelerationTrajectory(int j);
    vector<float> getAccelerationTrajectoryHistory(int j);
    vector<float> getAccelerationTrajectoryHistory(int j, int frames);
    float getAccelerationTrajectoryMean(int j, int frames = 30);
    //float & getAccelerationTrajectoryMeanRef(int j, int frames = 30);
    
    float getDistanceToTorso(int j);
    vector<float> getDistanceToTorsoHistory(int j);
    vector<float> getDistanceToTorsoHistory(int j, int frames);
    
    ofPoint getRelativePositionToTorso(int j);
    vector<ofPoint> getRelativePositionToTorsoHistory(int j);
    vector<ofPoint> getRelativePositionToTorsoHistory(int j, int frames);
    
    float getQom();
    float getCI();
    float getSymmetry();
    float getYMaxHands();
    
    // References
    
    ofPoint & getPositionRef(int j);
    ofPoint & getPositionFilteredRef(int j);
    ofPoint & getVelocityRef(int j);
    float & getVelocityMagnitudeRef(int j);
    float & getVelocityMeanRef(int j);
    ofPoint & getAccelerationRef(int j);
    float & getAccelerationMagnitudeRef(int j);
    float & getAccelerationMeanRef(int j);
    float & getAccelerationTrajectoryRef(int j);
    float & getDistanceToTorsoRef(int j);
    ofPoint & getRelativePositionToTorsoRef(int j);

    float & getQomRef();
    float & getCIRef();
    float & getSymmetryRef();
    float & getYMaxHandsRef();

private:

    void updateFeatures();
    MocapElement* getElement(int _id);
    void computeJointDescriptors(int jointId, ofPoint jointPos, const float &headTorsoDist);
    ofPoint applyFilter (vector<ofPoint> x, vector<ofPoint> y, float *a, float *b);
    
    template <typename T> vector<T> createVector (T element);
    
    // user
    ofxOpenNIUser *user;
    bool trackingEnabled;

    // filter
    FilterLevel filterLevel;
    float *aFilter, *bFilter, *aLpd1, *bLpd1, *aLpd2, *bLpd2;
    
    //descriptors
    float qom_, ci_, symmetry_, yMaxHands_;
    vector<float> meanVels_;
    vector<MocapElement> elements_;
    int depth_;
    
    //Functor to look for mocap elements matching a Joint
    struct MatchId
    {
        MatchId(const int& j) : j_(j) {}
        bool operator()(MocapElement& obj) const
        {
            return obj.getElementId() == j_;
        }
        
    private:
        const int& j_;
    };
};

