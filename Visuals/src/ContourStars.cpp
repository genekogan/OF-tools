#include "ContourStars.h"




//----------
static bool shouldRemove(ofPtr<ofxBox2dBaseShape>shape) {
    //return !ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape.get()->getPosition());
    return !ofRectangle(0, -400, 1280, 800+400).inside(shape.get()->getPosition());
}



Contour3::Contour3(vector<ofVec2f> & points, ofPoint center, int label)
{
    this->points = points;
    this->center = center;
    this->label = label;
    age = 0;
    color = ofColor(ofRandom(60,255), ofRandom(60,255), ofRandom(60,255));
}

void Contour3::setPoints(vector<ofVec2f> & points, ofPoint center)
{
    this->points = points;
    this->center = center;
}

void Contour3::draw()
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




void ContourStars::setup(int width, int height)
{
    this->width = width;
    this->height = height;
    
    
    panel.setName("stars");
    panel.setPosition(0, 360);
    panel.addSlider("rate", &rate, 1, 20);
    panel.addSlider("tolerance", &tolerance, 0.0f, 100.0f);
    panel.addSlider("circleDensity", &circleDensity, 0.0f, 1.0f);
    panel.addSlider("circleBounce", &circleBounce, 0.0f, 1.0f);
    panel.addSlider("circleFriction", &circleFriction, 0.0f, 1.0f);
//    panel.addEvent("clear", this, &ContourStars::clearCircles);

    
    
    img.loadImage("/Users/Gene/Code/openFrameworks/templates/KinectProjectorToolkit/kpt-box2d/bin/data/star.png");
    //img.loadImage("/Users/Gene/Desktop/cat_png_by_dbszabo1-d3dn2c8.png");
    //img.loadImage("/Users/Gene/Desktop/shazeb.png");
    
    img.resize(40, 40);
    
	box2d.init();
	box2d.setGravity(0, 5);
	//box2d.createGround(ofPoint(0, secondWindow.getHeight()), ofPoint(secondWindow.getWidth(), secondWindow.getHeight()));
    
    rate = 5;
    tolerance = 0.3f;
    circleDensity = 0.3;
    circleBounce = 0.5;
    circleFriction = 0.1;
}

void ContourStars::update()
{
    lines.clear();
    edges.clear();
    
    // create box2d edges
    for(int i = 0; i < currentContours.size(); i++) {
        lines.push_back(ofPolyline());
        for (int j=0; j<currentContours[i].size(); j++) {
            lines.back().addVertex(currentContours[i][j].x, currentContours[i][j].y);
        }
        ofPtr <ofxBox2dEdge> edge = ofPtr<ofxBox2dEdge>(new ofxBox2dEdge);
        lines.back().simplify(tolerance);
        for (int i=0; i<lines.back().size(); i++) {
            edge.get()->addVertex(lines.back()[i]);
        }
        edge.get()->create(box2d.getWorld());
        edges.push_back(edge);
    }
    
    // add some falling circles every so often
	if((int)ofRandom(0, rate) == 0) {
        ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);
        circle.get()->setPhysics(circleDensity, circleBounce, circleFriction);
        circle.get()->setup(box2d.getWorld(), ofRandom(width), -20, ofRandom(15, 40));
		circles.push_back(circle);
	}
	
    // update box2d
    ofRemove(circles, shouldRemove);
    box2d.update();

}

void ContourStars::draw()
{
    
    
    for (int i=0; i<circles.size(); i++) {
        ofPushMatrix();
        ofPushStyle();
        
		ofFill();
		ofSetHexColor(0xc0dd3b);
		
        ofVec2f pos = circles[i].get()->getPosition();
        float rad = circles[i].get()->getRadius();
        float ang = circles[i].get()->getRotation();
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofTranslate(pos.x, pos.y);
        ofRotate(ang);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        img.draw(0, 0, 2*rad, 2*rad);
        ofDisableBlendMode();
        
        ofPopStyle();
        ofPopMatrix();
	}

}

void ContourStars::recordContours(OpenNI & openNi)
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
                contours.push_back(new Contour3(calibratedContour, center, label));
            }
            else {
                contours.push_back(new Contour3((vector<ofVec2f> &) contourFinder.getContour(i), center, label));
            }
        }
        labels.push_back(label);
    }

}
