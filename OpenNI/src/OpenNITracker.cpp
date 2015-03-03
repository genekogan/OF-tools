#include "OpenNITracker.h"

OpenNI::OpenNI()
{
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

    // user tracking
    numTrackedUsers = 0;
    maxUsers = 1;
    
    depthHistory.resize(numFrames);
    
    panel.disableControlRow();
    panel.setName("OpenNI");
    panel.addToggle("track contours", &trackingContours, this, &OpenNI::eventSetTrackingContours);
    panel.addToggle("track users", &trackingUsers, this, &OpenNI::eventSetTrackingUsers);
}

OpenNI::~OpenNI()
{
    ofRemoveListener(kinect.userEvent, this, &OpenNI::eventUser);
    clearUsers();
    stop();
}

void OpenNI::setup(string oni)
{
    oni != "" ? kinect.setupFromONI(oni) : kinect.setup();
    kinect.addDepthGenerator();
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
        GuiWidget *widget = panel.addWidget("user tracking");
        widget->addToggle("track features", &trackingUserFeatures, this, &OpenNI::eventSetTrackingUserFeatures);
        widget->addSlider("max users", &maxUsers, 1, 8, this, &OpenNI::eventSetMaxUsers);
        
        kinect.addUserGenerator();
        kinect.setMaxNumUsers(maxUsers);
        kinect.setUseMaskPixelsAllUsers(false);
        kinect.setUseMaskTextureAllUsers(false);
        kinect.setUsePointCloudsAllUsers(false);
    }
    else
    {
        setTrackingUserFeatures(false);
        panel.removeElement("user tracking");
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
        clearUsers();
    }
}

void OpenNI::setTrackingContours(bool trackingContours)
{
    this->trackingContours = trackingContours;
    if (trackingContours)
    {
        GuiWidget *widget = panel.addWidget("contour tracking");
        widget->addSlider("farThreshold", &farThreshold, 0, 255);
        widget->addSlider("nearThreshold", &nearThreshold, 0, 255);
        widget->addSlider("minArea", &minArea, 0, 100000);
        widget->addSlider("maxArea", &maxArea, 2500, 150000);
        widget->addSlider("threshold", &threshold, 0, 255);
        widget->addSlider("persistence", &persistence, 0, 100);
        widget->addSlider("maxDistance", &maxDistance, 0, 100);
        widget->addSlider("smoothingRate", &smoothingRate, 0, 100);
        widget->addToggle("simplified", &simplified);
        widget->addSlider("delay", &delay, 0, numFrames);
    }
    else
    {
        panel.removeElement("contour tracking");
    }
}

ofVec3f OpenNI::getWorldCoordinateAt(int x, int y)
{
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
        if (trackingUsers) {
            updateUsers();
        }
        if (trackingContours) {
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

void OpenNI::clearUsers()
{
    map<int, OpenNIUser*>::iterator it = users.begin();
    for (; it != users.end(); ++it) {
        delete it->second;
    }
    users.clear();
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
    if (calibrating) {
        drawCalibration();
    }
    else {
        drawDebug();
    }
    
    panel.draw();
}

void OpenNI::drawDebug()
{
    ofPushMatrix();
    ofPushStyle();
    
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
    
    if (trackingUsers) {
        kinect.drawSkeletons();
    }
    
    ofPopStyle();
    ofPopMatrix();
}

inline bool OpenNI::isNewSkeletonDataAvailable(ofxOpenNIUser & user)
{
    return (user.getJoint(JOINT_TORSO).getWorldPosition() != ofPoint(0,0,0) && (users.count(user.getXnID()) || user.getJoint(JOINT_TORSO).getWorldPosition() != users[user.getXnID()]->getPosition(0) ));
}

void OpenNI::eventSetTrackingUsers(GuiElementEventArgs & b)
{
    setTrackingUsers(trackingUsers);
}

void OpenNI::eventSetMaxUsers(GuiElementEventArgs & e)
{
    kinect.setMaxNumUsers(maxUsers);
}

void OpenNI::eventSetTrackingContours(GuiElementEventArgs & b)
{
    setTrackingContours(trackingContours);
}

void OpenNI::eventSetTrackingUserFeatures(GuiElementEventArgs & b)
{
    setTrackingUserFeatures(trackingUserFeatures);
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
    panel.addToggle("calibrate", &calibrating, this, &OpenNI::eventToggleCalibrationModule);
}

void OpenNI::eventAddPointPairs(GuiElementEventArgs & e)
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

void OpenNI::eventCalibrate(GuiElementEventArgs & e)
{
    calibration.calibrate(kpt);
}

void OpenNI::eventSaveCalibration(GuiElementEventArgs & e)
{
    calibration.saveCalibration(kpt);
}

void OpenNI::eventLoadCalibration(GuiElementEventArgs & e)
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
    setTrackingUsers(false);
    setTrackingContours(false);
    calibrating = true;
    
    GuiWidget *widget = panel.addWidget("calibration");
    widget->addSlider("x", &calibration.getChessboard().x, 0, window->getWidth());
    widget->addSlider("y", &calibration.getChessboard().y, 0, window->getHeight());
    widget->addSlider("size", &calibration.getChessboard().size, 10, 800);
    widget->addButton("add point", this, &OpenNI::eventAddPointPairs);
    widget->addButton("calibrate", this, &OpenNI::eventCalibrate);
    widget->addButton("save", this, &OpenNI::eventSaveCalibration);
    widget->addButton("load", this, &OpenNI::eventLoadCalibration);
    
    kinect.addImageGenerator();
    calibration.setup(window->getWidth(), window->getHeight());
    updateCalibration();
}

void OpenNI::stopCaibrationModule()
{
    calibrating = false;
    calibration.stop();
    
    kinect.removeImageGenerator();
    panel.removeElement("calibration");
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

