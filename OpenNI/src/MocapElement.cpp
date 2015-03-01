/*
 Adapted from: [ofxKinectFeatures](https://github.com/asarasua/ofxKinectFeatures)
*/

#include "MocapElement.h"

MocapElement::MocapElement()
{
    initialize();
}

MocapElement::MocapElement(int elementId, int depth)
{
    elementId_ = elementId;
    historyDepth_ = depth;
    historyWeight_ = 1.0 / (float) historyDepth_;
    initialize();
}

void MocapElement::initialize()
{
    ofPoint zeros = ofPoint(0.0,0.0,0.0);
    for (int i = 0; i < historyDepth_; i++)
    {
        setPosition(zeros);
        setPositionFiltered(zeros);
        setVelocity(zeros);
        setVelocityMagnitude(0.0);
        setVelocityMean(0.0);
        setAcceleration(zeros);
        setAccelerationMagnitude(0.0);
        setAccelerationMean(0.0);
        setAccelerationTrajectory(0.0);
        setDistanceToTorso(0.0);
        setRelativePositionToTorso(zeros);
    }
}

void MocapElement::setHistoryDepth(int depth)
{
    historyDepth_ = depth;
    historyWeight_ = 1.0 / (float) historyDepth_;
}

void MocapElement::setPosition(ofPoint position)
{
	if (position_.size() <= historyDepth_) {
        position_.insert(position_.begin(), position);
	}
	if (position_.size() > historyDepth_) {
		position_.pop_back();
	}
}

void MocapElement::setPositionFiltered(ofPoint positionFiltered)
{
	if (positionFiltered_.size() <= historyDepth_) {
        positionFiltered_.insert(positionFiltered_.begin(), positionFiltered);
	}
	if (positionFiltered_.size() > historyDepth_) {
		positionFiltered_.pop_back();
	}
}

void MocapElement::setVelocity(ofPoint velocity)
{
	if (velocity_.size() <= historyDepth_) {
        velocity_.insert(velocity_.begin(), velocity);
	}
	if (velocity_.size() > historyDepth_) {
		velocity_.pop_back();
	}
}

void MocapElement::setVelocityMagnitude(float velocityMagnitude)
{
    if (velocityMagnitude_.size() <= historyDepth_) {
        velocityMeanRunning_ += (velocityMagnitude * historyWeight_);
        velocityMagnitude_.insert(velocityMagnitude_.begin(), velocityMagnitude);
	}
	if (velocityMagnitude_.size() > historyDepth_) {
        velocityMeanRunning_ -= (velocityMagnitude_.back() * historyWeight_);
		velocityMagnitude_.pop_back();
	}
    setVelocityMean(velocityMeanRunning_);
}

void MocapElement::setVelocityMean(float velocityMean)
{
    if (velocityMean_.size() <= historyDepth_) {
        velocityMean_.insert(velocityMean_.begin(), velocityMean);
	}
	if (velocityMean_.size() > historyDepth_) {
		velocityMean_.pop_back();
	}
}

void MocapElement::setAcceleration(ofPoint acceleration)
{
	if (acceleration_.size() <= historyDepth_) {
        acceleration_.insert(acceleration_.begin(), acceleration);
	}
	if (acceleration_.size() > historyDepth_) {
		acceleration_.pop_back();
	}
}

void MocapElement::setAccelerationMagnitude(float accelerationMagnitude)
{
    if (accelerationMagnitude_.size() <= historyDepth_) {
        accelerationMeanRunning_ += (accelerationMagnitude * historyWeight_);
        accelerationMagnitude_.insert(accelerationMagnitude_.begin(), accelerationMagnitude);
	}
	if (accelerationMagnitude_.size() > historyDepth_) {
        accelerationMeanRunning_ -= (accelerationMagnitude_.back() * historyWeight_);
		accelerationMagnitude_.pop_back();
	}
    setAccelerationMean(accelerationMeanRunning_);
}

void MocapElement::setAccelerationMean(float accelerationMean)
{
    if (accelerationMean_.size() <= historyDepth_) {
        accelerationMean_.insert(accelerationMean_.begin(), accelerationMean);
	}
	if (accelerationMean_.size() > historyDepth_) {
		accelerationMean_.pop_back();
	}
}

void MocapElement::setAccelerationTrajectory(float accelerationTrajectory)
{
    if (accelerationTrajectory_.size() <= historyDepth_) {
        accelerationTrajectory_.insert(accelerationTrajectory_.begin(), accelerationTrajectory);
	}
	if (accelerationTrajectory_.size() > historyDepth_) {
		accelerationTrajectory_.pop_back();
	}
}

void MocapElement::setDistanceToTorso(float distanceToTorso)
{
    if (distanceToTorso_.size() <= historyDepth_) {
        distanceToTorso_.insert(distanceToTorso_.begin(), distanceToTorso);
	}
	if (distanceToTorso_.size() > historyDepth_) {
		distanceToTorso_.pop_back();
	}
}

void MocapElement::setRelativePositionToTorso(ofPoint relativePositionToTorso)
{
    if (relativePositionToTorso_.size() <= historyDepth_) {
        relativePositionToTorso_.insert(relativePositionToTorso_.begin(), relativePositionToTorso);
	}
	if (relativePositionToTorso_.size() > historyDepth_) {
		relativePositionToTorso_.pop_back();
	}
}