#pragma once

#include "ofMain.h"


template <typename T>
class TimeFunction
{
public:
    TimeFunction(bool autoUpdate)
    {
        setup(autoUpdate);
    }
    
    TimeFunction()
    {
        setup(true);
    }
    
    ~TimeFunction()
    {
        setAutoUpdate(false);
    }
    
    void setup(bool autoUpdate)
    {
        setAutoUpdate(autoUpdate);
        noiseOffset = ofVec3f(ofRandom(100), ofRandom(100), ofRandom(100));
        sinePhase = ofVec3f(ofRandom(TWO_PI), ofRandom(TWO_PI), ofRandom(TWO_PI));
        delTime = 1.0;
        reset();
    }
    
    void setAutoUpdate(bool autoUpdate)
    {
        this->autoUpdate = autoUpdate;
        if (autoUpdate) {
            ofAddListener(ofEvents().update, this, &TimeFunction::update);
        }
        else {
            ofRemoveListener(ofEvents().update, this, &TimeFunction::update);
        }
    }
    
    void reset();
    
    /* getters */
    T getConstant() { return constant; }
    T getTimeCoefficient() { return timeCoefficient; }
	T getRandMin() { return randMin; }
	T getRandMax() { return randMax; }
	T getNoiseMin() { return noiseMin; }
	T getNoiseMax() { return noiseMax; }
	T getNoiseSpeed() { return noiseSpeed; }
	ofVec3f getNoiseOffset() { return noiseOffset; }
	T getSineMin() { return sineMin; }
	T getSineMax() { return sineMax; }
	T getSineFreq() { return sineFreq; }
	ofVec3f getSinePhase() { return sinePhase; }
	T getDelTime() { return delTime; }
	
	/* reference getters */
    T & getConstantRef() { return constant; }
    T & getTimeCoefficientRef() { return timeCoefficient; }
    T & getRandMinRef() { return randMin; }
    T & getRandMaxRef() { return randMax; }
    T & getNoiseMinRef() { return noiseMin; }
    T & getNoiseMaxRef() { return noiseMax; }
    T & getNoiseSpeedRef() { return noiseSpeed; }
    ofVec3f & getNoiseOffsetRef() { return noiseOffset; }
    T & getSineMinRef() { return sineMin; }
    T & getSineMaxRef() { return sineMax; }
    T & getSineFreqRef() { return sineFreq; }
    ofVec3f & getSinePhaseRef() { return sinePhase; }
    T & getDelTimeRef() { return delTime; }
    
    /* setters */
	void setConstant(T z) { this->constant = z; }
	void setTimeCoefficient(T z) { this->timeCoefficient = z; }
	void setRandMin(T z) { this->randMin = z; }
	void setRandMax(T z) { this->randMax = z; }
	void setNoiseMin(T z) { this->noiseMin = z; }
	void setNoiseMax(T z) { this->noiseMax = z; }
	void setNoiseSpeed(T z) { this->noiseSpeed = z; }
	void setNoiseOffset(ofVec3f z) { this->noiseOffset = z; }
	void setSineMin(T z) { this->sineMin = z; }
	void setSineMax(T z) { this->sineMax = z; }
	void setSineFreq(T z) { this->sineFreq = z; }
	void setSinePhase(ofVec3f z) { this->sinePhase = z; }
	void setTime(T z) { this->time = z; }
	void setDelTime(float z) { this->delTime = z; }
    
    /* main function setters */
	void setFunctionRandom(T randMin, T randMax)
    {
	    this->randMin = randMin;
	    this->randMax = randMax;
	}
	
    void setFunctionNoise(T noiseMin, T noiseMax, T noiseSpeed, ofVec3f noiseOffset)
    {
	    this->noiseMin = noiseMin;
	    this->noiseMax = noiseMax;
	    this->noiseSpeed = noiseSpeed;
	    this->noiseOffset = noiseOffset;
	}
	
    void setFunctionNoise(T noiseMin, T noiseMax, T noiseSpeed)
    {
        setFunctionNoise(noiseMin, noiseMax, noiseSpeed,
                         ofVec3f(ofRandom(100), ofRandom(100), ofRandom(100)));
    }
	
    void setFunctionSine(T sineMin, T sineMax, T sineFreq, ofVec3f sinePhase)
    {
	    this->sineMin = sineMin;
	    this->sineMax = sineMax;
	    this->sineFreq = sineFreq;
	    this->sinePhase = sinePhase;
	}
    
    void setFunctionSine(T sineMin, T sineMax, T sineFreq)
    {
        setFunctionSine(sineMin, sineMax, sineFreq,
                        ofVec3f(ofRandom(TWO_PI), ofRandom(TWO_PI), ofRandom(TWO_PI)));
    }
    
    /* update time factors */
    void update()
    {
        timeFactor += (delTime * timeCoefficient);
        noiseFactor += (delTime * noiseSpeed);
        sineFactor += (delTime * sineFreq);
    }
    
    /* sample current value */
    T getRef() { return &value; }
    T get();
    
private:
    
    void update(ofEventArgs &data)
    {
        update();
    }
    
    T constant;
    T timeCoefficient;
    T randMin, randMax;
    T noiseMin, noiseMax, noiseSpeed;
    T sineMin, sineMax, sineFreq;
    T value;
 	float delTime;
    ofVec3f noiseOffset, sinePhase;
    
    T noiseFactor;
    T sineFactor;
    T timeFactor;
    
    bool autoUpdate;
};


template<> inline void TimeFunction<int>::reset()
{
    setConstant(0);
    setTimeCoefficient(0);
    setFunctionNoise(0, 0, 0);
    setFunctionRandom(0, 0);
    setFunctionSine(0, 0, 0);
}
template<> inline void TimeFunction<float>::reset()
{
    setConstant(0.0);
    setTimeCoefficient(0.0);
    setFunctionNoise(0.0, 0.0, 0.0);
    setFunctionRandom(0.0, 0.0);
    setFunctionSine(0.0, 0.0, 0.0);
}
template<> inline void TimeFunction<ofVec3f>::reset()
{
    setConstant(ofVec3f(0, 0, 0));
    setTimeCoefficient(ofVec3f(0, 0, 0));
    setFunctionNoise(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), ofVec3f(0, 0, 0));
    setFunctionRandom(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0));
    setFunctionSine(ofVec3f(0, 0, 0), ofVec3f(0, 0, 0), ofVec3f(0, 0, 0));
}
template<> inline void TimeFunction<ofVec2f>::reset()
{
    setConstant(ofVec2f(0, 0));
    setTimeCoefficient(ofVec2f(0, 0));
    setFunctionNoise(ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(0, 0));
    setFunctionRandom(ofVec2f(0, 0), ofVec2f(0, 0));
    setFunctionSine(ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(0, 0));
}


template<> inline int TimeFunction<int>::get()
{
    value = constant +
              timeFactor +
              ofRandom(randMin, randMax) +
              ofMap(ofNoise(noiseFactor, noiseOffset.x), 0, 1, noiseMin, noiseMax) +
              ofMap(sin(sineFactor + sinePhase.x), -1, 1, sineMin, sineMax) ;
    return value;
}

template<> inline float TimeFunction<float>::get()
{
    value = constant +
              timeFactor +
              ofRandom(randMin, randMax) +
              ofMap(ofNoise(noiseFactor, noiseOffset.x), 0, 1, noiseMin, noiseMax) +
              ofMap(sin(sineFactor + sinePhase.x), -1, 1, sineMin, sineMax) ;
    return value;
}

template<> inline ofVec3f TimeFunction<ofVec3f>::get()
{
    value.set(constant +
              ofVec3f(timeFactor.x,
                      timeFactor.y,
                      timeFactor.z) +
              ofVec3f(ofRandom(randMin.x, randMax.x),
                      ofRandom(randMin.y, randMax.y),
                      ofRandom(randMin.z, randMax.z)) +
              ofVec3f(ofMap(ofNoise(noiseFactor.x, noiseOffset.x), 0, 1, noiseMin.x, noiseMax.x),
                      ofMap(ofNoise(noiseFactor.y, noiseOffset.y), 0, 1, noiseMin.y, noiseMax.y),
                      ofMap(ofNoise(noiseFactor.z, noiseOffset.z), 0, 1, noiseMin.z, noiseMax.z)) +
              ofVec3f(ofMap(sin(sineFactor.x + sinePhase.x), -1, 1, sineMin.x, sineMax.x),
                      ofMap(sin(sineFactor.y + sinePhase.y), -1, 1, sineMin.y, sineMax.y),
                      ofMap(sin(sineFactor.z + sinePhase.z), -1, 1, sineMin.z, sineMax.z)) );
    return value;
}

template<> inline ofVec2f TimeFunction<ofVec2f>::get()
{
    value.set(constant +
              ofVec2f(timeFactor.x,
                      timeFactor.y) +
              ofVec2f(ofRandom(randMin.x, randMax.x),
                      ofRandom(randMin.y, randMax.y))+
              ofVec2f(ofMap(ofNoise(noiseFactor.x, noiseOffset.x), 0, 1, noiseMin.x, noiseMax.x),
                      ofMap(ofNoise(noiseFactor.y, noiseOffset.y), 0, 1, noiseMin.y, noiseMax.y))+
              ofVec2f(ofMap(sin(sineFactor.x + sinePhase.x), -1, 1, sineMin.x, sineMax.x),
                      ofMap(sin(sineFactor.y + sinePhase.y), -1, 1, sineMin.y, sineMax.y)) );
    return value;
}
