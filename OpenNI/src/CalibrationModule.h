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
        ofPoint position;
        int cols, rows;
        int size;
    };

    CalibrationModule();

    void setup(int width, int height);
    void stop();
    
    Chessboard & getChessboard() {return chessboard;}
    vector<cv::Point2f> & getChessboardCorners() {return cvPoints;}
    bool & getTesting() {return testing;}
    
    void addPointPairs(vector<ofVec3f> & worldPoints);
    void testWorldPoint(ofVec3f & worldPoint);
    void searchForCorners(ofxCvColorImage & rgbImage);
    void drawTestingPoint(ofVec2f projectedPoint);
    
    vector<double> getCalibration();
    void calibrate(ofxKinectProjectorToolkit &kpt);
    void loadCalibration(ofxKinectProjectorToolkit &kpt);
    void saveCalibration(ofxKinectProjectorToolkit &kpt);

    void draw(ofxSecondWindow *projector);
    
private:
    
    void drawChessboard();
    
    ofFbo fboChessboard;
    cv::Mat cvRgbImage;
    Chessboard chessboard;
    bool foundChessboard;
    
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


