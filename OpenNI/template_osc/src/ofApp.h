#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "ofxSecondWindow.h"

#include "ofxOsc.h"


class ofApp : public ofBaseApp{
    
public:
    

    
    
    struct JointTracker {
        ofPoint position;
        ofPoint velo;
        ofPoint min, max;
        JointTracker() {
            min = ofPoint(9999,9999,9999);
            max = ofPoint(-9999, -9999, -9999);
        }
        void trackMinMax() {
            if (position.x < min.x) min.x = position.x;
            if (position.y < min.y) min.y = position.y;
            if (position.z < min.z) min.z = position.z;
            if (position.x > max.x) max.x = position.x;
            if (position.y > max.y) max.y = position.y;
            if (position.z > max.z) max.z = position.z;
        }
        void set(ofPoint pos, bool toTrackMinMax, bool toNormalize=true)
        {
            position.set(pos.x, pos.y, pos.z);
            if (toTrackMinMax) {
                trackMinMax();
            }
            if (toNormalize) {
                position.x = (position.x - min.x) / (max.x - min.x);
                position.y = (position.y - min.y) / (max.y - min.y);
                position.z = (position.z - min.z) / (max.z - min.z);
            }
        }
    };
    
    
    struct SkeletonSender
    {
        ofPoint point;
        string address;
        bool active = false;
        
        int device;
        bool hasDevice;
        
        int parameter;
        bool hasParameter;
        
        JointTracker *joint;
        bool hasJoint = false;
        int dim = 0;
        
        float min = 0;
        float max = 0;
        
        void setJoint(JointTracker *joint, int dim=0) {
            this->joint = joint;
            this->dim = dim;
            hasJoint = true;
        }
        
        void setMin(float min) {this->min = min;}
        void setMax(float max) {this->max = max;}
        
        SkeletonSender(string address, int device, int parameter) {
            this->address = address;
            this->device = device;
            this->parameter = parameter;
            hasDevice = true;
            hasParameter = true;
        }

        SkeletonSender(string address, int device, int parameter, float min, float max) {
            this->address = address;
            this->device = device;
            this->parameter = parameter;
            this->min = min;
            this->max = max;
            hasDevice = device!=-1;
            hasParameter = parameter!=-1;
        }

        void setupMessage(ofxOscMessage &msg)
        {
            msg.setAddress(address);
            if (hasDevice) {
                msg.addIntArg(device);
            }
            if (hasParameter) {
                msg.addIntArg(parameter);
            }
            if (hasJoint) {
                float value;
                if      (dim == 0)   value = ofMap(joint->position.x, 0.0, 1.0, min, max);
                else if (dim == 1)   value = ofMap(joint->position.y, 0.0, 1.0, min, max);
                else if (dim == 2)   value = ofMap(joint->position.z, 0.0, 1.0, min, max);
                msg.addFloatArg(value);
                cout << "SEND "<<address<< " "<<dim << " " << ofToString(value) << endl;
            }
            
            
            
        }
    };
    
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
    
    int id;
    float *x;
    ofPoint *p;
    bool done;
    OpenNI kinect;
    
    ofPoint head, ctr;
    
    ofxSecondWindow window;
    
    
    ofxOscSender send;
    
    vector<SkeletonSender*> skels;
    vector<JointTracker*> joints;
    
    
    bool trackMinMax = true;
    bool toNormalize = true;
};
