#include "ContourVisual.h"


Contour::Contour(vector<ofVec2f> & points, ofPoint center, int label)
{
    this->points = points;
    this->center = center;
    this->label = label;
    age = 0;
    color = ofColor(ofRandom(60,255), ofRandom(60,255), ofRandom(60,255));
}

void Contour::setPoints(vector<ofVec2f> & points, ofPoint center)
{
    this->points = points;
    for (auto p : points) {
        p.y = 800 - p.y;
    }
    this->center = center;
}

void Contour::draw()
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


void ContourVisual::setup(int width, int height)
{
    this->width = width;
    this->height = height;

    
    //
    //
    //
    //
    //
    panel.setPosition(5, 300);
}

void ContourVisual::update(OpenNI & openNi)
{
    recordContours(openNi);
}

void ContourVisual::draw()
{

}

void ContourVisual::recordContours(OpenNI & openNi)
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
                    /*
                    for (auto p2 : calibratedContour) {
                        cout << p2.y << endl;
                        p2.y = height - p2.y;
                        cout << "now " << p2.y << endl;
                        cout << "----"<<endl;
                    }*/
                    
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
                openNi.getCalibratedContour(i, calibratedContour, width, height, contourSmoothness);
                
                /*
                for (auto p2 : calibratedContour) {
                    cout << p2.y << endl;
                    p2.y = height - p2.y;
                    cout << "now " << p2.y << endl;
                    cout << "----"<<endl;
                }
                 */
                
                contours.push_back(new Contour(calibratedContour, center, label));
            }
            else {
                contours.push_back(new Contour((vector<ofVec2f> &) contourFinder.getContour(i), center, label));
            }
        }
        labels.push_back(label);
    }
}

void ContourVisual::eraseContours()
{
    vector<Contour*>::iterator it = contours.begin();
    while (it != contours.end()) {
        delete *it;
        contours.erase(it);
    }
}
