#include "OpenNI.h"

OpenNI::OpenNI()
{
    numTrackedUsers = 0;
    
    // contour parameters
    minArea = 5000;
    maxArea = 140000;
    threshold = 15;
    persistence = 15;
    maxDistance = 32;
    nearThreshold = 8;
    farThreshold = 0;
    smoothingRate = 1;
    
    panel.setName("OpenNI");
    panel.setPosition(10, 10);
    panel.addParameter("track contours", &trackingContours, this, &OpenNI::eventSetTrackingContours);
    panel.addParameter("track users", &trackingUsers, this, &OpenNI::eventSetTrackingUsers);
    panel.addParameter("tilt", &kinectTiltAngle, -1.0f, 1.0f, this, &OpenNI::eventAdjustKinectTilt);
}

OpenNI::~OpenNI()
{
}

void OpenNI::stop()
{
    kinect.stop();
}

void OpenNI::setup(string oni)
{
    
    if (oni != "") {
        kinect.setupFromONI(oni);
    }
    else {
        kinect.setup();
    }
    kinect.addDepthGenerator();
    kinect.addImageGenerator();   // optional?
    kinect.setRegister(true);
    kinect.setMirror(false);
    kinect.setUseDepthRawPixels(true);
    kinect.setDepthColoring(COLORING_GREY);
    kinect.start();
    
    setTrackingUsers(false);
    setTrackingUserFeatures(false);
    setTrackingContours(false);
     
    
    ofAddListener(kinect.userEvent, this, &OpenNI::eventUser);
}

void OpenNI::setTrackingUsers(bool trackingUsers)
{
    this->trackingUsers = trackingUsers;
    if (trackingUsers)
    {
        userTrackingParameters.push_back(new Parameter<bool>("track features", &trackingUserFeatures));
        panel.addParameter("user tracking", userTrackingParameters, this, &OpenNI::eventSetTrackingUserFeatures);
        kinect.addUserGenerator();
        
        kinect.setMaxNumUsers(1);
        kinect.setUseMaskPixelsAllUsers(false);
        kinect.setUseMaskTextureAllUsers(false);
        kinect.setUsePointCloudsAllUsers(false);
        kinect.setPointCloudDrawSizeAllUsers(1);
        kinect.setPointCloudResolutionAllUsers(1);

    }
    else
    {
        setTrackingUserFeatures(false);
        for (auto p : userTrackingParameters) {
            delete p;
        }
        userTrackingParameters.clear();
        panel.removeWidget("user tracking");
        kinect.removeUserGenerator();
    }
}

void OpenNI::setTrackingUserFeatures(bool trackingUserFeatures)
{
    this->trackingUserFeatures = trackingUserFeatures;
    if (trackingUserFeatures)
    {
        map<int, OpenNIUser*>::iterator it = users.begin();
        for (; it != users.end(); ++it) {
            it->second->setFeatureTrackingEnabled(true);
        }
    }
    else
    {
        map<int, OpenNIUser*>::iterator it = users.begin();
        for (; it != users.end(); ++it) {
            delete it->second;
        }
        users.clear();
    }
}

void OpenNI::setTrackingContours(bool trackingContours)
{
    this->trackingContours = trackingContours;
    if (trackingContours)
    {
        contourTrackingParameters.push_back(new Parameter<int>("farThreshold", &farThreshold, 0, 255));
        contourTrackingParameters.push_back(new Parameter<int>("nearThreshold", &nearThreshold, 0, 255));
        contourTrackingParameters.push_back(new Parameter<int>("minArea", &minArea, 0, 100000));
        contourTrackingParameters.push_back(new Parameter<int>("maxArea", &maxArea, 2500, 150000));
        contourTrackingParameters.push_back(new Parameter<int>("threshold", &threshold, 0, 255));
        contourTrackingParameters.push_back(new Parameter<int>("persistence", &persistence, 0, 100));
        contourTrackingParameters.push_back(new Parameter<int>("maxDistance", &maxDistance, 0, 100));
        contourTrackingParameters.push_back(new Parameter<int>("smoothingRate", &smoothingRate, 0, 100));

        panel.addParameter("contour tracking", contourTrackingParameters);
    }
    else
    {
        for (auto p : contourTrackingParameters) {
            delete p;
        }
        contourTrackingParameters.clear();
        panel.removeWidget("contour tracking");
    }
}

bool OpenNI::update()
{
    panel.update();
    
    kinect.update();
    if (kinect.isNewFrame())
    {
        depthPixels = kinect.getDepthRawPixels();
        
        if (trackingUsers)
        {
            updateUsers();
        }
        
        if (trackingContours)
        {
            updateContours();
        }
        return true;
    }
    else
    {
        return false;
    }
}

void OpenNI::updateUsers()
{
    numTrackedUsers = kinect.getNumTrackedUsers();
    
    for (int i = 0; i < numTrackedUsers; i++)
    {
        ofxOpenNIUser * user = &kinect.getTrackedUser(i);
        
        
        if (user->isSkeleton() && users.count(user->getXnID()) == 0)
        {
            OpenNIUser *newUser = new OpenNIUser(user);
            newUser->setFeatureTrackingEnabled(trackingUserFeatures);
            users[user->getXnID()] = newUser;
        }
        else if (trackingUserFeatures)
        {
            if (isNewSkeletonDataAvailable(*user)) {
                users[user->getXnID()]->update();
            }
        }
    }
    
    
    resetUserGenerator();
}

void OpenNI::updateSkeletonFeatures()
{
}

void OpenNI::updateContours()
{
    grayImage.setFromPixels(depthPixels);
    grayThreshNear = grayImage;
    grayThreshFar = grayImage;
    grayThreshNear.threshold(nearThreshold, true);
    grayThreshFar.threshold(farThreshold);
    cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
    grayImage.flagImageChanged();
    contourFinder.setMinArea(minArea);
    contourFinder.setMaxArea(maxArea);
    contourFinder.setThreshold(threshold);
    contourFinder.getTracker().setPersistence(persistence);
    contourFinder.getTracker().setMaximumDistance(maxDistance);
    contourFinder.getTracker().setSmoothingRate(smoothingRate);
    contourFinder.findContours(grayImage);
}



void OpenNI::resetUserGenerator()
{
    if (kinect.getNumTrackedUsers()) {
        hadUsers = true;
    }
    else if (!kinect.getNumTrackedUsers() && hadUsers)
    {
        hadUsers = false;
        kinect.setPaused(true);
        kinect.removeUserGenerator();
        kinect.addUserGenerator();
        kinect.setPaused(false);
    }
}

void OpenNI::draw()
{
    ofPushMatrix();
    
    kinect.drawImage();
    ofTranslate(640, 0);
    kinect.drawDepth();
    ofTranslate(-640, 0);

    if (trackingUsers)
    {
        kinect.drawSkeletons();
        drawFeatureExtractor();
    }
    
    if (trackingContours)
    {
        ofTranslate(640, 0);
        grayImage.draw(0, 0);
        ofSetColor(255, 0, 0);
        ofSetLineWidth(4);
        contourFinder.draw();
    }

    ofPopMatrix();
    
    panel.draw();
}

void OpenNI::drawFeatureExtractor()
{
    int j = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 14);
    int f = VELOCITY_MEAN;
    
    ostringstream os;
    os << "ofxKinectFeatures example " << endl;
    os << "FPS: " << ofGetFrameRate() << endl;
    if (users.count(1)) {
        ofPoint jointProjectivePosition = kinect.worldToProjective(users[1]->getPosition((Joint)j));
        os << "Quantity of Motion: " << users[1]->getQom() << endl;
        os << "Symmetry: " << users[1]->getSymmetry() << endl;
        os << "Contraction Index: " << users[1]->getCI() << endl << endl;
        os << "Current joint (left-right to change): " << getJointAsString((Joint)j) << endl;
        os << "Current feature (up-down to change): ";
        switch (f) {
            case VELOCITY_MEAN:
                os << "Velocity magnitude mean" << endl;
                ofDrawBitmapString(ofToString(users[1]->getVelocityMean((Joint)j)), jointProjectivePosition.x, jointProjectivePosition.y);
                break;
            case ACCELERATION_Y:
                os << "Acceleration along y axis (up-down movement)" << endl;
                ofDrawBitmapString(ofToString(users[1]->getAcceleration((Joint)j).y), jointProjectivePosition.x, jointProjectivePosition.y);
                break;
            case RELPOSTOTORSO_X:
                os << "Relative position to torso in x axis" << endl;
                ofDrawBitmapString(ofToString(users[1]->getRelativePositionToTorso((Joint)j).x), jointProjectivePosition.x, jointProjectivePosition.y);
                break;
            default:
                break;
        }
    }
    
    ofSetColor(0,0,0,100);
    ofRect(10, 10, 500, 150);
    ofSetColor(255,255,255);
    ofDrawBitmapString(os.str(), 20, 30);
}

