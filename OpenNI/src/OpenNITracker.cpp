#include "OpenNITracker.h"

OpenNI::OpenNI()
{
    numTrackedUsers = 0;
    
    // contour parameters
    minArea = 5000;
    maxArea = 140000;
    threshold = 15;
    persistence = 15;
    maxDistance = 32;
    nearThreshold = 11;
    farThreshold = 0;
    smoothingRate = 1;
    
    numFrames = 1024;
    
    depthHistory.resize(numFrames);
    
    panel.setName("OpenNI");
    panel.setPosition(10, 10);
    panel.addToggle("track contours", &trackingContours, this, &OpenNI::eventSetTrackingContours);
    panel.addToggle("track users", &trackingUsers, this, &OpenNI::eventSetTrackingUsers);
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

void OpenNI::stop()
{
    kinect.stop();
}

void OpenNI::setTrackingUsers(bool trackingUsers)
{
    this->trackingUsers = trackingUsers;
    if (trackingUsers)
    {
        userTrackingParameters.push_back(new Parameter<bool>("track features", &trackingUserFeatures));
        //panel.addToggle("user tracking", userTrackingParameters, this, &OpenNI::eventSetTrackingUserFeatures);
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
//        panel.removeWidget("user tracking");
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
        contourTrackingParameters.push_back(new Parameter<bool>("simplified", &simplified));
        contourTrackingParameters.push_back(new Parameter<int>("delay", &delay, 0, numFrames));
        
        //panel.addParameter("contour tracking", contourTrackingParameters);
    }
    else
    {
        for (auto p : contourTrackingParameters) {
            delete p;
        }
        contourTrackingParameters.clear();
        //panel.removeWidget("contour tracking");
    }
}

ofVec3f OpenNI::getWorldCoordinateAt(int x, int y)
{
    //ofPoint depthPoint = ofPoint(x, y, depthPixels[x + y * 640]);
    
    int idx = (idxHistory - 1 - delay + numFrames) % numFrames;
    ofPoint depthPoint = ofPoint(x, y, depthHistory[idx][x + y * 640]);
    
    ofVec3f worldPoint = kinect.projectiveToWorld(depthPoint);
    return worldPoint;
}

ofVec2f OpenNI::getProjectedPointAt(int x, int y)
{
    return kpt.getProjectedPoint(getWorldCoordinateAt(x, y));
}

void OpenNI::getCalibratedContour(int idx, vector<ofVec2f> & calibratedPoints, int width, int height, float smoothness)
{
    smoothness = max(smoothness, 1.0f);
    ofVec2f projectedPoint, mappedPoint;
    
    //vector<cv::Point> & points = contourFinder.getContour(idx);
    
    ofPolyline &line = contourFinder.getPolyline(idx);
    line.simplify(smoothness);
    vector<ofPoint> & vertices = line.getVertices();
    for (float j = 0; j < vertices.size(); j++)
    {
        projectedPoint.set(kpt.getProjectedPoint(getWorldCoordinateAt(vertices[j].x, vertices[j].y)));
        mappedPoint.set(width * projectedPoint.x, height * projectedPoint.y);
        calibratedPoints.push_back(mappedPoint);
    }

}

bool OpenNI::update()
{
    panel.update();
    
    kinect.update();
    if (kinect.isNewFrame())
    {
        depthHistory[idxHistory] = kinect.getDepthRawPixels();
        idxHistory = (idxHistory + 1) % numFrames;
        
        if (calibrating) {
            updateCalibration();
        }

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
    int idx = (idxHistory - 1 - delay + numFrames) % numFrames;
    grayImage.setFromPixels(depthHistory[idx]);
    
    grayThreshNear = grayImage;
    grayThreshFar = grayImage;
    grayThreshNear.threshold(nearThreshold, true);
    grayThreshFar.threshold(farThreshold);
    cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
    grayImage.flagImageChanged();
    contourFinder.setMinArea(minArea);
    contourFinder.setMaxArea(maxArea);
    contourFinder.setThreshold(threshold);
    contourFinder.setSimplify(simplified);
    contourFinder.getTracker().setPersistence(persistence);
    contourFinder.getTracker().setMaximumDistance(maxDistance);
    contourFinder.getTracker().setSmoothingRate(smoothingRate);
    contourFinder.findContours(grayImage);
    numContours = contourFinder.size();
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
    if (calibrating)
    {
        drawCalibration();
    }
    else
    {
        ofPushMatrix();
        
        ofTranslate(200, 0);
        
        if (trackingContours)
        {
            grayImage.draw(0, 0);
            ofSetColor(255, 0, 0);
            ofSetLineWidth(4);
            contourFinder.draw();
        }
        else {
            kinect.drawDepth();
        }
        
        if (trackingUsers)
        {
            kinect.drawSkeletons();
            drawFeatureExtractor();
        }
        
        ofPopMatrix();
    }
    
    panel.draw();
}

void OpenNI::drawFeatureExtractor()
{
    int j = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 14);
    int f = 0;
    
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
            case 0:
                os << "Velocity magnitude mean" << endl;
                ofDrawBitmapString(ofToString(users[1]->getVelocityMean((Joint)j)), jointProjectivePosition.x, jointProjectivePosition.y);
                break;
            case 1:
                os << "Acceleration along y axis (up-down movement)" << endl;
                ofDrawBitmapString(ofToString(users[1]->getAcceleration((Joint)j).y), jointProjectivePosition.x, jointProjectivePosition.y);
                break;
            case 2:
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

inline bool OpenNI::isNewSkeletonDataAvailable(ofxOpenNIUser & user)
{
    return (user.getJoint(JOINT_TORSO).getWorldPosition() != ofPoint(0,0,0) && (users.count(user.getXnID()) || user.getJoint(JOINT_TORSO).getWorldPosition() != users[user.getXnID()]->getPosition(0) ));
}

void OpenNI::eventSetTrackingUsers(GuiElementEventArgs & b)
{
    setTrackingUsers(trackingUsers);
}

void OpenNI::eventSetTrackingContours(GuiElementEventArgs & b)
{
    setTrackingContours(trackingContours);
}

void OpenNI::eventSetTrackingUserFeatures(GuiElementEventArgs & b)
{
    if (b.name == "track features"){
        setTrackingUserFeatures(trackingUserFeatures);
    }
}

void OpenNI::eventToggleCalibrationModule(GuiElementEventArgs & b)
{
    if (calibrating) {
        startCalibrationModule();
    }
    else {
        stopCaibrationModule();
    }
}

void OpenNI::eventUser(ofxOpenNIUserEvent & event)
{
    ofLog(OF_LOG_NOTICE, "User event: " + ofToString(event.userStatus) + " " + ofToString(event.id));
    
    if (event.userStatus == USER_TRACKING_STARTED) {
        ofLog(OF_LOG_NOTICE, "OpenNI: TRACK STARTED FOR  :: "+ofToString(event.id));
    }
    else if (event.userStatus == USER_TRACKING_STOPPED)
    {
        ofLog(OF_LOG_NOTICE, "OpenNI: TRACK STOPPED FOR  :: "+ofToString(event.id));
        if (users.count(event.id) > 0)
        {
            delete users[event.id];
            users.erase(event.id);
        }
    }
    else if (event.userStatus == USER_CALIBRATION_STOPPED) {
        ofLog(OF_LOG_NOTICE, "OpenNI: CALIB STOPPED FOR  :: "+ofToString(event.id));
    }
    else if (event.userStatus == USER_CALIBRATION_STARTED) {
        ofLog(OF_LOG_NOTICE, "OpenNI: CALIB STARTED FOR  :: "+ofToString(event.id));
    }
    else if (event.userStatus == USER_SKELETON_LOST) {
        ofLog(OF_LOG_NOTICE, "OpenNI: SKEL LOST FOR  :: "+ofToString(event.id));
    }
    else if (event.userStatus == USER_SKELETON_FOUND) {
        ofLog(OF_LOG_NOTICE, "OpenNI: SKEL FOUNd FOR  :: "+ofToString(event.id));
    }
}


////////////////////////////////////////////////////////////////
//  Calibration

void OpenNI::enableCalibration(ofxSecondWindow & window)
{
    this->window = &window;
    panel.addToggle("calibration", &calibrating, this, &OpenNI::eventToggleCalibrationModule);
}

void OpenNI::eventAddPointPairs(GuiElementEventArgs & b)
{
    vector<cv::Point2f> & cvPoints = calibration.getChessboardCorners();
    vector<ofVec3f> worldPoints;
    for (int i=0; i<cvPoints.size(); i++)
    {
        ofVec3f worldPoint = getWorldCoordinateAt(cvPoints[i].x, cvPoints[i].y);
        if (worldPoint.z > 0) {
            worldPoints.push_back(worldPoint);
        }
    }
    calibration.addPointPairs(worldPoints);
}

void OpenNI::eventCalibrate(GuiElementEventArgs & b)
{
    calibration.calibrate(kpt);
}

void OpenNI::eventSaveCalibration(GuiElementEventArgs & b)
{
    calibration.saveCalibration(kpt);
}

void OpenNI::eventLoadCalibration(GuiElementEventArgs & b)
{
    calibration.loadCalibration(kpt);
}

void OpenNI::saveCalibration(string filename)
{
    kpt.saveCalibration(filename);
}

void OpenNI::loadCalibration(string filename)
{
    kpt.loadCalibration(filename);
}

void OpenNI::startCalibrationModule()
{
    calibrating = true;
    panel.addSlider("x", &calibration.getChessboard().x, 0, window->getWidth());
    panel.addSlider("y", &calibration.getChessboard().y, 0, window->getHeight());
    panel.addSlider("size", &calibration.getChessboard().size, 10, 800);
    panel.addButton("add point", this, &OpenNI::eventAddPointPairs);
    panel.addButton("calibrate", this, &OpenNI::eventCalibrate);
    panel.addButton("save", this, &OpenNI::eventSaveCalibration);
    panel.addButton("load", this, &OpenNI::eventLoadCalibration);
    calibration.setup(window->getWidth(), window->getHeight());
    updateCalibration();
}

void OpenNI::stopCaibrationModule()
{
    calibrating = false;
    calibration.stop();
    /*
    panel.removeWidget("x");
    panel.removeWidget("y");
    panel.removeWidget("size");
    panel.removeWidget("add point");
    panel.removeWidget("calibrate");
    panel.removeWidget("save");
    panel.removeWidget("load");
     */
}

void OpenNI::updateCalibration()
{
    rgbImage.setFromPixels(kinect.getImagePixels());
    calibration.searchForCorners(rgbImage);
}

void OpenNI::testCalibration()
{
    ofPoint testPoint(ofClamp(ofGetMouseX()-200, 0, kinect.getWidth()-1), ofClamp(ofGetMouseY(), 0, kinect.getHeight()-1));
    ofVec3f worldPoint = getWorldCoordinateAt(testPoint.x, testPoint.y);
    ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);
    calibration.drawTestingPoint(projectedPoint);
}

void OpenNI::drawCalibration()
{
    kinect.drawImage(200, 0);
    calibration.draw(window);
}

