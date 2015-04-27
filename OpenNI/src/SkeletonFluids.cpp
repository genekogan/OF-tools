#include "SkeletonFluids.h"


void SkeletonFluids::setup(int width, int height)
{
    SkeletonVisual::setup(width, height);
    
    panel.setName("fluids");
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
    maxUsers = 3;
    
    
    /*
    
    // hold previous joints
    for (int i=0; i<maxUsers; i++) {
        vector<ofVec2f> newVec;
        //newVec.resize(openNi->getJointNames().size());
        pContourPoints.push_back(newVec);
        displace.push_back(newVec);
    }
    
    */
    
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

void SkeletonFluids::update(OpenNI & openNi)
{
    SkeletonVisual::update(openNi);
    
    /*
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
     */
    
    map<int, Skeleton*>::iterator it = users.begin();
    for (; it !=users.end(); ++it) {
        for (int j=0; j<it->second->points.size(); j++) {
            ofPoint m = it->second->points[j];
            ofPoint d = it->second->velocity[j] * 10.0;
            ofPoint c = ofPoint(640*0.5, 480*0.5) - m;
            c.normalize();
            fluid.addTemporalForce(m,
                                   d,
                                   ofFloatColor(c.x,c.y,0.5)*sin(ofGetElapsedTimef()),
                                   strength);
        }
    }
    
    //fluid.setGravity(ofVec2f(gravityX, gravityY));
    fluid.dissipation = dissipation;
    fluid.velocityDissipation = velDissipation;
    
    fluid.update();
    
}

void SkeletonFluids::draw()
{
    fluid.draw();
    
    //eraseContours();
}

