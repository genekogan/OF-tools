#include "CalibrationModule.h"


CalibrationModule::CalibrationModule()
{
    chessboard.size = 300;
    chessboard.cols = 5;
    chessboard.rows = 4;
    chessboard.x = 0;
    chessboard.y = 0;
}

void CalibrationModule::setup(int width, int height)
{
    fboChessboard.allocate(width, height, GL_RGBA);
}

void CalibrationModule::stop()
{
    fboChessboard.allocate(0, 0);
}

void CalibrationModule::drawChessboard()
{
    float w = chessboard.size / chessboard.cols;
    float h = chessboard.size / chessboard.rows;
    currentProjectorPoints.clear();
    
    fboChessboard.begin();
    ofBackground(255);
    ofPushStyle();
    
    ofSetColor(0);
    ofFill();
    ofTranslate(chessboard.x, chessboard.y);
    for (int j = 0; j < chessboard.rows; j++)
    {
        for (int i = 0; i < chessboard.cols; i++)
        {
            int x0 = ofMap(i, 0, chessboard.cols, 0, chessboard.size);
            int y0 = ofMap(j, 0, chessboard.rows, 0, chessboard.size);
            if (j > 0 && i > 0)
            {
                ofVec2f pp(ofMap(chessboard.x + x0, 0, fboChessboard.getWidth(), 0, 1),
                           ofMap(chessboard.y + y0, 0, fboChessboard.getHeight(), 0, 1));
                currentProjectorPoints.push_back(pp);
            }
            if ((i+j) % 2 == 0) {
                ofRect(x0, y0, w, h);
            }
        }
    }
    ofPopStyle();
    fboChessboard.end();
}

void CalibrationModule::drawTestingPoint(ofVec2f projectedPoint)
{
    fboChessboard.begin();
    ofPushStyle();
    ofBackground(255);
    ofCircle(fboChessboard.getWidth() * projectedPoint.x,
             fboChessboard.getHeight() * projectedPoint.y,
             ofMap(sin(ofGetFrameNum() * 0.1), -1, 1, 3, 40));
    ofPopStyle();
    fboChessboard.end();
}

void CalibrationModule::addPointPairs(vector<ofVec3f> & worldPoints)
{
    if (worldPoints.size() == (chessboard.cols-1)*(chessboard.rows-1))
    {
        for (int i=0; i<worldPoints.size(); i++)
        {
            pairsKinect.push_back(worldPoints[i]);
            pairsProjector.push_back(currentProjectorPoints[i]);
        }
        resultMessage = "Added " + ofToString(worldPoints.size()) + " points pairs.";
        resultMessageColor.set(0, 255, 0);
    }
    else
    {
        resultMessage = "Points not added because not all chessboard\npoints' depth known. Try re-positionining.";
        resultMessageColor.set(255, 0, 0);
    }
}

void CalibrationModule::searchForCorners(ofxCvColorImage & rgbImage)
{
    drawChessboard();
    cvRgbImage = ofxCv::toCv(rgbImage.getPixelsRef());
    cv::Size patternSize = cv::Size(chessboard.cols-1, chessboard.rows-1);
    int chessFlags = cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FAST_CHECK;
    foundChessboard = findChessboardCorners(cvRgbImage, patternSize, cvPoints, chessFlags);

    /*
    if (foundChessboard)
    {
        cv::Mat gray;
        cvtColor(cvRgbImage, gray, CV_RGB2GRAY);
        cornerSubPix(gray, cvPoints, cv::Size(11, 11), cv::Size(-1, -1),
                     cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
        drawChessboardCorners(cvRgbImage, patternSize, cv::Mat(cvPoints), foundChessboard);
    }*/
}

void CalibrationModule::draw(ofxSecondWindow *projector)
{
    ofPushStyle();
    
    
    
    cv::Size patternSize = cv::Size(chessboard.cols-1, chessboard.rows-1);
    if (foundChessboard)
    {
        cv::Mat gray;
        cvtColor(cvRgbImage, gray, CV_RGB2GRAY);
        cornerSubPix(gray, cvPoints, cv::Size(11, 11), cv::Size(-1, -1),
                     cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
        drawChessboardCorners(cvRgbImage, patternSize, cv::Mat(cvPoints), foundChessboard);
    }

    
    
    
    ofSetColor(0);
    if (testing)
    {
        if (saved) {
            ofDrawBitmapString("Calibration saved.", 10, 500);
        }
        ofSetLineWidth(3);
        ofSetColor(255, 0, 0);
        ofCircle(testPoint.x, testPoint.y, ofMap(cos(ofGetFrameNum() * 0.1), -1, 1, 3, 40));
    }
    else
    {
        ofSetColor(resultMessageColor);
        ofDrawBitmapString(resultMessage, 10, 500);
        ofSetColor(0);
        ofDrawBitmapString(ofToString(pairsKinect.size())+" point pairs collected.", 10, 520);
    }
    ofPopStyle();
    
    projector->begin();
    ofBackground(255);
    fboChessboard.draw(0, 0);
    projector->end();
}

void CalibrationModule::calibrate(ofxKinectProjectorToolkit & kpt)
{
    kpt.calibrate(pairsKinect, pairsProjector);
}

void CalibrationModule::loadCalibration(ofxKinectProjectorToolkit & kpt)
{
    string filename = "/Users/Gene/Desktop/calibration.xml";
    kpt.loadCalibration(filename);
}

void CalibrationModule::saveCalibration(ofxKinectProjectorToolkit & kpt)
{
    string filename = "/Users/Gene/Desktop/calibration.xml";
    kpt.saveCalibration(filename);
}
