#pragma once

#include "ofMain.h"
#include "ofxSecondWindow.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"

#include "Control.h"


using namespace ofxCv;
using namespace cv;


class CalibrationModule
{
public:

    struct Chessboard {
        int x, y;
        int cols, rows;
        int size;
    };

    CalibrationModule();

    void setup(int width, int height);
    void stop();
    
    vector<double> getCalibration();
    
    Chessboard & getChessboard() {return chessboard;}
    
    vector<cv::Point2f> & getChessboardCorners() {return cvPoints;}
    
    
    void addPointPairs(vector<ofVec3f> & worldPoints);
    void testWorldPoint(ofVec3f & worldPoint);
    void searchForCorners(ofxCvColorImage & rgbImage);
    void drawTestingPoint(ofVec2f projectedPoint);
    
    void draw(ofxSecondWindow * secondWindow);
    
    
    void calibrate(ofxKinectProjectorToolkit & kpt) {
        kpt.calibrate(pairsKinect, pairsProjector);
    }
    void loadCalibration(ofxKinectProjectorToolkit & kpt) {
        string filename = "/Users/Gene/Desktop/calibration.xml";
        kpt.loadCalibration(filename);
    }
    void saveCalibration(ofxKinectProjectorToolkit & kpt) {
        //kpt.calibrate(pairsKinect, pairsProjector);
        string filename = "/Users/Gene/Desktop/calibration.xml";
        kpt.saveCalibration(filename);
    }
    
private:
    
    void drawChessboard();
    
    ofFbo fboChessboard;
    cv::Mat cvRgbImage;
    Chessboard chessboard;
    
    vector<cv::Point2f> cvPoints;
    vector<ofVec2f> currentProjectorPoints;
    vector<ofVec3f> pairsKinect;
    vector<ofVec2f> pairsProjector;
    
    string resultMessage;
    ofColor resultMessageColor;
    ofVec2f testPoint;
    
    bool testing;
    bool saved;
};


