#pragma once


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxSecondWindow.h"

#include "OpenNI.h"
#include "Control.h"

class ProjectorCalibration
{
public:
    ProjectorCalibration();
    void setup(OpenNI & openNi_, ofxSecondWindow & secondWindow_);
    
    void start();
    void stop();
    
    vector<double> getCalibration();

    void getCalibratedContour(vector<cv::Point> & points, vector<ofVec2f> & calibratedPoints, int width, int height);
    
    
    ofVec2f getProjectedPoint(int x, int y) {
        ofVec3f worldPoint = openNi->getWorldCoordinateAt(x, y);
        ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
        return projectedPoint;
    }
    
    GuiPanel panel;

    
    void update();
    void draw();

private:

    void drawChessboard(int x, int y, int chessboardSize);
    void drawTestingPoint(ofVec2f projectedPoint);

    void eventAddPointPair(GuiElementEventArgs & b);
    void eventCalibrate(GuiElementEventArgs & b);
    void eventSaveCalibration(GuiElementEventArgs & b);
    void eventLoadCalibration(GuiElementEventArgs & b);
    
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    
    
    int width, height;
    int kinectWidth, kinectHeight;
    
    OpenNI * openNi;
    ofxKinectProjectorToolkit   kpt;
    float                       tiltAngle;
    
    ofxSecondWindow             * secondWindow;
    ofFbo                       fboChessboard;
    ofxCvColorImage             rgbImage;
    cv::Mat                     cvRgbImage;
    
    vector<ofVec2f>             currentProjectorPoints;
    vector<cv::Point2f>         cvPoints;
    vector<ofVec3f>             pairsKinect;
    vector<ofVec2f>             pairsProjector;
    
    string                      resultMessage;
    ofColor                     resultMessageColor;
    ofVec2f                     testPoint;
    
    int                         chessboardSize;
    int                         chessboardNumCols;
    int                         chessboardNumRows;
    int                         chessboardPositionX;
    int                         chessboardPositionY;
    bool                        testing;
    bool                        saved;
};


