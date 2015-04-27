#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxSecondWindow.h"
#include "Control.h"
#include "OpenNIUser.h"
#include "OpenNITracker.h"
#include "CalibrationModule.h"

using namespace ofxCv;
using namespace cv;


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
    
    ofxOpenNI & getKinect() {return kinect;}
    ofxKinectProjectorToolkit & getKinectProjectorToolkit() {return kpt;}
    vector<double> getCalibration() {return kpt.getCalibration();}

    ofVec3f getWorldCoordinateAt(int x, int y);
    ofVec2f getProjectedPointAt(int x, int y);
    
    int getNumContours() {return numContours;}
    vector<cv::Point> & getContour(int idx) {return contourFinder.getContour(idx);}
    void getCalibratedContour(int idx, vector<ofVec2f> & calibratedPoints, int width, int height, float smoothness=1.0);
    ContourFinder & getContourFinder() {return contourFinder;}
    
    map<int, OpenNIUser*> & getUsers() {return users;}
    int getNumTrackedUsers() {return numTrackedUsers;}
    
    bool update();
    void draw();
    
    // calibration
    bool isCalibrating() {return calibrating;}
    void enableCalibration(ofxSecondWindow & window);
    void startCalibrationModule();
    void stopCaibrationModule();
    void saveCalibration(string filename);
    void loadCalibration(string filename);
    
    GuiPanel & getPanel() {return panel;}

private:
    
    void updateUsers();
    void updateSkeletonFeatures();
    void updateContours();
    void applyDepthMask();
    void clearUsers();
    void resetUserGenerator();

    void drawDebug();
    
    inline bool isNewSkeletonDataAvailable(ofxOpenNIUser & user);
    
    void eventSetTrackingUsers(GuiButtonEventArgs & e);
    void eventSetTrackingContours(GuiButtonEventArgs & e);
    void eventSetTrackingUserFeatures(GuiButtonEventArgs & e);
    void eventSetMaxUsers(GuiSliderEventArgs<int> & e);
    void eventToggleCalibrationModule(GuiButtonEventArgs & e);
    void eventDepthMaskEdited(Gui2dPadEventArgs &e);
    void eventSetupMask(GuiButtonEventArgs & e);
    void eventUser(ofxOpenNIUserEvent & event);

    // calibration
    void updateCalibration();
    void testCalibration();
    void drawCalibration();
    
    void eventAddPointPairs(GuiButtonEventArgs & b);
    void eventCalibrate(GuiButtonEventArgs & b);
    void eventSaveCalibration(GuiButtonEventArgs & b);
    void eventLoadCalibration(GuiButtonEventArgs & b);

    // kinect
    int kinectWidth, kinectHeight;
    ofxOpenNI kinect;
    map<int, OpenNIUser*> users;
    ContourFinder contourFinder;
    ofxKinectProjectorToolkit kpt;
    
    // mode parameters
    bool trackingUsers;
    bool trackingUserFeatures;
    bool trackingContours;
    
    // tracking status
    int numTrackedUsers;
    int numContours;
    bool hadUsers;
    
    // contour tracking
    ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage grayThreshNear;
	ofxCvGrayscaleImage grayThreshFar;
    int nearThreshold;
    int farThreshold;
    int minArea;
    int maxArea;
    int threshold;
    int persistence;
    int maxDistance;
    int smoothingRate;
    bool simplified;
    int delay;
    
    // depth mask
    ofxCvColorImage cvMask;
    ofxCvGrayscaleImage cvGrayMask;
    ofFbo maskFBO;
    ofImage maskImage;
    ofPixels maskPixels;
    Gui2dPad *maskPad;
    bool depthMaskEnabled;
    
    // user tracking
    vector<ofShortPixels> depthHistory;
    int numFrames;
    int idxHistory;
    int maxUsers;
    
    // gui
    GuiPanel panel;
    GuiWidget *panelCalibration;
    GuiWidget *panelTracking;
    GuiWidget *panelUsers;
    GuiToggle *toggleCalibrate;
    
    // calibration module
    CalibrationModule calibration;
    ofxSecondWindow * window;
    ofxCvColorImage rgbImage;
    bool calibrating;
};
