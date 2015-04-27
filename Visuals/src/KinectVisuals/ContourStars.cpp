#include "ContourStars.h"

/*
static bool shouldRemove(ofPtr<ofxBox2dBaseShape>shape) {
    return !ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape.get()->getPosition());
}
 */



void ContourStars::setup(int width, int height)
{
    ContourVisual::setup(width, height);

    contourColor = ofFloatColor(1.0, 0.8, 0.1, 0.7);
    contourSmoothness = 2;

    panel.setName("stars");
    panel.addSlider("rate", &rate, 1, 20);
    panel.addSlider("tolerance", &tolerance, 0.0f, 100.0f);
    panel.addSlider("circleDensity", &circleDensity, 0.0f, 1.0f);
    panel.addSlider("circleBounce", &circleBounce, 0.0f, 1.0f);
    panel.addSlider("circleFriction", &circleFriction, 0.0f, 1.0f);
    panel.addColor("contour color", &contourColor);
    panel.addSlider("contour smooth", &contourSmoothness, 1, 10);
    panel.addButton("clear", this, &ContourStars::clearCircles);

    ofImage img1, img2, img3;
    
    img1.loadImage("/Users/gene/Downloads/lisa_chung.png");
    img2.loadImage("/Users/gene/Downloads/gene-kogan.png");
    img3.loadImage("/Users/gene/Downloads/colin-serious-twitter.png");
    img1.resize(40, 40);
    img2.resize(40, 40);
    img3.resize(40, 40);
    img.push_back(img1);
    img.push_back(img2);
    img.push_back(img3);
    
	box2d.init();
	box2d.setGravity(0, 5);
	//box2d.createGround(ofPoint(0, secondWindow.getHeight()), ofPoint(secondWindow.getWidth(), secondWindow.getHeight()));
    
    rate = 5;
    tolerance = 0.3f;
    circleDensity = 0.3;
    circleBounce = 0.5;
    circleFriction = 0.1;
}

void ContourStars::update(OpenNI & openNi)
{
    ContourVisual::update(openNi);
    
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
//		circles.push_back(circle);
        circles.push_back(new IdCircle(circle, ofRandom(img.size())));
	}
	
    // update box2d
    //ofRemove(circles, shouldRemove);
    
    vector<IdCircle*>::iterator it = circles.begin();
    while (it != circles.end())
    {
        if ( !ofRectangle(0, -200, width+400, height+400).inside((*it)->circle->getPosition()) )
        {
            (*it)->circle->destroy();
            circles.erase(it);
        }
        else {
            ++it;
        }
    }
    
    box2d.update();
}

void ContourStars::draw()
{
    ofClear(0, 0);
    
    ofPushStyle();
    ofEnableAlphaBlending();
    for(int i = 0; i < currentContours.size(); i++) {
        ofBeginShape();
        ofFill();
        ofSetColor(contourColor);
        for (int j=0; j<currentContours[i].size(); j+=contourSmoothness) {
            ofCurveVertex(currentContours[i][j].x, currentContours[i][j].y);
        }
        ofEndShape();
    }
    ofPopStyle();
    
    
    for (int i=0; i<circles.size(); i++)
    {
        ofPushMatrix();
        ofPushStyle();

		ofFill();
		//ofSetHexColor(0xc0dd3b);
		ofSetColor(255);
        
        ofVec2f pos = circles[i]->circle.get()->getPosition();
        float rad = circles[i]->circle.get()->getRadius();
        float ang = circles[i]->circle.get()->getRotation();
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofTranslate(pos.x, pos.y);
        ofRotate(ang);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        
        
        img[circles[i]->idx % 3].draw(0, 0, 2*rad, 2*rad);
        ofDisableBlendMode();
        ofPopStyle();
        ofPopMatrix();
	}
    
    //eraseContours();
}

void ContourStars::clearCircles(GuiButtonEventArgs & evt)
{
    circles.clear();
}