#pragma once

/*
 
 
 move warp to just GUI?
 
 xml / midi / osc
  - save to
  - load from
 
 */

#include "ofMain.h"
#include "ofxOsc.h"


class ParameterBase
{
public:
    
    virtual ~ParameterBase() { }
    
    // name
    string getName() {return name; }
    void setName(string name) {this->name = name;}
    
    // osc address
    string getOscAddress() {return oscAddress;}
    void setOscAddress(string oscAddress_)
    {
        this->oscAddress = oscAddress_;
        ofStringReplace(oscAddress, " ", "_");
    }
    
    // reference
    template<class T> void setReference(T *val);
    template<class T> T* getReference();
    
    // value
    template<class T> void set(T val);
    template<class T> T get();
    
    // min
    template<class T> void setMin(T min);
    template<class T> T getMin();
    
    // max
    template<class T> void setMax(T max);
    template<class T> T getMax();

    // osc
    virtual bool valueChanged() {}
    virtual void sendOsc(ofxOscMessage &m) {}
    virtual void receiveOsc(ofxOscMessage &m) {}
    
    // xml
    virtual void getXml(ofXml &xml) {}
    virtual void loadFromXml(ofXml &xml) {}
    
    // data
    string name;
    string oscAddress;
};


template <typename T>
class Parameter : public ParameterBase
{
public:
    Parameter(string name, T *val) : minValue(*val), maxValue(*val)
    {
        this->name = name;
        setOscAddress("/"+name);
        value = val;
    }
    
    Parameter(string name, T *val, T min, T max) : minValue(min), maxValue(max)
    {
        this->name = name;
        setOscAddress("/"+name);
        value = val;
    }

    Parameter(string name, T min, T max) : value(new T()), minValue(min), maxValue(max)
    {
        this->name = name;
        setOscAddress("/"+name);
    }
    
    bool valueChanged()
    {
        bool changed = (*value != previous);
        previous = *value;
        return changed;
    }

    //value
    T get() {return *value;}
    void set(const T& val) {*value=val;}
    T* getReference() {return value;}
    
    // min
    T getMin() {return minValue;}
    void setMin(T min) {minValue=min;}
    
    // max
    T getMax() {return maxValue;}
    void setMax(T max) {maxValue=max;}
    
    // osc
    void sendOsc(ofxOscMessage &m);
    void receiveOsc(ofxOscMessage &m);
    
    // xml
    void getXml(ofXml &xml);
    void loadFromXml(ofXml &xml) { }
    
protected:
    
    T *value;
    T previous;
    T minValue, maxValue;
};


// send osc templates

template<typename T> void Parameter<T>::sendOsc(ofxOscMessage &m) { }

template<> inline void Parameter<bool>::sendOsc(ofxOscMessage &m)
{
    m.setAddress(oscAddress);
    m.addIntArg(*value ? 1 : 0);
}

template<> inline void Parameter<int>::sendOsc(ofxOscMessage &m)
{
    m.setAddress(oscAddress);
    m.addIntArg(*value);
}

template<> inline void Parameter<float>::sendOsc(ofxOscMessage &m)
{
    m.setAddress(oscAddress);
    m.addFloatArg(*value);
}

template<> inline void Parameter<double>::sendOsc(ofxOscMessage &m)
{
    m.setAddress(oscAddress);
    m.addFloatArg(*value);
}

template<> inline void Parameter<ofVec2f>::sendOsc(ofxOscMessage &m)
{
    m.setAddress(oscAddress);
    m.addFloatArg(value->x);
    m.addFloatArg(value->y);
}

template<> inline void Parameter<ofVec3f>::sendOsc(ofxOscMessage &m)
{
    m.setAddress(oscAddress);
    m.addFloatArg(value->x);
    m.addFloatArg(value->y);
}

template<> inline void Parameter<ofVec4f>::sendOsc(ofxOscMessage &m)
{
    m.setAddress(oscAddress);
    m.addFloatArg(value->x);
    m.addFloatArg(value->y);
    m.addFloatArg(value->z);
    m.addFloatArg(value->w);
}

template<> inline void Parameter<ofFloatColor>::sendOsc(ofxOscMessage &m)
{
    m.setAddress(oscAddress);
    m.addFloatArg(value->r);
    m.addFloatArg(value->g);
    m.addFloatArg(value->b);
    m.addFloatArg(value->a);
}

template<> inline void Parameter<string>::sendOsc(ofxOscMessage &m)
{
    m.setAddress(oscAddress);
    m.addStringArg(*value);
}


// receive osc templates

template<typename T> void Parameter<T>::receiveOsc(ofxOscMessage &m) { }

template<> inline void Parameter<bool>::receiveOsc(ofxOscMessage &m)
{
    *value = m.getArgAsInt32(0);
}

template<> inline void Parameter<int>::receiveOsc(ofxOscMessage &m)
{
    *value = m.getArgAsInt32(0);
}

template<> inline void Parameter<float>::receiveOsc(ofxOscMessage &m)
{
    *value = m.getArgAsFloat(0);
}

template<> inline void Parameter<double>::receiveOsc(ofxOscMessage &m)
{
    *value = m.getArgAsFloat(0);
}

template<> inline void Parameter<ofVec2f>::receiveOsc(ofxOscMessage &m)
{
    value->set(m.getArgAsFloat(0), m.getArgAsFloat(1));
}

template<> inline void Parameter<ofVec3f>::receiveOsc(ofxOscMessage &m)
{
    value->set(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
}

template<> inline void Parameter<ofVec4f>::receiveOsc(ofxOscMessage &m)
{
    value->set(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2), m.getArgAsFloat(3));
}

template<> inline void Parameter<ofFloatColor>::receiveOsc(ofxOscMessage &m)
{
    value->set(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2), m.getArgAsFloat(3));
}

template<> inline void Parameter<string>::receiveOsc(ofxOscMessage &m)
{
    *value = m.getArgAsString(0);
}


// get xml templates

template<typename T> void Parameter<T>::getXml(ofXml &xml) { }

template<> inline void Parameter<bool>::getXml(ofXml &xml)
{

}

template<> inline void Parameter<int>::getXml(ofXml &xml)
{
    xml.addChild("name");
    

}

template<> inline void Parameter<float>::getXml(ofXml &xml)
{

}

template<> inline void Parameter<double>::getXml(ofXml &xml)
{

}

template<> inline void Parameter<ofVec2f>::getXml(ofXml &xml)
{

}

template<> inline void Parameter<ofVec3f>::getXml(ofXml &xml)
{

}

template<> inline void Parameter<ofVec4f>::getXml(ofXml &xml)
{

}

template<> inline void Parameter<ofFloatColor>::getXml(ofXml &xml)
{

}

template<> inline void Parameter<string>::getXml(ofXml &xml)
{

}




// dynamic casting

// reference
template<class T> T* ParameterBase::getReference() { return dynamic_cast<Parameter<T>&>(*this).getReference(); }
template<class T> void ParameterBase::setReference(T *val) { return dynamic_cast<Parameter<T>&>(*this).setReference(); }

// get
template<class T> T ParameterBase::get() { return dynamic_cast<Parameter<T>&>(*this).get(); }
template<class T> void ParameterBase::set(T val) { return dynamic_cast<Parameter<T>&>(*this).set(); }

// min
template<class T> T ParameterBase::getMin() { return dynamic_cast<Parameter<T>&>(*this).getMin(); }
template<class T> void ParameterBase::setMin(T min) { return dynamic_cast<Parameter<T>&>(*this).setMin(); }

// max
template<class T> T ParameterBase::getMax() { return dynamic_cast<Parameter<T>&>(*this).getMax(); }
template<class T> void ParameterBase::setMax(T max) { return dynamic_cast<Parameter<T>&>(*this).setMax(); }

