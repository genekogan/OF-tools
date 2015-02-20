#pragma once


#include "ofxOpenNI.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"

#include "OpenNIUser.h"

#include "Control.h"

using namespace ofxCv;
using namespace cv;


enum {
    VELOCITY_MEAN,
    ACCELERATION_Y,
    RELPOSTOTORSO_X
};



class OpenNI
{
public:

    
    OpenNI();
    ~OpenNI();
    
    void setup(string oni="");
    void stop();
    
    void setTrackingUsers(bool trackingUsers);
    void setTrackingUserFeatures(bool trackingUserFeatures);
    void setTrackingContours(bool trackingContours);
    
    
    ofVec3f getWorldCoordinateAt(int x, int y)
    {
        ofPoint depthPoint = ofPoint(x, y, depthPixels[x + y * 640]);
        ofVec3f worldPoint = kinect.projectiveToWorld(depthPoint);
        return worldPoint;
    }
    
    
    void eventSetTrackingUsers(GuiElementEventArgs & b)
    {
        setTrackingUsers(trackingUsers);
    }
    
    void eventSetTrackingContours(GuiElementEventArgs & b)
    {
        setTrackingContours(trackingContours);
    }

    void eventAdjustKinectTilt(GuiElementEventArgs & b)
    {
        
    }

    void eventSetTrackingUserFeatures(GuiElementEventArgs & b)
    {
        if (b.name == "track features"){
            setTrackingUserFeatures(trackingUserFeatures);
        }
    }

    void eventUser(ofxOpenNIUserEvent & event)
    {
        ofLog(OF_LOG_NOTICE, "User event: " + ofToString(event.userStatus) + " " + ofToString(event.id));
        
        
        
        if (event.userStatus == USER_TRACKING_STARTED)
        {
            ofLog(OF_LOG_NOTICE, "OpenNI: TRACK STARTED FOR  :: "+ofToString(event.id));
        }
        else if (event.userStatus == USER_TRACKING_STOPPED)
        {            
            ofLog(OF_LOG_NOTICE, "OpenNI: TRACK STOPPED FOR  :: "+ofToString(event.id));
            if (users.count(event.id) > 0)
            {
                delete users[event.id];
                users.erase(event.id);
            }
        }
        
        else if (event.userStatus == USER_CALIBRATION_STOPPED) {
            ofLog(OF_LOG_NOTICE, "OpenNI: CALIB STOPPED FOR  :: "+ofToString(event.id));
        }
        else if (event.userStatus == USER_CALIBRATION_STARTED) {
            ofLog(OF_LOG_NOTICE, "OpenNI: CALIB STARTED FOR  :: "+ofToString(event.id));
            
        }
        else if (event.userStatus == USER_SKELETON_LOST) {
            ofLog(OF_LOG_NOTICE, "OpenNI: SKEL LOST FOR  :: "+ofToString(event.id));
            
        }
        else if (event.userStatus == USER_SKELETON_FOUND) {
            
            ofLog(OF_LOG_NOTICE, "OpenNI: SKEL FOUNd FOR  :: "+ofToString(event.id));
        }
        
    }
    

    ofxOpenNI & getKinect() {return kinect;}
    
    ContourFinder & getContourFinder() {return contourFinder;}
    
    
    void updateSkeletonFeatures();
    
    void resetUserGenerator();
    
    inline bool isNewSkeletonDataAvailable(ofxOpenNIUser & user)
    {
        return (user.getJoint(JOINT_TORSO).getWorldPosition() != ofPoint(0,0,0) && (users.count(user.getXnID()) || user.getJoint(JOINT_TORSO).getWorldPosition() != users[user.getXnID()]->getPosition(0) ));
    }
    
    int getNumTrackedUsers() {return numTrackedUsers;}
    
//    ofxKinectFeatures & getFeatureExtractor() { return featExtractor;}
    
    
    
    bool update();
    void updateUsers();
    void updateContours();
    
    void draw();
    void drawFeatureExtractor();
    

    ofxOpenNI kinect;
    ContourFinder contourFinder;

    
    map<int, OpenNIUser*> users;
    
    
    map<int, OpenNIUser*> & getUsers() {return users;}
    

    
    float kinectTiltAngle;
    int numTrackedUsers;
    
    bool trackingUsers;
    bool trackingUserFeatures;
    bool trackingContours;

    bool hadUsers;
    

    GuiPanel panel;
    
    


    
    
    ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage grayThreshNear;
	ofxCvGrayscaleImage grayThreshFar;
    ofShortPixels depthPixels;

    
    
    int nearThreshold;
    int farThreshold;
    int minArea;
    int maxArea;
    int threshold;
    int persistence;
    int maxDistance;
    int smoothingRate;

    vector<ParameterBase*> contourTrackingParameters;
    vector<ParameterBase*> userTrackingParameters;
};