#include "ContourFluids.h"



Contour2::Contour2(vector<ofVec2f> & points, ofPoint center, int label)
{
    this->points = points;
    this->center = center;
    this->label = label;
    age = 0;
    color = ofColor(ofRandom(60,255), ofRandom(60,255), ofRandom(60,255));
}

void Contour2::setPoints(vector<ofVec2f> & points, ofPoint center)
{
    this->points = points;
    this->center = center;
}

void Contour2::draw()
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


void ContourFluids::setup(int width, int height)
{
    this->width = width;
    this->height = height;


    panel.setName("fluids");
    panel.setPosition(0, 360);
    panel.addSlider("simplify", &simplify, 0.0f, 100.0f);
    panel.addSlider("numContourPts", &numContourPts, 3, 100);
    panel.addSlider("skip", &skip, 1, 100);
    panel.addSlider("displaceLerp", &displaceLerp, 0.0f, 1.0f);
    panel.addSlider("dissipation", &dissipation, 0.0f, 1.0f);
    panel.addSlider("vel dissipation", &velDissipation, 0.0f, 1.0f);
    panel.addSlider("displacement", &displacement, 0.0f, 100.0f);
    panel.addSlider("strength", &strength, 0.0f, 10.0f);
    panel.addSlider("gravityX",  &gravityX, -0.02f, 0.02f);
    panel.addSlider("gravityY", &gravityY, -0.02f, 0.02f);
    
    
    
    simplify = 40.0f;
    numContourPts = 10;
    displaceLerp = 0.1;
    skip = 5;

    
    
    // hold previous joints
    for (int i=0; i<maxUsers; i++) {
        vector<ofVec2f> newVec;
        //newVec.resize(openNi->getJointNames().size());
        pContourPoints.push_back(newVec);
        displace.push_back(newVec);
    }
    
    
    
    // Initial Allocation
    fluid.allocate(width, height, 0.5);
    
    // Seting the gravity set up & injecting the background image
    fluid.dissipation = 0.99;
    fluid.velocityDissipation = 0.99;
    
    fluid.setGravity(ofVec2f(0.0,0.0));
    //    fluid.setGravity(ofVec2f(0.0,0.0098));
    
    //  Set obstacle
    fluid.begin();
    ofSetColor(0,0);
    ofSetColor(255);
    ofCircle(width*0.5, height*0.35, 40);
    fluid.end();
    fluid.setUseObstacles(false);
    
    // Adding constant forces
    /*
     fluid.addConstantForce(ofPoint(gfx.getWidth()*0.5,
     gfx.getHeight()*0.85),
     ofPoint(0,-2),
     ofFloatColor(0.5,0.1,0.0),
     10.f);
     */
    
    for (int i=0; i<12; i++) {
        float x = ofMap(i+0.5, 0, 12, 0, width);
        //fluid.addConstantForce(ofPoint(x, gfx.getHeight()*0.85),
        //                       ofPoint(0,-6),
        //                       ofFloatColor(0.5,0.1,0.0),
        //                       10.f);
    }
    
    
    
    dissipation = 0.99;
    velDissipation = 0.99;
    gravityX = 0.0;
    gravityY = 0.0;
    displacement = 10;
    strength = 4.8;
}

void ContourFluids::update()
{
    
    for (int i=0; i<pContourPoints.size(); i++) {
        while (pContourPoints[i].size() < numContourPts) {
            pContourPoints[i].push_back(ofVec2f(0,0));
        }
        if (pContourPoints[i].size() > numContourPts) {
            pContourPoints[i].resize(numContourPts);
        }
    }
    
    for (int i=0; i<displace.size(); i++) {
        while (displace[i].size() < numContourPts) {
            displace[i].push_back(ofVec2f(0,0));
        }
        if (displace[i].size() > numContourPts) {
            displace[i].resize(numContourPts);
        }
    }
    
    
    
    for (int i=0; i < min(maxUsers,(int) currentContours.size()); i++) {
        ofPolyline pl;
        for (int j=0; j<currentContours[i].size(); j++) {
            pl.addVertex(currentContours[i][j]);
        }
        //pl.addVertices((vector<ofPoint>&) currentContours[i]);
        pl.simplify(simplify);
        
        
        vector<ofPoint> verts = pl.getVertices();
        ofVec2f newDisplace;
        for (int j=0; j<numContourPts; j++) {
            int idx = ofMap(j, 0, numContourPts, 0, verts.size());
            ofVec2f pt = verts[idx];
            
            
            newDisplace = pt - pContourPoints[i][j];
            displace[i][j].set(ofLerp(displace[i][j].x, newDisplace.x, displaceLerp),
                               ofLerp(displace[i][j].y, newDisplace.y, displaceLerp));
            
            pContourPoints[i][j] = pt;
            
            
            
            if (j%skip==0) {
                ofPoint m = pt;
                ofPoint d = displace[i][j]*displacement;
                ofPoint c = ofPoint(320, 240) - m;
                c.normalize();
                fluid.addTemporalForce(m,
                                       d,
                                       ofFloatColor(c.x,c.y,0.5)*sin(ofGetElapsedTimef()),
                                       strength);
            }
            
            
        }
    }
    
    
    //fluid.setGravity(ofVec2f(gravityX, gravityY));
    fluid.dissipation = dissipation;
    fluid.velocityDissipation = velDissipation;
    
    fluid.update();
    
}

void ContourFluids::draw()
{
    
    //    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_LINEAR);
    fluid.draw();
    
    
    //renderContours();
    /*
     ofSetColor(255);
     for (int i=0; i<pContourPoints.size(); i++) {
     for (int j=0; j<pContourPoints[i].size(); j++) {
     ofLine(pContourPoints[i][j].x, pContourPoints[i][j].y, pContourPoints[i][j].x + displace[i][j].x, pContourPoints[i][j].y + displace[i][j].y);
     }
     }
     */
   
}

void ContourFluids::recordContours(OpenNI & openNi)
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
                contours.push_back(new Contour2(calibratedContour, center, label));
            }
            else {
                contours.push_back(new Contour2((vector<ofVec2f> &) contourFinder.getContour(i), center, label));
            }
        }
        labels.push_back(label);
    }

    
}
