#include "ProjectorCalibration.h"


ProjectorCalibration::ProjectorCalibration()
{
    chessboardSize = 300;
    chessboardNumCols = 5;
    chessboardNumRows = 4;
    
    panel.setAutoDraw(false);
    panel.setAutoUpdate(false);
}

void ProjectorCalibration::setup(OpenNI & openNi_, ofxSecondWindow & secondWindow_)
{
    this->openNi = &openNi_;
    this->secondWindow = &secondWindow_;
    this->width = secondWindow->getWidth();
    this->height = secondWindow->getHeight();
    kinectWidth = openNi->getKinect().getWidth();
    kinectHeight = openNi->getKinect().getHeight();
    
    panel.setName("Calibration");
    panel.setPosition(ofGetWidth()-300, 10);

    panel.addParameter("x", &chessboardPositionX, 0, width);
    panel.addParameter("y", &chessboardPositionY, 0, height);
    panel.addParameter("size", &chessboardSize, 10, 800);
    panel.addEvent("add point", this, &ProjectorCalibration::eventAddPointPair);
    panel.addEvent("calibrate", this, &ProjectorCalibration::eventCalibrate);
    panel.addEvent("save", this, &ProjectorCalibration::eventSaveCalibration);
    panel.addEvent("load", this, &ProjectorCalibration::eventLoadCalibration);
    panel.addParameter("testing", &testing);

}

void ProjectorCalibration::start()
{
    rgbImage.allocate(kinectWidth, kinectHeight);
    fboChessboard.allocate(width, height, GL_RGBA);
    
    panel.setAutoDraw(true);
    panel.setAutoUpdate(true);
}

void ProjectorCalibration::stop()
{
    rgbImage.clear();
    fboChessboard.allocate(0, 0);
    
    panel.setAutoDraw(false);
    panel.setAutoUpdate(false);
}

vector<double> ProjectorCalibration::getCalibration()
{
    vector<double> coefficients;
    return coefficients;
}



void ProjectorCalibration::getCalibratedContour(vector<cv::Point> & points, vector<ofVec2f> & calibratedPoints, int width, int height)
{
    int smoothness = 2;
    for (int j=0; j<points.size(); j+=smoothness) {
        
        
        ofVec3f worldPoint = openNi->getWorldCoordinateAt(points[j].x, points[j].y);
        ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);

        
        // discrepancy between ofxKinect (used for calibration) and ofxOpenNi
        //        worldPoint.x *= -1.0;
        //        worldPoint.y *= -1.0;
        
        ofPoint mappedPoint(ofMap(projectedPoint.x, 0, 1, 0, width),
                            ofMap(projectedPoint.y, 0, 1, 0, height));
        
        calibratedPoints.push_back(mappedPoint);
    }
}



void ProjectorCalibration::drawChessboard(int x, int y, int chessboardSize)
{
    float w = chessboardSize / chessboardNumCols;
    float h = chessboardSize / chessboardNumRows;
    currentProjectorPoints.clear();

    fboChessboard.begin();
    ofBackground(255);
    ofPushStyle();
    ofSetColor(0);
    ofFill();
    ofTranslate(x, y);
    for (int j = 0; j < chessboardNumRows; j++)
    {
        for (int i = 0; i < chessboardNumCols; i++)
        {
            int x0 = ofMap(i, 0, chessboardNumCols, 0, chessboardSize);
            int y0 = ofMap(j, 0, chessboardNumRows, 0, chessboardSize);
            if (j > 0 && i > 0)
            {
                ofVec2f pp(ofMap(x+x0, 0, fboChessboard.getWidth(), 0, 1),
                           ofMap(y+y0, 0, fboChessboard.getHeight(), 0, 1));
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

void ProjectorCalibration::drawTestingPoint(ofVec2f projectedPoint)
{
    
    float ptSize = ofMap(sin(ofGetFrameNum()*0.1), -1, 1, 3, 40);
    fboChessboard.begin();
    ofBackground(255);
    ofSetColor(0, 255, 0);
    ofCircle(
             ofMap(projectedPoint.x, 0, 1, 0, fboChessboard.getWidth()),
             ofMap(projectedPoint.y, 0, 1, 0, fboChessboard.getHeight()),
             ptSize);
    ofSetColor(255);
    fboChessboard.end();

}

void ProjectorCalibration::eventAddPointPair(GuiElementEventArgs & b)
{
    if (b.value==1.0)   return;
    
    
    int nDepthPoints = 0;
    for (int i=0; i<cvPoints.size(); i++)
    {
        ofVec3f worldPoint = openNi->getWorldCoordinateAt(cvPoints[i].x, cvPoints[i].y);
        if (worldPoint.z > 0)   nDepthPoints++;
    }
    if (nDepthPoints == (chessboardNumCols-1)*(chessboardNumRows-1))
    {
        for (int i=0; i<cvPoints.size(); i++)
        {
            ofVec3f worldPoint = openNi->getWorldCoordinateAt(cvPoints[i].x, cvPoints[i].y);
            pairsKinect.push_back(worldPoint);
            pairsProjector.push_back(currentProjectorPoints[i]);
        }
        resultMessage = "Added " + ofToString((chessboardNumCols-1)*(chessboardNumRows-1)) + " points pairs.";
        resultMessageColor = ofColor(0, 255, 0);
    }
    else
    {
        resultMessage = "Points not added because not all chessboard\npoints' depth known. Try re-positionining.";
        resultMessageColor = ofColor(255, 0, 0);
    }
}

void ProjectorCalibration::eventCalibrate(GuiElementEventArgs & b)
{
    if (b.value == 1.0) return;

    kpt.calibrate(pairsKinect, pairsProjector);
    testing = true;
}

void ProjectorCalibration::eventSaveCalibration(GuiElementEventArgs & b)
{
    kpt.saveCalibration("calibration.xml");
    saved = true;
}

void ProjectorCalibration::eventLoadCalibration(GuiElementEventArgs & b)
{
    kpt.loadCalibration("calibration.xml");
    testing = true;
}

void ProjectorCalibration::update()
{

    rgbImage.setFromPixels(openNi->getKinect().getImagePixels());
    if (testing)
    {
        testPoint.set(ofClamp(ofGetMouseX(), 0, 639), ofClamp(ofGetMouseY(), 0, 479));
        ofVec2f t = ofVec2f(min(kinectWidth-1.0f,testPoint.x), min(kinectHeight-1.0f,testPoint.y));
        ofVec3f worldPoint = openNi->getWorldCoordinateAt(t.x, t.y);
        ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
        drawTestingPoint(projectedPoint);
    }
    else
    {
        drawChessboard(chessboardPositionX, chessboardPositionY, chessboardSize);
        
        cvRgbImage = ofxCv::toCv(rgbImage.getPixelsRef());
        cv::Size patternSize = cv::Size(chessboardNumCols-1, chessboardNumRows-1);
        int chessFlags = cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FAST_CHECK;
        bool foundChessboard = findChessboardCorners(cvRgbImage, patternSize, cvPoints, chessFlags);
        if (foundChessboard)
        {
            cv::Mat gray;
            cvtColor(cvRgbImage, gray, CV_RGB2GRAY);
            cornerSubPix(gray, cvPoints, cv::Size(11, 11), cv::Size(-1, -1),
                         cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(cvRgbImage, patternSize, cv::Mat(cvPoints), foundChessboard);
        }
    }
    

}

void ProjectorCalibration::draw()
{
    rgbImage.draw(0, 0);
    
    
    //kinect.drawDepth(10, 490, 320, 240);
    
    ofSetColor(0);
    if (testing) {
        ofDrawBitmapString("Click on the image to test a point in the RGB image.", 340, 510);
        ofDrawBitmapString("The projector should place a green dot on the corresponding point.", 340, 530);
        ofDrawBitmapString("Press the 's' key to save the calibration.", 340, 550);
        if (saved) {
            ofDrawBitmapString("Calibration saved.", 340, 590);
        }
        ofSetColor(255, 0, 0);
        float ptSize = ofMap(cos(ofGetFrameNum()*0.1), -1, 1, 3, 40);
        ofCircle(testPoint.x, testPoint.y, ptSize);
    } else {
        ofDrawBitmapString("Position the chessboard using the mouse.", 340, 510);
        ofDrawBitmapString("Adjust the size of the chessboard using the 'q' and 'w' keys.", 340, 530);
        ofDrawBitmapString("Adjust kinect's tilt angle using up/down keys.", 340, 550);
        ofDrawBitmapString("Press the spacebar to save a set of point pairs.", 340, 570);
        ofDrawBitmapString("Press the 'c' key to calibrate.", 340, 590);
        ofSetColor(resultMessageColor);
        ofDrawBitmapString(resultMessage, 340, 630);
        ofSetColor(0);
        ofDrawBitmapString(ofToString(pairsKinect.size())+" point pairs collected.", 340, 650);
    }
    ofSetColor(255);
    
    
    secondWindow->begin();
    fboChessboard.draw(0, 0);
    secondWindow->end();
}

//--------------------------------------------------------------
void ProjectorCalibration::mousePressed(int x, int y, int button){
    /*
    if (testing) {
        testPoint.set(min(x, kinect.width-1), min(y, kinect.height-1));
    }*/
}
