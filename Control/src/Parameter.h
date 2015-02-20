#pragma once

/*
 
 
 move warp to just GUI?
 
 xml / midi / osc
  - save to
  - load from
 
 */




class ParameterBase
{
public:
    
    virtual ~ParameterBase() { }
    
    // name
    string getName() {return name; }
    void setName(string name) {this->name = name;}
    
    // osc address
    string getOscAddress() {return oscAddress;}
    void setOscAddress(string oscAddress)
    {
        this->oscAddress = oscAddress;
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
    
    // warp
    float getWarp() {return warp;}
    void setWarp(float warp) {this->warp = warp;}
    
    // data
    string name;
    string oscAddress;
    float warp;
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
        warp = 1.0;
    }
    
    Parameter(string name, T *val, T min, T max, float warp_=1.0) : minValue(min), maxValue(max)
    {
        this->name = name;
        setOscAddress("/"+name);
        value = val;
        warp = warp_;
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
    
protected:
    
    T *value;
    T minValue, maxValue;
};


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

