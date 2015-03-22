#include "ContourBodyMap.h"



Contour4::Contour4(vector<ofVec2f> & points, ofPoint center, int label)
{
    this->points = points;
    this->center = center;
    this->label = label;
    age = 0;
    color = ofColor(ofRandom(60,255), ofRandom(60,255), ofRandom(60,255));
}

void Contour4::setPoints(vector<ofVec2f> & points, ofPoint center)
{
    this->points = points;
    this->center = center;
}

void Contour4::draw()
{
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(2);
    ofSetColor(color);
    ofBeginShape();
    for (int j=0; j<points.size(); j++) {
        ofVertex(points[j].x, points[j].y);
    }
    ofEndShape();
    ofPopStyle();
}




void ContourBodyMap::setup(int width, int height)
{
    this->width = width;
    this->height = height;
    
    
    player.loadMovie("/Users/Gene/Desktop/feel-white_building_high.mp4");
    player.setLoopState(OF_LOOP_NORMAL);
    player.play();

    
    mask.allocate(width,height);
    final.allocate(width,height);
    
    shader.load("/Users/Gene/Code/openFrameworks/templates_old/etc/mask/bin/data/standard.vert",
                "/Users/Gene/Code/openFrameworks/templates_old/etc/mask/bin/data/alphamask.frag");
    
    
    // graphics texture
    gfx.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    gfx.begin();
    ofBackground(0, 255, 0);
    for (int i=0; i<200; i++) {
        ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
        ofRect(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 200, 200);
    }
    gfx.end();
    
    
    // final texture
    final.begin();
    ofClear(0,0,0,0);
    final.end();
    

}

void ContourBodyMap::update()
{
    player.update();
}

void ContourBodyMap::draw()
{
    
    // update mask
    mask.begin();
    ofClear(0, 0);
    
    for (int i=0; i<currentContours.size(); i++)
    {
        vector<ofVec2f> calibratedPoints;
        //kinect.getCalibratedContour(i, calibratedPoints, projector.getWidth(), projector.getHeight(), smoothness);
        
        ofBeginShape();
        ofFill();
        ofSetColor(240, 210, 45);
        for (int j=0; j<currentContours[i].size(); j++) {
            ofCurveVertex(currentContours[i][j].x, currentContours[i][j].y);
        }
        ofEndShape();
    }
    mask.end();
    
    
    // make final texture
    final.begin();
    ofClear(0, 0, 0, 0);
    
    shader.begin();
    shader.setUniformTexture("maskTex", mask.getTextureReference(), 1);
    
    //gfx.draw(0,0);
    player.draw(0, 0, final.getWidth(), final.getHeight());
    
    shader.end();
    final.end();
    
    
    
    //bgPlayer.draw(0, 0, projector.getWidth(), projector.getHeight());
    final.draw(0, 0);

}

void ContourBodyMap::recordContours(OpenNI & openNi)
{
    ContourFinder & contourFinder = openNi.getContourFinder();
    RectTracker & tracker = contourFinder.getTracker();
    
    currentContours.clear();
    labels.clear();
    
    calibrated = true;
    
    for(int i = 0; i < openNi.getNumContours(); i++)
    {
        vector<cv::Point> points = contourFinder.getContour(i);
        int label = contourFinder.getLabel(i);
        ofPoint center = toOf(contourFinder.getCenter(i));
        int age = tracker.getAge(label);
        vector<cv::Point> fitPoints = contourFinder.getFitQuad(i);
        cv::RotatedRect fitQuad = contourFinder.getFitEllipse(i);
        
        bool contourExists = false;
        for (int c=0; c<contours.size(); c++)
        {
            if (label == contours[c]->label)
            {
                if (calibrated)
                {
                    vector<ofVec2f> calibratedContour;
                    openNi.getCalibratedContour(i, calibratedContour, width, height, 2.0);
                    currentContours.push_back(calibratedContour);
                    contours[c]->setPoints(calibratedContour, center);
                }
                else {
                    contours[c]->setPoints((vector<ofVec2f> &) contourFinder.getContour(i), center);
                }
                contourExists = true;
                break;
            }
        }
        if (!contourExists)
        {
            if (calibrated)
            {
                vector<ofVec2f> calibratedContour;
                openNi.getCalibratedContour(i, calibratedContour, width, height, 2.0);
                contours.push_back(new Contour4(calibratedContour, center, label));
            }
            else {
                contours.push_back(new Contour4((vector<ofVec2f> &) contourFinder.getContour(i), center, label));
            }
        }
        labels.push_back(label);
    }
    
}

