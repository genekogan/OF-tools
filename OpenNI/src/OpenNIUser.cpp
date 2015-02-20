/*
 Adapted from: [ofxKinectFeatures](https://github.com/asarasua/ofxKinectFeatures)
 Real-time filters for MoCap by Skogstad et al ( http://www.uio.no/english/research/groups/fourms/projects/sma/subprojects/mocapfilters/ )
*/

#include "OpenNIUser.h"

float OpenNIUser::lpf_soft_a[] = {1,-1.2982434912,1.4634092217,-0.7106501488,0.2028836637};
float OpenNIUser::lpf_soft_b[] = {0.1851439645,0.1383283833,0.1746892243,0.1046627716,0.0464383730};
float OpenNIUser::lpf_med_a[] = {1,-1.7462227354,1.7354077932,-0.8232679111,0.1793463694};
float OpenNIUser::lpf_med_b[] = {0.1526249789,0.0333481282,0.0777551903,0.0667145281,0.0138945068};
float OpenNIUser::lpf_hard_a[] = {1,-1.9185418203,1.5929378702,-0.5939699187,0.0814687111};
float OpenNIUser::lpf_hard_b[] = {0.1400982208,-0.0343775491,0.0454003083,0.0099732061,0.0008485135};

float OpenNIUser::lpd1_soft_a[] = {1,-0.2919477037,0.5104653639,-0.01557831719,0.000283848732};
float OpenNIUser::lpd1_soft_b[] = {0.2712475020,0.1323672597,-0.0487267360,-0.1783422292,-0.1765457966};
float OpenNIUser::lpd1_med_a[] = {1,-0.9870779094,0.7774863652,-0.2206843188,0.02813441289};
float OpenNIUser::lpd1_med_b[] = {0.1973679432,-0.0056567353,-0.0321850947,-0.1099445540,-0.0495815592};
float OpenNIUser::lpd1_hard_a[] = {1,-2.057494776,1.858705877,-0.801785135,0.131076358};
float OpenNIUser::lpd1_hard_b[] = {-0.1543174259,0.1742393427,-0.0178886989,-0.0022975713,0.0002643535};

float OpenNIUser::lpd2_soft_a[] = {1,-0.8274946715,0.8110775672,-0.3530877871,0.06598917583};
float OpenNIUser::lpd2_soft_b[] = {0.1099156485,-0.1289124440,-0.0372667405,0.0216082189,0.0346553170};
float OpenNIUser::lpd2_med_a[] = {1,-1.571029458,1.459212744,-0.7173743414,0.1488005975};
float OpenNIUser::lpd2_med_b[] = {-0.0795571277,0.1390709784,-0.0479192600,-0.0031459045,-0.0084486862};
float OpenNIUser::lpd2_hard_a[] = {1,-1.628286742,1.418759018,-0.6223424612,0.1085280231};
float OpenNIUser::lpd2_hard_b[] = {-0.0738989849,0.1351624829,-0.0512998379,-0.0072918334,-0.0026718267};


OpenNIUser::OpenNIUser(ofxOpenNIUser * user)
{
    this->user = user;
    setFilterLevel(SOFT);
    setFeatureTrackingEnabled(false);
    setDepth(30);
}

OpenNIUser::~OpenNIUser()
{
    setFeatureTrackingEnabled(false);
}

void OpenNIUser::setFeatureTrackingEnabled(bool trackingEnabled)
{
    this->trackingEnabled = trackingEnabled;
    if (!trackingEnabled) {
        elements_.clear();
    }
}

void OpenNIUser::setFilterLevel(FilterLevel filterLevel)
{
    this->filterLevel = filterLevel;
    switch (filterLevel)
    {
        case HARD:
            aFilter = lpf_hard_a;
            bFilter = lpf_hard_b;
            aLpd1 = lpd1_hard_a;
            bLpd1 = lpd1_hard_b;
            aLpd2 = lpd2_hard_a;
            bLpd2 = lpd2_hard_b;
            break;
        case MEDIUM:
            aFilter = lpf_med_a;
            bFilter = lpf_med_b;
            aLpd1 = lpd1_med_a;
            bLpd1 = lpd1_med_b;
            aLpd2 = lpd2_med_a;
            bLpd2 = lpd2_med_b;
            break;
        case SOFT:
            aFilter = lpf_soft_a;
            bFilter = lpf_soft_b;
            aLpd1 = lpd1_soft_a;
            bLpd1 = lpd1_soft_b;
            aLpd2 = lpd2_soft_a;
            bLpd2 = lpd2_soft_b;
        default:
            break;
    }
}

void OpenNIUser::update()
{
    if (trackingEnabled) {
        updateFeatures();
    }
}

void OpenNIUser::updateFeatures()
{
    if (user->getJoint(JOINT_TORSO).getWorldPosition() == ofPoint(0,0,0))   return;

    if (elements_.empty())
    {
        for (int j = 0; j < user->getNumJoints(); j++) {
            MocapElement newElement(j, depth_);
            elements_.push_back(newElement);
        }
    }
    
    ofPoint headPos = user->getJoint(JOINT_HEAD).getWorldPosition();
    ofPoint torsoPos = user->getJoint(JOINT_TORSO).getWorldPosition();
    
    //for CI
    float xMax = numeric_limits<float>::min();
    float yMax = numeric_limits<float>::min();
    float zMax = numeric_limits<float>::min();
    float xMin = numeric_limits<float>::max();
    float yMin = numeric_limits<float>::max();
    float zMin = numeric_limits<float>::max();
    
    float headTorsoDist = headPos.distance(torsoPos);
    float meanVel = 0.0; //for qom

    for (int j = 0; j < user->getNumJoints(); j++)
    {
        computeJointDescriptors(j, user->getJoint((Joint) j).getWorldPosition(), headTorsoDist);
        
        //qom
        meanVel += getVelocityMagnitude(j);
        
        //ci
        xMin = min(xMin, getPositionFiltered(j).x);
        yMin = min(yMin, getPositionFiltered(j).y);
        zMin = min(zMin, getPositionFiltered(j).z);
        xMax = max(xMax, getPositionFiltered(j).x);
        yMax = max(yMax, getPositionFiltered(j).y);
        zMax = max(zMax, getPositionFiltered(j).z);
    }
    
    // Add position to history
    if (meanVels_.size() <= depth_) {
        meanVels_.insert(meanVels_.begin(), meanVel / user->getNumJoints());
    }
    if (meanVels_.size() > depth_) {
        meanVels_.pop_back();
    }
    
    qom_ = accumulate(meanVels_.begin(), meanVels_.end(), 0.0) / (meanVels_.size());
    ci_ = ( -4.0 + (( abs(xMax-xMin) + abs(yMax-yMin) + abs(zMax-zMin) ) / headTorsoDist) ) / 6.0;
    symmetry_ = 1.0 - (0.5 * (abs(sqrt(getDistanceToTorso(JOINT_RIGHT_HAND))-sqrt(getDistanceToTorso(JOINT_LEFT_HAND))) + abs(sqrt(getDistanceToTorso(JOINT_RIGHT_ELBOW))-sqrt(getDistanceToTorso(JOINT_LEFT_ELBOW)))) / headTorsoDist);
    yMaxHands_ = max(getRelativePositionToTorso(JOINT_RIGHT_HAND).y, getRelativePositionToTorso(JOINT_LEFT_HAND).y);
}

void OpenNIUser::computeJointDescriptors(int jointId, ofPoint jointPos, const float &headTorsoDist)
{
    MocapElement* mocapElement = getElement(jointId);
    
    //Position
    mocapElement->setPosition(jointPos);
    
    //Filtered position
    mocapElement->setPositionFiltered(applyFilter(mocapElement->getPosition(), mocapElement->getPositionFiltered(), aFilter, bFilter));
    
    //Velocity
    mocapElement->setVelocity(applyFilter(mocapElement->getPosition(), mocapElement->getVelocity(), aLpd1, bLpd1));
    mocapElement->setVelocityMagnitude(mocapElement->getVelocity()[0].length());
    
    //Acceleration
    mocapElement->setAcceleration(applyFilter(mocapElement->getPosition(), mocapElement->getAcceleration(), aLpd2, bLpd2));
    mocapElement->setAccelerationMagnitude(mocapElement->getAcceleration()[0].length());
    
    //Acceleration along trajectory
    ofPoint acc = mocapElement->getAcceleration()[0];
    ofPoint vel = mocapElement->getVelocity()[0];
    mocapElement->setAccelerationTrajectory(acc.dot(vel) / vel.length());
    
    //Distance to torso
    mocapElement->setDistanceToTorso(getPositionFiltered(jointId).distanceSquared(getPositionFiltered(JOINT_TORSO)));
    
    //Relative position to torso
    ofPoint relPosToTorso;
    relPosToTorso.x = (jointPos.x - getPositionFiltered(JOINT_TORSO).x) / (headTorsoDist * 1.8);
    relPosToTorso.y = (jointPos.y - getPositionFiltered(JOINT_TORSO).y) / (headTorsoDist * 1.8);
    relPosToTorso.z = -((jointPos.z - getPositionFiltered(JOINT_TORSO).z) / headTorsoDist) / 1.4;
    mocapElement->setRelativePositionToTorso(relPosToTorso);
    
    //TODO: auto hand
    //beatTracker.update(getAccTrVector(JOINT_RIGHT_HAND), get3DFiltPosVector(JOINT_RIGHT_HAND)[coord::Y]);
}

MocapElement* OpenNIUser::getElement(int _id)
{
    vector<MocapElement>::iterator it = find_if(elements_.begin(), elements_.end(), MatchId(_id));
    if (it != elements_.end()){
        return &(*it);
    } else {
        return false;
    }
}

ofPoint OpenNIUser::applyFilter(vector<ofPoint> x, vector<ofPoint> y, float *a, float *b)
{
    return b[0]*x[0] + b[1]*x[1] + b[2]*x[2] + b[3]*x[3] + b[4]*x[4] - (a[1]*y[0] + a[2]*y[1] + a[3]*y[2] + a[4]*y[3]);
}

template <typename T> vector<T> OpenNIUser::createVector(T element)
{
    vector<T> v (depth_);
    fill(v.begin(), v.begin()+depth_, element);
    return v;
}

//Descriptors getters

ofPoint OpenNIUser::getPosition(int j)
{
    if (getElement(j)) {
        return getElement(j)->getPosition()[0];
    }
    else {
        return ofPoint(0,0,0);
    }
}

vector<ofPoint> OpenNIUser::getPositionHistory(int j)
{
    if (getElement(j)) {
        return getElement(j)->getPosition();
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

vector<ofPoint> OpenNIUser::getPositionHistory(int j, int frames)
{
    if (getElement(j))
    {
        vector<ofPoint> fullHistory = getElement(j)->getPosition();
        vector<ofPoint> history(fullHistory.begin(), fullHistory.begin() + frames);
        return history;
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

ofPoint OpenNIUser::getPositionFiltered(int j)
{
    if (getElement(j)) {
        return getElement(j)->getPositionFiltered()[0];
    }
    else {
        return ofPoint(0,0,0);
    }
}

vector<ofPoint> OpenNIUser::getPositionFilteredHistory(int j)
{
    if (getElement(j)) {
        return getElement(j)->getPositionFiltered();
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

vector<ofPoint> OpenNIUser::getPositionFilteredHistory(int j, int frames)
{
    if (getElement(j)) {
        vector<ofPoint> fullHistory = getElement(j)->getPositionFiltered();
        vector<ofPoint> history(fullHistory.begin(), fullHistory.begin() + frames);
        return history;
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

ofPoint OpenNIUser::getVelocity(int j)
{
    if (getElement(j)) {
        return getElement(j)->getVelocity()[0];
    }
    else {
        return ofPoint(0.0,0.0,0.0);
    }
}

vector<ofPoint> OpenNIUser::getVelocityHistory(int j)
{
    if (getElement(j)) {
        return getElement(j)->getVelocity();
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

vector<ofPoint> OpenNIUser::getVelocityHistory(int j, int frames)
{
    if (getElement(j))
    {
        vector<ofPoint> fullHistory = getElement(j)->getVelocity();
        vector<ofPoint> history(fullHistory.begin(), fullHistory.begin() + frames);
        return history;
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

float OpenNIUser::getVelocityMagnitude(int j)
{
    if (getElement(j)) {
        return getElement(j)->getVelocity()[0].length();
    }
    else {
        return 0.0;
    }
}

float & OpenNIUser::getVelocityMagnitudeRef(int j)
{
    if (getElement(j)) {
        return getElement(j)->getVelocityMagnitudeRef()[0];
    }
}

float OpenNIUser::getVelocityMean(int j, int frames)
{
    if (getElement(j))
    {
        float sum = 0.0;
        for (int i = 0; i < frames && i < getElement(j)->getVelocity().size(); i++) {
            sum += getElement(j)->getVelocity()[i].length();
        }
        if (frames <= getElement(j)->getVelocity().size()) {
            return sum / frames;
        }
        else {
            return sum / getElement(j)->getVelocity().size();
        }
    }
    else {
        return 0.0;
    }
}

float & OpenNIUser::getVelocityMeanRef(int j)
{
    if (getElement(j)) {
        return getElement(j)->getVelocityMeanRef()[0];
    }
}


ofPoint OpenNIUser::getAcceleration(int j)
{
    if (getElement(j)) {
        return getElement(j)->getAcceleration()[0];
    }
    else {
        return ofPoint(0.0,0.0,0.0);
    }
}

vector<ofPoint> OpenNIUser::getAccelerationHistory(int j)
{
    if (getElement(j)) {
        return getElement(j)->getAcceleration();
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

vector<ofPoint> OpenNIUser::getAccelerationHistory(int j, int frames)
{
    if (getElement(j)) {
        vector<ofPoint> fullHistory = getElement(j)->getAcceleration();
        vector<ofPoint> history(fullHistory.begin(), fullHistory.begin() + frames);
        return history;
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

float OpenNIUser::getAccelerationMagnitude(int j)
{
    if (getElement(j)) {
        return getElement(j)->getAcceleration()[0].length();
    }
    else {
        return 0.0;
    }
}

float & OpenNIUser::getAccelerationMagnitudeRef(int j)
{
    if (getElement(j)) {
        return getElement(j)->getAccelerationMagnitudeRef()[0];
    }
}

float OpenNIUser::getAccelerationMean(int j, int frames)
{
    if (getElement(j))
    {
        float sum = 0.0;
        for (int i = 0; i < frames && i < getElement(j)->getAcceleration().size(); i++) {
            sum += getElement(j)->getAcceleration()[i].length();
        }
        if (frames <= getElement(j)->getAcceleration().size()) {
            return sum / frames;
        } else {
            return sum / getElement(j)->getAcceleration().size();
        }
    }
    else {
        return 0.0;
    }
}

float & OpenNIUser::getAccelerationMeanRef(int j)
{
    if (getElement(j)) {
        return getElement(j)->getAccelerationMeanRef()[0];
    }
}

float OpenNIUser::getAccelerationTrajectory(int j)
{
    if (getElement(j)){
        return getElement(j)->getAccelerationTrajectory()[0];
    }
    else {
        return 0.0;
    }
}

vector<float> OpenNIUser::getAccelerationTrajectoryHistory(int j)
{
    if (getElement(j)) {
        return getElement(j)->getAccelerationTrajectory();
    }
    else {
        return createVector(0.0f);
    }
}

vector<float> OpenNIUser::getAccelerationTrajectoryHistory(int j, int frames)
{
    if (getElement(j))
    {
        vector<float> fullHistory = getElement(j)->getAccelerationTrajectory();
        vector<float> history(fullHistory.begin(), fullHistory.begin() + frames);
        return history;
    }
    else {
        return createVector(0.0f);
    }
}

float OpenNIUser::getAccelerationTrajectoryMean(int j, int frames)
{
    if (getElement(j))
    {
        float sum = 0.0;
        for (int i = 0; i < frames && i < getElement(j)->getAccelerationTrajectory().size(); i++) {
            sum += getElement(j)->getAccelerationTrajectory()[i];
        }
        if (frames <= getElement(j)->getAccelerationTrajectory().size()) {
            return sum / frames;
        }
        else {
            return sum / getElement(j)->getAccelerationTrajectory().size();
        }
    }
    else {
        return 0.0;
    }
}

float OpenNIUser::getDistanceToTorso(int j)
{
    if (getElement(j)){
        return getElement(j)->getDistanceToTorso()[0];
    }
    else {
        return 0.0;
    }
}

vector<float> OpenNIUser::getDistanceToTorsoHistory(int j)
{
    if (getElement(j)) {
        return getElement(j)->getDistanceToTorso();
    }
    else {
        return createVector(0.0f);
    }
}

vector<float> OpenNIUser::getDistanceToTorsoHistory(int j, int frames)
{
    if (getElement(j))
    {
        vector<float> fullHistory = getElement(j)->getDistanceToTorso();
        vector<float> history(fullHistory.begin(), fullHistory.begin() + frames);
        return history;
    }
    else {
        return createVector(0.0f);
    }
}

ofPoint OpenNIUser::getRelativePositionToTorso(int j)
{
    if (getElement(j)) {
        return getElement(j)->getRelativePositionToTorso()[0];
    }
    else {
        return ofPoint(0.0,0.0,0.0);
    }
}

vector<ofPoint> OpenNIUser::getRelativePositionToTorsoHistory(int j)
{
    if (getElement(j)) {
        return getElement(j)->getRelativePositionToTorso();
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

vector<ofPoint> OpenNIUser::getRelativePositionToTorsoHistory(int j, int frames)
{
    if (getElement(j))
    {
        vector<ofPoint> fullHistory = getElement(j)->getRelativePositionToTorso();
        vector<ofPoint> history(fullHistory.begin(), fullHistory.begin() + frames);
        return history;
    }
    else {
        return createVector(ofPoint(0.0,0.0,0.0));
    }
}

float OpenNIUser::getQom()
{
    return qom_;
}

float OpenNIUser::getCI()
{
    return ci_;
}

float OpenNIUser::getSymmetry()
{
    return symmetry_;
}

float OpenNIUser::getYMaxHands()
{
    return yMaxHands_;
}

/////// REFERENCES

ofPoint & OpenNIUser::getPositionRef(int j)
{
    if (getElement(j)) {
        return getElement(j)->getPositionRef()[0];
    }
}

ofPoint & OpenNIUser::getPositionFilteredRef(int j)
{
    if (getElement(j)) {
        return getElement(j)->getPositionFilteredRef()[0];
    }
}

ofPoint & OpenNIUser::getVelocityRef(int j)
{
    if (getElement(j)) {
        return getElement(j)->getVelocityRef()[0];
    }
}

ofPoint & OpenNIUser::getAccelerationRef(int j)
{
    if (getElement(j)) {
        return getElement(j)->getAccelerationRef()[0];
    }
}

float & OpenNIUser::getAccelerationTrajectoryRef(int j)
{
    if (getElement(j)){
        return getElement(j)->getAccelerationTrajectoryRef()[0];
    }
}

float & OpenNIUser::getDistanceToTorsoRef(int j)
{
    if (getElement(j)){
        return getElement(j)->getDistanceToTorsoRef()[0];
    }
}

ofPoint & OpenNIUser::getRelativePositionToTorsoRef(int j)
{
    if (getElement(j)) {
        return getElement(j)->getRelativePositionToTorsoRef()[0];
    }
}

float & OpenNIUser::getQomRef()
{
    return qom_;
}

float & OpenNIUser::getCIRef()
{
    return ci_;
}

float & OpenNIUser::getSymmetryRef()
{
    return symmetry_;
}

float & OpenNIUser::getYMaxHandsRef()
{
    return yMaxHands_;
}
